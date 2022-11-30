/* 
* MIT License
* 
* Copyright (c) 2021 juliokscesar
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "griffinLog.h"

#include <cstdint>
#include <ctime>
#include <cstdio>
#include <array>
#include <vector>
#include <utility>
#include <memory>

#define EXPAND_AND_LOG(lvl, what) va_list vaArgs; \
                                  va_start(vaArgs, what); \
                                  log(lvl, what, vaArgs); \
                                  va_end(vaArgs)


namespace grflog
{
    namespace utils
    {
        void make_directory(const std::string& dir_path)
        {
            #if defined(GRIFFIN_LOG_WIN32)

            CreateDirectoryA(dir_path.c_str(), nullptr);

            #elif defined(GRIFFIN_LOG_LINUX)

            mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            #endif // GRIFFIN_LOG_WIN32
        }

        const std::string get_date_time()
        {
            std::time_t t = std::time(nullptr);
            std::tm now;

            #if defined(GRIFFIN_LOG_WIN32) && defined(_MSC_VER)
            if (localtime_s(&now, &t) != 0)
            {
                std::perror("Error occured while trying to take time:");
                return std::string("");
            }
            #else
            now = *(localtime(&t));
            #endif // GRIFFIN_LOG_WIN32 && _MSC_VER

            char datetime[21];
            strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", &now);

            return std::string(std::move(datetime));
        }

        std::string fmt_str(const std::string& fmt, va_list vaArgs)
        {
            const std::size_t str_size = fmt.size() + 256;

            std::unique_ptr<char[]> dest(new char[str_size]);
            vsnprintf(dest.get(), str_size - 1, fmt.c_str(), vaArgs);

            return std::string(dest.get());
        }
    }

    namespace visual
    {
        const std::string get_log_lvl_str(const log_level& lvl)
        {
            static const std::array<const std::string, 5> log_level_strs = { "INFO", "DEBUG", "WARN", "CRITICAL", "FATAL" };
            return log_level_strs[static_cast<uint32_t>(lvl)];
        }

        const GRIFFIN_COLOR get_log_lvl_color(const log_level& lvl)
        {
            static const std::array<const GRIFFIN_COLOR, 5> log_level_colors = { GRIFFIN_COLOR_BLUE, GRIFFIN_COLOR_GREEN, GRIFFIN_COLOR_YELLOW, GRIFFIN_COLOR_RED, GRIFFIN_COLOR_BLACK_RED };
            return log_level_colors[static_cast<uint32_t>(lvl)];
        }

        #ifdef GRIFFIN_LOG_WIN32
        /// Function to get the default text attributes in Windows. Really bad, but will stay like that for now.
        /// @returns WORD default console's attributes.
        static WORD get_win32_default_attributes()
        {
            static bool already_got = false;
            static WORD attrb;

            if (!already_got)
            {
                CONSOLE_SCREEN_BUFFER_INFO Info;
                HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                GetConsoleScreenBufferInfo(hStdout, &Info);
                attrb = Info.wAttributes;
                already_got = true;
            }
            
            return attrb;
        }
        #endif // GRIFFIN_LOG_WIN32

        void set_text_color(const log_level& lvl)
        {
            const GRIFFIN_COLOR color = get_log_lvl_color(lvl);

            #if defined(GRIFFIN_LOG_WIN32)
            
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

            #elif defined(GRIFFIN_LOG_LINUX)

            std::fputs(color.c_str(), stdout);

            #endif
        }

        void reset_text_color()
        {
             #if defined(GRIFFIN_LOG_WIN32)

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), get_win32_default_attributes());

            #elif defined(GRIFFIN_LOG_LINUX)

            std::fputs(GRIFFIN_COLOR_RESET, stdout);

            #endif
        }
    }

    void log(log_level lvl, const std::string& what, va_list vaArgs)
    {
        log_event l_ev(lvl, utils::fmt_str(what, vaArgs));

        // TODO: implement std::string logging
        console_log(l_ev);
        file_log(l_ev);
    }

    void console_log(const log_event& l_ev)
    {
        visual::reset_text_color();

        std::fprintf(stdout, "[%s] [", l_ev.date_time.c_str());

        visual::set_text_color(l_ev.lvl);
        std::fputs(l_ev.log_lvl_str.c_str(), stdout);
        visual::reset_text_color();

        std::fprintf(stdout, "] %s\n", l_ev.content.c_str());
    }


    // File logging function and class implementations

    /* class file_logger */
    file_logger::file_logger()
    {
        m_file_name = "";
        m_file_path = "./logs/";
    }

    file_logger::file_logger(const std::string& file_name)
    {
        set_file_name(file_name);
    }

    file_logger::file_logger(const file_logger& other)
    {
        copy_from(other);
    }

    void file_logger::copy_from(const file_logger& other)
    {
        set_file_name(other.m_file_name);
    }

    bool file_logger::is_initialized()
    {
        return m_file.is_open();
    }

    bool file_logger::init_file_logging()
    {
        if (m_file_name.empty() && m_file_path.empty())
            set_file_name("grflog_file.log");

        if (!is_initialized())
        {
            utils::make_directory("./logs");
            m_file.open(m_file_path, std::ios::out);
        }
        else
        {
            finish_file_logging();
            m_file.open(m_file_path);
        }

        return is_initialized();
    }

    void file_logger::write_to_file(const std::string& what)
    {
        m_file << what;
    }

    const std::string file_logger::get_file_name()
    {
        return m_file_name;
    }

    void file_logger::set_file_name(const std::string& file_name)
    {
        m_file_name = file_name;
        m_file_path = "./logs/" + m_file_name;
    }

    void file_logger::finish_file_logging()
    {
        if (is_initialized())
        {
            m_file.flush();
            m_file.close();
        }
    }

    file_logger::~file_logger()
    {
        finish_file_logging();
    }


    /// Get the file used for logging.
    /// @returns A file_logger reference to keep track of file to log.
    static file_logger& get_file_logger()
    {
        static file_logger f;
        return f;
    }

    void set_file_logger(const file_logger& file)
    {
        file_logger& fl = get_file_logger();

        fl.copy_from(file);
        if (!fl.init_file_logging())
            critical("Couldn't add file");
    }

    void stop_file_logging()
    {
        get_file_logger().finish_file_logging();
    }

    void file_log(const log_event& l_ev)
    {
        file_logger& fl = get_file_logger();
        if (fl.is_initialized())
            fl.write_to_file("[" + l_ev.date_time + "] [" + l_ev.log_lvl_str + "] " + l_ev.content + "\n");
    }


    // Logging level implemented functions

    void info(const std::string& what, ...)
    {
        EXPAND_AND_LOG(log_level::INFO, what);
    }

    void debug(const std::string& what, ...)
    {
        EXPAND_AND_LOG(log_level::DEBUG, what);
    }

    void warn(const std::string& what, ...)
    {
        EXPAND_AND_LOG(log_level::WARN, what);
    }

    void critical(const std::string& what, ...)
    {
        EXPAND_AND_LOG(log_level::CRITICAL, what);
    }

    void fatal(const std::string& what, ...)
    {
        EXPAND_AND_LOG(log_level::FATAL, what);
    }
}

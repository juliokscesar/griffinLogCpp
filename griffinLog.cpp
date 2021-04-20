#include "griffinLog.h"

#include <ctime>
#include <cstdio>
#include <array>
#include <vector>
#include <thread>
#include <exception>

#ifdef GRIFFIN_LOG_WIN32
    #include <Windows.h>
#elif defined(GRIFFIN_LOG_LINUX)
    #include <sys/stat.h>
#endif // GRIFFIN_LOG_WIN32

namespace grflog
{
    namespace utils
    {
        void make_directory(const std::string& dir_path)
        {
            #ifdef GRIFFIN_LOG_WIN32

            CreateDirectoryA(dir_path.c_str(), nullptr);

            #elif defined(GRIFFIN_LOG_LINUX)

            mkdir(dir_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

            #endif // GRIFFIN_LOG_WIN32
        }

        const std::string get_date_time()
        {
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t);

            char datetime[20];
            strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", now);

            return std::string(datetime);
        }
    }

    namespace visual
    {
        const std::string get_log_lvl_str(const log_level& lvl)
        {
            static std::array<std::string, 5> log_level_strs = { "INFO", "DEBUG", "WARN", "CRITICAL", "FATAL" };
            return log_level_strs[static_cast<uint32_t>(lvl)];
        }

        const GRIFFIN_COLOR get_log_lvl_color(const log_level& lvl)
        {
            static std::array<GRIFFIN_COLOR, 5> log_level_colors = { GRIFFIN_COLOR_BLUE, GRIFFIN_COLOR_GREEN, GRIFFIN_COLOR_YELLOW, GRIFFIN_COLOR_RED, GRIFFIN_COLOR_BLACK_RED };
            return log_level_colors[static_cast<uint32_t>(lvl)];
        }

        const GRIFFIN_COLOR_RES set_text_color(const GRIFFIN_COLOR& color)
        {
            #if defined(GRIFFIN_LOG_WIN32)

            // TODO: Implement set text color for Win32

            #elif defined(GRIFFIN_LOG_LINUX)

            return GRIFFIN_COLOR_RES(color);

            #endif
        }

        const GRIFFIN_COLOR_RES reset_text_color()
        {
             #if defined(GRIFFIN_LOG_WIN32)

            // TODO: Implement reset text color for Win32

            #elif defined(GRIFFIN_LOG_LINUX)

            return GRIFFIN_COLOR_RES(GRIFFIN_COLOR_RESET);

            #endif
        }
    }

    void log(log_level lvl, const std::string& what, va_list vaArgs)
    {
        const std::string datetime = utils::get_date_time();
        
        const std::string lvl_str = visual::get_log_lvl_str(lvl);
        const GRIFFIN_COLOR lvl_color = visual::get_log_lvl_color(lvl);

        std::size_t fmt_log_sz = what.size() + 256;
        char fmt_log[fmt_log_sz];
        vsnprintf(fmt_log, fmt_log_sz - 1, what.c_str(), vaArgs);

        log_event l_ev(datetime, lvl_str, lvl_color, std::string(fmt_log));

        std::thread console_thrd(console_log, l_ev);
        std::thread file_thrd(file_log, l_ev);

        console_thrd.join();
        file_thrd.join();
    }

    void console_log(const log_event& l_ev)
    {
        std::cout << "[" << l_ev.date_time << "] [";
        std::cout << visual::set_text_color(l_ev.log_lvl_color) << l_ev.log_lvl_str << visual::reset_text_color();
        std::cout << "] " << l_ev.formatted_what << "\n";
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
        m_file_name = file_name;
        m_file_path = "./logs/" + m_file_name;
    }

    file_logger::file_logger(const file_logger& other)
    {
        copy_from(other);
    }

    void file_logger::copy_from(const file_logger& other)
    {
        m_file_name = other.m_file_name;
        m_file_path = "./logs/" + m_file_name;
    }

    bool file_logger::init_file_logging()
    {
        if (m_file_name == "" && m_file_path == "")
            set_file_name("grflog_file.log");

        if (!m_file.is_open())
        {
            utils::make_directory("./logs");
            m_file.open(m_file_path, std::ios::out);
        }

        return m_file.is_open();
    }

    bool file_logger::is_initialized()
    {
        return m_file.is_open();
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
        if (m_file_name == "" && m_file_path == "")
        {
            m_file_name = file_name;
            m_file_path = "./logs/" + m_file_name;
        }

        else
            throw std::runtime_error("file_logger (set_file_name()): Trying to update file's name, but it was already provided.");
    }

    void file_logger::finish_file_logging()
    {
        if (is_initialized())
        {
            m_file.flush();
            m_file.close();

            m_file_name = "";
            m_file_path = "";
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

    void add_file_logger(const file_logger& file)
    {
        file_logger& fl = get_file_logger();

        if (fl.is_initialized())
            fl.finish_file_logging();

        fl.copy_from(file);

        if (!fl.init_file_logging())
            critical("Couldn't add file");
    }

    void file_log(const log_event& l_ev)
    {
        file_logger& fl = get_file_logger();
        if (fl.is_initialized())
            fl.write_to_file("[" + l_ev.date_time + "] [" + l_ev.log_lvl_str + "] " + l_ev.formatted_what + "\n");
    }


    // Logging level implemented functions

    void info(const std::string& what, ...)
    {
        va_list vaArgs;
        va_start(vaArgs, what);
        log(log_level::INFO, what, vaArgs);
        va_end(vaArgs);
    }

    void debug(const std::string& what, ...)
    {
        va_list vaArgs;
        va_start(vaArgs, what);
        log(log_level::DEBUG, what, vaArgs);
        va_end(vaArgs);
    }

    void warn(const std::string& what, ...)
    {
        va_list vaArgs;
        va_start(vaArgs, what);
        log(log_level::WARN, what, vaArgs);
        va_end(vaArgs);
    }

    void critical(const std::string& what, ...)
    {
        va_list vaArgs;
        va_start(vaArgs, what);
        log(log_level::CRITICAL, what, vaArgs);
        va_end(vaArgs);
    }

    void fatal(const std::string& what, ...)
    {
        va_list vaArgs;
        va_start(vaArgs, what);
        log(log_level::FATAL, what, vaArgs);
        va_end(vaArgs);
    }
}

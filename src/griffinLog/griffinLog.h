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


#ifndef GRIFFIN_LOG_H
#define GRIFFIN_LOG_H

#include <iostream>
#include <fstream>
#include <cstdarg>
#include <cstdint>
#include <utility>

/* GRIFFIN LOG PLATFORM DEFINITIONS */
#if defined(WIN32) || defined(_WIN32)
    #define GRIFFIN_LOG_WIN32
#else
    #if defined(__linux__) && !defined(__ANDROID__)
        #define GRIFFIN_LOG_LINUX
    #else
        #error Griffin log currently supports Win32 and Linux only
    #endif // __linux__ && !__ANDROID__
#endif // WIN32 || _WIN32


#if defined(GRIFFIN_LOG_WIN32)
    #include <Windows.h>
#elif defined(GRIFFIN_LOG_LINUX)
    #include <sys/stat.h>
#endif // GRIFFIN_LOG_WIN32

// Griffin Color Definitions
#if defined(GRIFFIN_LOG_WIN32)
    typedef DWORD GRIFFIN_COLOR;

    #define GRIFFIN_COLOR_RED       0x0c
    #define GRIFFIN_COLOR_GREEN     0x0a
    #define GRIFFIN_COLOR_BLUE      0x09
    #define GRIFFIN_COLOR_YELLOW    0x0e
    #define GRIFFIN_COLOR_BLACK_RED 0xc0

#elif defined(GRIFFIN_LOG_LINUX)
    typedef std::string GRIFFIN_COLOR;

    #define GRIFFIN_COLOR_RED       "\x1b[31;1;1m"
    #define GRIFFIN_COLOR_GREEN     "\x1b[32;1;1m"
    #define GRIFFIN_COLOR_BLUE      "\x1b[34;1;1m"
    #define GRIFFIN_COLOR_YELLOW    "\x1b[38;2;255;255;1;1m"
    #define GRIFFIN_COLOR_BLACK_RED "\x1b[38;2;0;0;0;48;2;255;0;0;1;1m"
    #define GRIFFIN_COLOR_RESET     "\x1b[0m"

#endif // GRIFFIN_LOG_WIN32

namespace grflog
{
    namespace utils
    {
        /// Create a directory if not exists.
        /// @param dirPath Path to create the new  directory.
        void make_directory(const std::string& dirPath);

        /// Get the current local date time.
        const std::string get_date_time();

        /**
         * Utility function to format a C++ string given a C-printf-style format and its va_list
         * @param fmt Format string with placeholders to replace values.
         * @param vaArgs Arguments list.
         * @returns A formatted std::string.
         */
        std::string fmt_str(const std::string& fmt, va_list vaArgs);
    }

    enum class log_level : uint32_t
    {
        INFO        =       0,
        DEBUG       =       1,
        WARN        =       2,
        CRITICAL    =       3,
        FATAL       =       4
    };

    namespace visual
    {
        /// Function to get the correspondent string for the log level.
        /// @param lvl log level to be used.
        const std::string get_log_lvl_str(const log_level& lvl);


        /// Function to get the correspondent color for the log level.
        /// @param lvl log level to be used.
        const GRIFFIN_COLOR get_log_lvl_color(const log_level& lvl);


        /// Function to set the console's text's color. Platform specific.
        void set_text_color(const log_level& lvl);

        
        /// Function to reset the console's text's color. Platform specific.
        void reset_text_color();
    }

    struct log_event
    {
        const std::string date_time;

        const log_level lvl;
        const std::string log_lvl_str;

        const std::string content;

        /// Log event constructor, get every needed information for a log event.
        /// @param dt Date Time string formatted as: YYYY-mm-dd H:M:S (see utils::get_date_time()).
        /// @param llvl Log Level of this log event.
        /// @param fmt_what "Formatted What" formatted message to log in this event.
        log_event(const log_level& llvl, const std::string& msg)
            : date_time(std::move(utils::get_date_time())), 
              lvl(llvl), 
              log_lvl_str(visual::get_log_lvl_str(llvl)), 
              content(msg) 
              {} 
    };

    // Logging functions

    /// Main logging function, will create a log_event struct object with the needed information and 
    /// call console_log() and file_log() (if file was added) passing the created log_event.
    /// @param lvl The log level to use. Enumerated in enum log_level.
    /// @param what The message to be logged.
    /// @param vaArgs Variardic Args to format the log message.
    void log(log_level lvl, const std::string& what, va_list vaArgs);


    /// Console logging function, will be called from log().
    /// @param l_ev Log event struct to be used for logging the information in the console.
    void console_log(const log_event& l_ev);


    // File logging functions and class

    class file_logger
    {
    public:
        /// Default constructor, only initialize the file's name and path.
        file_logger();

        /// Construct file logger with its file's name.
        /// @param m_file_name indicate the file's name to be created and to write.
        file_logger(const std::string& file_name);

        /// Copy constructor.
        /// @param other file_logger object to copy from.
        file_logger(const file_logger& other);

        file_logger& operator=(const file_logger& other)
        {
            this->copy_from(other);
            return *this;
        }

        /// Copy file_logger object other to this
        /// @param other file_logger to be copied.
        void copy_from(const file_logger& other);

        // Check if file logging has been initialized.
        /// @returns true if it was initialized, else return false.
        bool is_initialized();

        /// Initialize the file logging, will be called on add_file_log(). 
        /// If the file's name wasn't provided, it will open (or create) a file called by default "grflog_file.log"
        /// @returns true if successfully opened and initialized the file, otherwise return false if couldn't open the file.
        bool init_file_logging();

        /// Write any string to the file, get called on log() with every information in a log_event struct.
        /// @param what string message to be written.
        void write_to_file(const std::string& what);

        /// Get the file's name.
        const std::string get_file_name();

        /// Set the file's name if the file's name is empty
        /// @param file_name File name to set
        void set_file_name(const std::string& file_name);

        /// Finish the file logging, close and flush the std::ofstream file.
        void finish_file_logging();

        ~file_logger();

    private:
        std::string m_file_name;
        std::string m_file_path;
        std::ofstream m_file;
    };


    /// Set the file_logger object to be used for file logging, by default grflog only logs to console.
    /// If a file was previously openned, it will be finished and will open the new one.
    /// @param file file_logger object to be used. If file's name is empty, throw an exception.
    void set_file_logger(const file_logger& file);

    /// Stop current file from logging if there is one.
    void stop_file_logging();

    /// File logging function, will be called from log().
    /// @param l_ev Log event struct to be used for logging the information into a file.
    void file_log(const log_event& l_ev);


    // Level implemented logging functions
    
    /// Info logging function, simply calls log() with log_level::INFO
    /// @param what The INFO message to be logged.
    void info(const std::string& what, ...);
    

    /// Debug logging function, simply calls log() with log_level::DEBUG
    /// @param what The DEBUG message to be logged.
    void debug(const std::string& what, ...);


    /// Warn logging function, simply calls log() with log_level::WARN
    /// @param what The WARN message to be logged.
    void warn(const std::string& what, ...);


    /// Critical logging function, simply calls log() with log_level::CRITICAL
    /// @param what The CRITICAL message to be logged.
    void critical(const std::string& what, ...);


    /// Fatal logging function, simply calls log() with log_level::FATAL
    /// @param what The FATAL message to be logged.
    void fatal(const std::string& what, ...);
}

#endif // GRIFFIN_LOG_H

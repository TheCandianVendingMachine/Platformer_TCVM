// logger.hpp
// the logger class. Mostly static functions, with the ablility to call different types of loggers
// Make sure to initialize all loggers at start of program. Do not log an event then create a new log. It will crash program
#pragma once

#include <fstream>
#include <string>
#include <map>

class console;

namespace logr
    {
        class logger
            {
                private:
                    console *_console;

                    std::string _loggerName;
                    std::string _loggerFileName;
                    std::string _loggerFilePath;

                    std::ofstream _logWriter;

                    /* all loggers made. Name is string, logger is actual logger */
                    std::map<std::string, logger> _loggers;

                    /* add the '.log' file extension to text*/
                    void addFileExtension(std::string *text);
                    /* add the '.log' file extension to some text, and append some extra text if wanted */
                    void addFileExtension(std::string *text, const std::string &extraText);

                    std::string getTime();

                public:
                    logger() = default;
                    ~logger();

                    /* set where the files will be saved to (different folders) */
                    void setLogFilepath(const std::string& path);
            
                    /* Create new logger. */
                    logger(const std::string &loggerName, const std::string &loggerFileName, bool withTime);

                    logger(const logger&) = delete;
                    logger & operator=(const logger& other);

                    /* logs a message to the current log */
                    void log(const std::string &out);

                    /* Set the console which will be logged to */
                    void setConsole(console *cnsl);
                    /* log a message to console. Will not write to any file */
                    void logToConsole(const std::string &out);

                    /* clears the current log */
                    void clear();

                    /* check if logger exists */
                    bool doesLoggerExist(const std::string &logName);
            };
    }

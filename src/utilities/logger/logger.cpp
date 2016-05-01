#include "logger.hpp"
#include "../game/console.hpp"

#include <iostream>

#include <chrono>
#include <ctime>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <assert.h>

namespace logr
    {
        void logger::addFileExtension(std::string *text)
            {
                *text += ".log";
            }

        void logger::addFileExtension(std::string *text, const std::string &extraText)
            {
                *text += extraText;
                addFileExtension(text);
            }

        std::string logger::getTime()
            {
                // get current date
                time_t t = time(NULL);
                struct tm ts;
                char szBuffer[1000] = "";

                auto err = localtime_s(&ts, &t);

                if (!err)
                    {
                    // Format the time
                        strftime(szBuffer, sizeof(szBuffer), "%Y_%m_%d_%H-%M-%S", &ts);
                    }

                std::string time = szBuffer;
                return time;
            }

        logger::~logger()
            {

            }

        void logger::setLogFilepath(const std::string &path)
            {
                _loggerFilePath = path;
                addFileExtension(&_loggerFilePath);
            }

        logger::logger(const std::string &loggerName, const std::string &loggerFileName, bool withTime)
            {
                _loggerName = loggerName;

                _loggerFileName = _loggerFilePath + loggerFileName;

                if (withTime)
                    {
                        addFileExtension(&_loggerFileName, "-" + getTime());
                    }
                else
                    {
                        addFileExtension(&_loggerFileName);
                    }

                _console = nullptr;
            }

        logger & logger::operator=(const logger &other)
            {
                _loggerName = other._loggerName;
                _loggerFileName = other._loggerFileName;
                _loggerFilePath = other._loggerFilePath;
                return *this;
            }

        void logger::log(const std::string &out)
            {
                // make sure the logger is closed
                if (_logWriter.is_open())
                    {
                        _logWriter.close();
                    }
        
                if (_loggerFileName.empty())
                    {
                        _loggerFileName = "NULL_LOG_NAME";
                        addFileExtension(&_loggerFileName);
                        logToConsole("Log name has not been created");
                    }

                // and then log the wanted output
                _logWriter.open(_loggerFileName, std::ios::app);
                if (_logWriter.good())
                    {
                        _logWriter << getTime() << ": " << out.c_str() << std::endl;
                    #ifdef _DEBUG
                        logToConsole(getTime() + ": " + out.c_str());
                    #endif // DEBUG
                    }
                else
                    {
                        logToConsole(_loggerFileName + " cannot be opened");
                    }
                _logWriter.close();
            }

        void logger::setConsole(console *cnsl)
            {
                _console = cnsl;
            }

        void logger::logToConsole(const std::string &out)
            {
                std::string message = getTime() + ": " + out.c_str();
                #if _DEBUG
                    std::cout << "<CNSL> " + message + "\n";
                #endif

                if (_console)
                    {
                        _console->log(message);
                    }
            }

        void logger::clear()
            {
                if (_logWriter.is_open())
                    {
                        _logWriter.close();
                    }

                _logWriter.open(_loggerFileName, std::ios::trunc);
                _logWriter.close();
            }

        bool logger::doesLoggerExist(const std::string &logName)
            {
                return _loggers.find(logName) != _loggers.end();
            }
    }
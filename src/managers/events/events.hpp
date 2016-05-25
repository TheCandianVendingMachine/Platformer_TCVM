// events.hpp
// all possible events to be passed to the eventManager
#pragma once

#include <string>

namespace nonGlobal
    {
        union data
            {
                int intDat;
                float floatDat;
                double doubleDat;
                const char *stringDat;
            };

        enum dataTypes
            {
                NONE,
                INTEGER,
                FLOAT,
                DOUBLE,
                STRING
            };
    }

enum events
    {
        RELOAD_ENTITY_LIST,
        LOAD_ENTITY_LIST,
        NEXT_LEVEL,
        PLAYER_LOSE_HEALTH,
    };

struct eventData
    {
        // data to be passed if any data needs to be
        nonGlobal::data _data;
        // what type of data would be passed through
        nonGlobal::dataTypes _dataType;

        // what event will be passed through
        events _event;

        // event ID
        std::string _eventID;

        eventData(events event, const std::string &id = "") { _dataType = nonGlobal::NONE; _event = event; _eventID = id; }
        eventData(int data, events event, const std::string &id = "") { _data.intDat = data, _dataType = nonGlobal::INTEGER; _event = event; _eventID = id; }
        eventData(float data, events event, const std::string &id = "") { _data.floatDat = data, _dataType = nonGlobal::DOUBLE;_event = event; _eventID = id; }
        eventData(double data, events event, const std::string &id = "") { _data.doubleDat = data, _dataType = nonGlobal::FLOAT;_event = event; _eventID = id; }
        eventData(const char *data, events event, const std::string &id = "") { _data.stringDat = data, _dataType = nonGlobal::STRING; _event = event; _eventID = id; }

    };
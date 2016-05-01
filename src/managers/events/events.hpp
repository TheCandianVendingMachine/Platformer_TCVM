// events.hpp
// all possible events to be passed to the eventManager
#pragma once

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
    };

struct eventData
    {
        // data to be passed if any data needs to be
        nonGlobal::data _data;
        // what type of data would be passed through
        nonGlobal::dataTypes _dataType;

        // what event will be passed through
        events _event;

        eventData(events event) { _dataType = nonGlobal::NONE; _event = event; }
        eventData(int data, events event) { _data.intDat = data, _dataType = nonGlobal::INTEGER; _event = event; }
        eventData(float data, events event) { _data.floatDat = data, _dataType = nonGlobal::DOUBLE;_event = event; }
        eventData(double data, events event) { _data.doubleDat = data, _dataType = nonGlobal::FLOAT;_event = event; }
        eventData(const char *data, events event) { _data.stringDat = data, _dataType = nonGlobal::STRING; _event = event; }

    };
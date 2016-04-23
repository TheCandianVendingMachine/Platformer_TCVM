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
            };

        enum dataTypes
            {
                INTEGER,
                FLOAT,
                DOUBLE,
            };
    }

enum events
    {

    };

struct eventData
    {
        // data to be passed if any data needs to be
        nonGlobal::data _data;
        // what type of data would be passed through
        nonGlobal::dataTypes _dataType;

        // what event will be passed through
        events _event;

        eventData(int data, events event) { _data.intDat = data, _dataType = nonGlobal::INTEGER; _event = event; }
        eventData(float data, events event) { _data.floatDat = data, _dataType = nonGlobal::DOUBLE;_event = event; }
        eventData(double data, events event) { _data.doubleDat = data, _dataType = nonGlobal::FLOAT;_event = event; }

    };
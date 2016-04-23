#include "state.hpp"

const states state::getState() const
    {
        return _state;
    }

const bool state::renderOvertop() const
    {
        return _renderOvertop;
    }

const bool state::updateUnderneath() const
    {
        return _updateUnderneath;
    }

const bool state::isInitialized() const
    {
        return _isInitialized;
    }

state::~state()
    {}

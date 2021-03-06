#include "../game/globals.hpp"
#include <assert.h>
#include <exception>
#include "resourceManager.hpp"

template<typename T>
T *resourceManager<T>::get(const std::string &resourceName, bool getDefaultTexture)
    {
        auto it = _resources.find(resourceName);
        if (it != _resources.end())
            {
                return it->second.get();
            }
        else if (getDefaultTexture)
            {
                if (!_defaultResource.empty())
                    {
                        globals::_logger->log("Default texture has to be used. \"" + resourceName + "\" not loaded");
                        return get(_defaultResource, false);
                    }
            }
      
        globals::_logger->logToConsole("Cannot load \"" + resourceName + "\" from resource pool");
        return nullptr;
    }

template<typename T>
T *resourceManager<T>::add(const std::string &filepath, const std::string &resourceName)
    {
        T *newResource = get(resourceName, false);
        if (newResource)
            {
                return newResource;
            }

        newResource = new T;
        if (newResource->loadFromFile(filepath))
            {
                _resources[resourceName] = std::make_shared<T>(*newResource);
                return _resources[resourceName].get();
            }
        
        globals::_logger->log("Cannot add \"" + resourceName + "\" to the resource pool");
        return nullptr;
    }

template<typename T>
void resourceManager<T>::remove(const std::string &resourceName)
    {
        _resources.erase(_resources.find(resourceName));
    }

template<typename T>
void resourceManager<T>::removeAll()
    {
        _resources.clear();
    }

template<typename T>
void resourceManager<T>::setDefaultResource(const std::string &resourceName)
    {
        _defaultResource = resourceName;
    }

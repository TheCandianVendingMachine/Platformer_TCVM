// resourceManager.hpp
// manages all resource assets and makes sure there is only ever 1 pointer to them at a time
#pragma once

#include <unordered_map>
#include <string>
#include <memory>

template<typename T>
class resourceManager
    {
        private:
            std::unordered_map<std::string, std::shared_ptr<T>> _resources;
            std::string _defaultResource;
            
        public:
            /* get a texture in the resource map. Returns a pointer to it if it exists, otherwise a nullptr will be returned */
            T *get(const std::string &resourceName, bool getDefaultTexture);
            /* add a new resource to the map. If one exists or it succesfully adds it, it will return a pointer to the resource. Otherwise a nullptr is returned */
            T *add(const std::string &filepath, const std::string& resourceName);

            /* set the default resource that will be used if a texture isnt avaliable */
            void setDefaultResource(const std::string &resourceName);

    };

#include "resourceManager.inl"
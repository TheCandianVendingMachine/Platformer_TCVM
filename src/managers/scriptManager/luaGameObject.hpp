// luaGameObject.hpp
// handles the passing in and out of game objects to Lua safely.
#pragma once

class gameObject;

class luaGameObject
    {
        private:
            gameObject *_obj;

        public:
            luaGameObject() = default;
            luaGameObject(gameObject *obj);

            void setImpulse(float X, float Y);
            float getImpulseX();
            float getImpulseY();

			void setPos(float X, float Y);
			float getPosX();
			float getPosY();

			void setSize(float X, float Y);
			float getSizeX();
			float getSizeY();

			float getDeltaTime();

            void test();

    };
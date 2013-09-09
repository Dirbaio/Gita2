#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"
#include "Utils.hpp"

class SceneMain;
class GameObject { //Static objects that have a position and rotation but don't move
	public:
        GameObject(SceneMain* scene);
        GameObject(SceneMain* scene, const vec3f &pos, const vec3f &scale);
        virtual ~GameObject();

		virtual void update(float deltaTime);
		virtual void draw() const;

		bool isAlive;
		vec3f pos;
		vec3f scale;
	protected:
        SceneMain* scene;
};

#endif // GAMEOBJECT_HPP

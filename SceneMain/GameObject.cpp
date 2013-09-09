#include "GameObject.hpp"
#include "SceneMain.hpp"

GameObject::GameObject(SceneMain* scene, const vec3f &pos, const vec3f &scale) :
	isAlive(true), pos(pos), scale(scale),
    scene(scene) {
}

GameObject::GameObject(SceneMain* scene) :
    isAlive(true), pos(0.0), scale(1.0), scene(scene) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
	(void) deltaTime;
}

void GameObject::draw() const {
}

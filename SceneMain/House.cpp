#include "House.hpp"
#include "SceneMain.hpp"
#include "../Game.hpp"

House::House(SceneMain* parentScene, int x, int y, int type) : GameObject(parentScene) {
	pos = vec3f(x, 0, y);
	tri.mesh = new Mesh("data/models/house.obj");
	tri.program = parentScene->shaderHouse;
}

House::~House() {
	delete tri.mesh;
}

void House::update(float deltaTime) {
	(void) deltaTime;
	updateMatrix();
}

void House::updateMatrix() {
	mat4f m(1.0);
	m = glm::translate(m,pos);
	m = glm::scale(m,scale);
	tri.modelMatrix = m;
}

void House::draw() const {
	mat4f transform = scene->getState().projection*scene->getState().view*tri.modelMatrix;
	tri.program->uniform("modelViewProjectionMatrix")->set(transform);
	tri.draw();
}

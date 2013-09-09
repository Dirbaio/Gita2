#ifndef HOUSE_HPP
#define HOUSE_HPP
#include "GameObject.hpp"
#include "../graphics/Model.hpp"

class House : public GameObject {
	public:
		House(SceneMain* parentScene, ShaderProgram* program, vec3f pos, vec3f scale);
		virtual ~House();

		void update(float deltaTime);
		void updateMatrix();
		void draw() const;
	private:
		Model tri;
};

#endif // HOUSE_HPP

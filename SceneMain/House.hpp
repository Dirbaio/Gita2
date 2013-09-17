#ifndef HOUSE_HPP
#define HOUSE_HPP
#include "GameObject.hpp"
#include "../graphics/Model.hpp"

class House : public GameObject {
	public:
		House(SceneMain* parentScene, int x, int y, int type);
		virtual ~House();

		void update(float deltaTime);
		void updateMatrix();
		void draw() const;
	private:
		Model tri;
};

#endif // HOUSE_HPP

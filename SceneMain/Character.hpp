#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.hpp"
#include <queue>
#include "../graphics/Model.hpp"
#include "Animation.hpp"

using namespace std;


enum FaceDir {
	FACE_UP = 0, FACE_DOWN, FACE_LEFT, FACE_RIGHT, FACE_SIZE
};

class Character : public GameObject
{
	public:
	Character(SceneMain* sc);
	virtual ~Character() {}

	void move(vec2f posf);
	bool canSee(const vec2f& pos);
	void update(float deltaTime);
	void draw() const;
	virtual vec2f moveCharacter(float deltaTime);
	vec2f getPosition() const { return position; }

	protected:

	string currAnim;
	void ensureAnim(std::string name);

	virtual void noLeftCollision() {}
	virtual void noRightCollision(){}
	virtual void noUpCollision()   {}
	virtual void noDownCollision() {}

	virtual bool onLeftCollision(int x, int j);
	virtual bool onRightCollision(int x, int j);
	virtual bool onUpCollision(int x, int j);
	virtual bool onDownCollision(int x, int j);

	Animation anim;
	string texName;
	string action;

	FaceDir faceDir;
	vec2f position;

	float vel;
	float deadrot;
	bool drawDead;
	void moveInDir(vec2f dir, float deltaTime);
};

#endif // CHARACTER_H

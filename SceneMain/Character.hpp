#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameObject.hpp"
#include <queue>
#include "../graphics/Model.hpp"

using namespace std;

enum CharacterMark {
    MARK_NONE,
    MARK_EXCLAMATION,
    MARK_RED_EXCLAMATION,
    MARK_BLUE_QUESTION,
    MARK_QUESTION
};

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

protected:


	virtual void noLeftCollision() {}
	virtual void noRightCollision(){}
	virtual void noUpCollision()   {}
	virtual void noDownCollision() {}

    virtual bool onLeftCollision(int x, int j);
    virtual bool onRightCollision(int x, int j);
    virtual bool onUpCollision(int x, int j);
    virtual bool onDownCollision(int x, int j);

    FaceDir faceDir;
    vec2f position;
    float vel;

    queue<vec2f> path;
    vec2f goal;
    bool hasGoal;

    CharacterMark mark;
    void drawMark();

    void setGoal(vec2f goal);
    vec2f dirTowardsGoal();
    void moveInDir(vec2f dir, float deltaTime);

    Model model;
};

#endif // CHARACTER_H

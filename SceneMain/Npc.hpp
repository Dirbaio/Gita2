#ifndef NPC_HPP
#define NPC_HPP

#include "Character.hpp"

class Npc : public Character
{
public:

    enum CharacterMark {
        MARK_NONE,
        MARK_EXCLAMATION,
        MARK_RED_EXCLAMATION,
        MARK_BLUE_QUESTION,
        MARK_QUESTION
    };

    Npc(SceneMain* scene);

protected:
    CharacterMark mark;

    void setGoal(vec2f goal);
    vec2f dirTowardsGoal();

    queue<vec2f> path;
    vec2f goal;
    bool hasGoal;

};

#endif // NPC_HPP

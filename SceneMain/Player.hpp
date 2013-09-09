#ifndef PLAYER_H
#define PLAYER_H

#include "Character.hpp"
#include "InputEngine.hpp"

class Player : public Character {
public:

    Player(SceneMain* sc);

    virtual vec2f moveCharacter(float deltaTime);
    void hitAction();
    void ensureAnim(string s) {}
    void gotCaught();

    int getMoney(){return myMoney;}
    void setMoney(int sc) {myMoney = sc;}

    int getKills() {return myKills;}
    void setKills(int k) {myKills = k;}

    bool isDoingAction() { return actionDelay > 0; }

    InputEng playerInput;

    bool jailed;

    std::string currentAnim;
    float actionDelay;

    int myMoney;
    int myKills;

    float jailedTime;
};

#endif // PLAYER_H

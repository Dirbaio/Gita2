#ifndef PERSON_H
#define PERSON_H

#include "Npc.hpp"

#include <SFML/Audio.hpp>
#include <list>

class Person : public Npc {
public:
    Person(SceneMain* sc) : Npc(sc) {}

	void Init();
    void Update();

	void doDeath();
	void onHit();
	bool is_alive();

	enum State {
		STATE_WALKING,
		STATE_PANIC,
		STATE_DEAD,
		STATE_CONFUSED
	};

    int getState() { return state; }
    bool knowsPlayer(int i);

private:
    float getClosestMenace(vec2f pos, vec2f& menacePos);
	void lookAtRandomPlace();

	int ix, iy;
    vector<bool> knowsPlayers;
    vector<vec2f> lastSawPlayer;
    vector<float> playerActionTime;

    float dissappearTime;
    float deathTimer;
    float panicTime, startPanicTime;
    vec2f panicSource;
    float walkingTime;
    float confuseCooldown;
    float confusedTime;
    float confusedTimeFacing;

    State state;

    //sf::SoundBuffer dieSoundBuff;
    //sf::Sound dieSound;
};

#endif // PERSON_H

#ifndef POLICE_H
#define POLICE_H

#include "Npc.hpp"

class Police : public Npc {
	public:
		Police(SceneMain* sc);

		vec2f moveCharacter(float delta);

	private:

		vec2f getNewGoal(vec2f pos);
		void lookAtRandomPlace();

		bool onLeftCollision(int x, int j);
		bool onRightCollision(int x, int j);
		bool onUpCollision(int x, int j);
		bool onDownCollision(int x, int j);

		enum State  {
			STATE_PATROL_MOVING,
			STATE_PATROL_WATCHING,
			STATE_ALERT,
			STATE_CONFUSE,
			STATE_CHASING_PLAYER,
			STATE_PLAYER_LOST
		};

		State state;
		float watchingTime;
		float watchingTimeFacing;
		float alertTime;
		vec2f lastAlertPos;

		vector<float> lastPosSawTime;
		vector<vec2f> lastPosSawPlayer;
		vector<vec2f> lastDirSawPlayer;
		vector<bool> knowPlayer;
		int chasingPlayerNum;
		bool collided;
};

#endif // POLICE_H

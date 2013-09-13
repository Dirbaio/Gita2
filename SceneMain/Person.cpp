#include "Person.hpp"
#include "Npc.hpp"
#include "SceneMain.hpp"
#include "Animation.hpp"
#include "Player.hpp"

#define NUANIMS_DATA 6
AnimationData* s_person_data[NUANIMS_DATA] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

const char* s_person_dataFilenames[NUANIMS_DATA] = {
	"data/anim/calvo.anim",
	"data/anim/tupe.anim",
	"data/anim/gordo.anim",
	"data/anim/rubiaca.anim",
	"data/anim/morenaca.anim",
	"data/anim/moderno.anim"
};

Person::Person(SceneMain* sc) : Npc(sc) {
	dissappearTime = 30.0f;
	walkingTime = 0.0f;

	startPanicTime = 10.0f;
	state = STATE_WALKING;

	knowsPlayers = vector<bool>(scene->players.size(), false);
	lastSawPlayer = vector<vec2f>(scene->players.size());
	playerActionTime = vector<float>(scene->players.size(), 10000);

	confuseCooldown = 0.0f;

	int rand = Utils::randomInt(0, NUANIMS_DATA-1);
	if (s_person_data[rand] == NULL) {
		s_person_data[rand] = new AnimationData();
		s_person_data[rand]->Load(s_person_dataFilenames[rand]);
	}
	anim.setAnimData(s_person_data[rand]);

	//dieSoundBuff.loadFromFile("audio/wilhelmscream.ogg");
	//dieSound.setBuffer(dieSoundBuff);
	//dieSound.setLoop(false);
	//dieSound.setPitch(1.5f);
	//dieSound.setVolume(10000.0f);

	velMult = Utils::randomFloat(0.7, 1.5);
	if(Utils::randomBool(5))
		velMult = 4;

	ix = Utils::randomFloat(0.4, 0.6);
	iy = Utils::randomFloat(0.4, 0.6);

	position = vec2f(sc->map->getRandomStreet())+0.5f;
}


float Person::getClosestMenace(vec2f pos, vec2f& menacePos)
{
	int menaceCount = 1;
	menacePos.x = 0;
	menacePos.y = 0;
	menacePos += panicSource;

	/*

	//TODO FIX THIS SHIT
	for(int i = 0; i < (int)scene->players.size(); i++)
	{
		if(scene->players[i]->jailed) continue;
		if(knowsPlayers[i] || playerActionTime[i] < startPanicTime)
		{
			menacePos = lastSawPlayer[i]*6.0f;
			menaceCount += 6;
		}
	}
	vector<Person*> v = scene->getPeopleSeen(this, SceneMain::SEARCH_DEAD);
	for(int i = 0; i < (int) v.size(); i++) {
		menacePos += v[i]->getPosition();
		menaceCount++;
	}
*/


	menacePos /= float(menaceCount);
	return glm::distance(pos, menacePos);
}

static vec2i dirInc[] = {
	vec2i(0,-1),
	vec2i(0, 1),
	vec2i(-1,0),
	vec2i( 1,0)
};

vec2f Person::moveCharacter(float delta) {

	//Player* p = &scene->players[0];

	for(int i = 0; i < (int)scene->players.size(); i++)
	{
		if(scene->players[i]->jailed) continue;
		playerActionTime[i] += delta;
		vec2f currPlayerPosition = scene->players[i]->getPosition();
		bool seesPlayerNow = canSee(currPlayerPosition);
		if(seesPlayerNow)
		{
			if(scene->players[i]->isDoingAction())
				playerActionTime[i] = 0;
			lastSawPlayer[i] = currPlayerPosition;
		}
	}

	switch(state)
	{
		case STATE_WALKING:
		{
			mark = MARK_NONE;
			vel = 2.0*velMult;
			if(!hasGoal)
				setGoal(vec2f(scene->map->getRandomStreet()));

			for(int i = 0; i < (int)scene->players.size(); i++)
			{
				if(scene->players[i]->jailed) continue;
				if ((knowsPlayers[i] || scene->players[i]->isDoingAction()) && canSee(scene->players[i]->getPosition()))
				{
					state = STATE_PANIC;
					panicTime = startPanicTime;
					panicSource = scene->players[i]->getPosition();
				}
			}

			vector<Person*> v = scene->getPeopleSeen(this, SceneMain::SEARCH_DEAD);
			for(int j = 0; j < (int) v.size(); j++)
			{
				state = STATE_PANIC;
				panicTime = startPanicTime;
				panicSource = v[j]->getPosition();

				for(int i = 0; i < (int)scene->players.size(); i++)
				{
					if(scene->players[i]->jailed) continue;
					if (glm::distance(v[j]->position, lastSawPlayer[i]) < 4) knowsPlayers[i] = true;
				}
			}

			v = scene->getPeopleSeen(this, SceneMain::SEARCH_PANIC);

			if (confuseCooldown <= 0.0f) {
				for(int i = 0; i < (int) v.size(); i++)
				{
					state              = STATE_CONFUSED;
					confusedTime       = (float) Utils::randomInt(2,4);
					confusedTimeFacing = (float) Utils::randomInt(1, 3)/4.0;
					confuseCooldown    = (float) Utils::randomInt(6,12);
				}
			}
			else
				confuseCooldown -= delta;


			return dirTowardsGoal();
		}
		case STATE_PANIC:
		{
			vel = 3.5*velMult;
			if (panicTime > 0) panicTime -= delta;
			else {
				state = STATE_WALKING;
				hasGoal = false;
			}

			mark = MARK_EXCLAMATION;

			for(int i = 0; i < (int)scene->players.size(); i++)
			{
				if(scene->players[i]->jailed) continue;

				if (knowsPlayers[i] && canSee(scene->players[i]->getPosition()))
					panicTime = startPanicTime;
			}

			vec2i now = vec2i(position);
			vec2i best = now;
			vec2f menacePos;
			float bestd = getClosestMenace(position, menacePos);

			if(bestd < 3)
				return(vec2f(position - menacePos));
			else
			{
				for(int i = 0; i < 4; i++)
				{
					vec2i lol = now + dirInc[i];
					if(scene->map->tile(lol.x, lol.y).isSolid()) continue;

					float d = getClosestMenace(vec2f(lol.x + 0.5f, lol.y + 0.5f), menacePos);
					if(d > bestd) {
						bestd = d;
						best = lol;
					}
				}

				vec2f togo = vec2f(best.x+ix, best.y+iy);
				vec2f dir  = togo - position;
				return dir;
			}
			break;
		}
		case STATE_CONFUSED: {
			mark = MARK_BLUE_QUESTION;
			vel = 1.0*velMult;

			confusedTime -= delta;
			confusedTimeFacing -= delta;

			if (confusedTime < 0) {
				setGoal(vec2f(scene->map->getRandomStreet())+0.5f);
				state = STATE_WALKING;
			}


			vector<Person*> v = scene->getPeopleSeen(this, SceneMain::SEARCH_DEAD);
			for(int j = 0; j < v.size(); j++) {
				state = STATE_PANIC;
				panicTime = startPanicTime;
				panicSource = v[j]->getPosition();
				for(int i = 0; i < (int)scene->players.size(); i++)
					if (glm::distance(v[j]->position, lastSawPlayer[i]) < 3) knowsPlayers[i] = true;
			}


			for(int i = 0; i < (int)scene->players.size(); i++)
			{
				if(scene->players[i]->jailed) continue;
				if ((knowsPlayers[i] || scene->players[i]->isDoingAction()) && canSee(scene->players[i]->getPosition()))
				{
					state = STATE_PANIC;
					panicTime = startPanicTime;
					panicSource = scene->players[i]->getPosition();
				}
			}

			if (confusedTimeFacing < 0) {
				//FIXME
				confusedTimeFacing = Utils::randomInt(1, 3)/4.0f;
				return vec2f(Utils::randomFloat(-1, 1), Utils::randomFloat(-1, 1));
			}
			else
				return dirTowardsGoal();

			break;
		}
		case STATE_DEAD: {
			mark = MARK_NONE;
			deathTimer -= delta;
			if (deathTimer < 0) isAlive = false;

			return vec2f(0, 0);
			break;
		}
		default:
			return vec2f(0, 0);
	}

}

void Person::doDeath() {

	deathTimer = dissappearTime;
	state = STATE_DEAD;

}

void Person::onHit() {
	//dieSound.play();
	doDeath();
}
/*

void Person::lookAtRandomPlace() {
	sf::Vector2i v = scene->city.absoluteToTilePos(position);

	int lastFaceDir = faceDir;
	int i = 0;
	while(i < 8) {
		faceDir = Utils::randomInt(FACE_UP, FACE_RIGHT);
		sf::Vector2i v2 = v + dirInc[faceDir];
		if(!scene->city.occupedIJ(v2.x, v2.y) && faceDir != lastFaceDir) break;
		i++;
	}
}
*/
bool Person::is_alive() {
	return state != STATE_DEAD;
}

bool Person::knowsPlayer(int i)
{
	return knowsPlayers[i];
}

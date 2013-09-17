#include "Player.hpp"
#include "Person.hpp"
#include "SceneMain.hpp"

Player::Player(SceneMain* sc) : Character(sc)
{
    pos.x = 0;
    pos.y = 0;

    actionDelay = 0;

    myMoney = 0;
    myKills = 0;

    jailed = false;
	vel = 5;

	AnimationData* data = new AnimationData();
	data->Load("data/anim/takena.anim");
	anim.setAnimData(data);

	texName = "player";

	position = vec2f(sc->map->getRandomStreet())+0.5f;
}

void Player::hitAction()
{
    actionDelay = 0.18f;
}

vec2f Player::moveCharacter(float delta)
{

	drawDead = jailed;
    if(jailed)
    {
        jailedTime += delta;
        return vec2f(0, 0);
    }

    playerInput.Update();


    if (playerInput.getKeyDown(InputEng::PLAYER_ACTION)) {

        hitAction();
		std::vector<Person*> persons = scene->getPeopleAround(getPosition(), 1, SceneMain::SEARCH_ANY);
        for (std::vector<Person*>::iterator it = persons.begin(); it != persons.end(); ++it) {
            if (!(*it)->is_alive()) continue;

            (*it)->onHit();
            int n_moneys = Utils::randomInt(1, 3);
			//for (int i = 0; i < n_moneys; ++i) scene->spawnNewMoney((*it)->getPosition());
        }
    }


//    anim->Update(delta);

    /*
    for (std::list<Item>::iterator it = scene->itemList.begin(); it != scene->itemList.end(); ++it) {
        if (it->isTakeable()) {

            sf::FloatRect moneyBox = this->getBoundBox();
            moneyBox.left -= moneyBox.width/6;
            moneyBox.width *= 3;
            moneyBox.top -= moneyBox.height/6;
            moneyBox.height *= 3;

            if (Utils::rectCollision(it->getBoundBox(), moneyBox)) {
                it->takeAction();
                myMoney += it->getValue();
            }
        }
    }*/

    if (actionDelay <= 0)
    {
        vec2f dir (0, 0);

        if (playerInput.getKeyState(InputEng::PLAYER_UP) && !playerInput.getKeyState(InputEng::PLAYER_DOWN))
            dir.y = -1;
        if (playerInput.getKeyState(InputEng::PLAYER_DOWN) && !playerInput.getKeyState(InputEng::PLAYER_UP))
            dir.y = 1;
        if (playerInput.getKeyState(InputEng::PLAYER_LEFT) && !playerInput.getKeyState(InputEng::PLAYER_RIGHT))
            dir.x = -1;
        if (playerInput.getKeyState(InputEng::PLAYER_RIGHT) && !playerInput.getKeyState(InputEng::PLAYER_LEFT))
            dir.x = 1;

		action = "Idle";
		return dir*vel*delta;
    }
    else
    {
        actionDelay -= delta;
		action = "Attack";
        return vec2f(0, 0);
    }
}

void Player::gotCaught() {
    if(jailed) return;

    jailed = true;
    jailedTime = 0;
}

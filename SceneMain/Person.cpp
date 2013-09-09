#include "Person.hpp"
#include "Npc.hpp"
#include "SceneMain.hpp"
#include "Animation.hpp"

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
    "anim/calvo.anim",
    "anim/tupe.anim",
    "anim/gordo.anim",
    "anim/rubiaca.anim",
    "anim/morenaca.anim",
    "anim/moderno.anim"
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

    int rand = 0; //Utils::randomInt(0, NUANIMS_DATA-1);
    if (s_person_data[rand] == NULL) {
        s_person_data[rand] = new AnimationData();
        s_person_data[rand]->Load(s_person_dataFilenames[rand]);
    }

    //dieSoundBuff.loadFromFile("audio/wilhelmscream.ogg");
    //dieSound.setBuffer(dieSoundBuff);
    //dieSound.setLoop(false);
    //dieSound.setPitch(1.5f);
    //dieSound.setVolume(10000.0f);

    vel = 16.0f * 1.25f * (float) Utils::randomInt(750, 2000) / 1000.0f;
    ix = Utils::randomInt(8, 56);
    iy = Utils::randomInt(8, 56);
}


float Person::getClosestMenace(vec2f pos, vec2f& menacePos)
{
    int menaceCount = 1;
    menacePos.x = 0;
    menacePos.y = 0;
    menacePos += panicSource;
    for(int i = 0; i < (int)scene->players.size(); i++)
    {
        if(scene->players[i].jailed) continue;
        if(knowsPlayer[i] || playerActionTime[i] < startPanicTime)
        {
            menacePos = lastSawPlayer[i]*6.0f;
            menaceCount += 6;
        }
    }
    vector<Person*> v = scene->getPeopleSeen(this, SEARCH_DEAD);
    for(int i = 0; i < (int) v.size(); i++) {
        menacePos += v[i]->getPosition();
        menaceCount++;
    }

    menacePos /= float(menaceCount);
    return Utils::distance(pos, menacePos);
}


void Person::Update() {
    Npc::Update();

    float delta = scene->input.getFrameTime().asSeconds();

    //Player* p = &scene->players[0];

    for(int i = 0; i < (int)scene->players.size(); i++)
    {
        if(scene->players[i].jailed) continue;
        playerActionTime[i] += delta;
        vec2f currPlayerPosition = scene->players[i].getPosition();
        bool seesPlayerNow = canSee(currPlayerPosition);
        if(seesPlayerNow)
        {
            if(scene->players[i].isDoingAction())
                playerActionTime[i] = 0;
            lastSawPlayer[i] = currPlayerPosition;
        }
    }

    switch(state)
    {
    case STATE_WALKING:
    {
        mark = MARK_NONE;

        if(!hasGoal)
            setGoal(scene->city.getRandomStreet());
        moveTowardsGoal();

        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].jailed) continue;
            if ((knowsPlayer[i] || scene->players[i].isDoingAction()) && canSee(scene->players[i].getPosition()))
            {
                state = STATE_PANIC;
                panicTime = startPanicTime;
                panicSource = scene->players[i].getPosition();
            }
        }

        vector<Person*> v = scene->getPeopleSeen(this, SEARCH_DEAD);
        for(int j = 0; j < (int) v.size(); j++)
        {
            state = STATE_PANIC;
            panicTime = startPanicTime;
            panicSource = v[j]->getPosition();

            for(int i = 0; i < (int)scene->players.size(); i++)
            {
                if(scene->players[i].jailed) continue;
                if (Utils::distance(v[j]->position, lastSawPlayer[i]) < 70) knowsPlayer[i] = true;
            }
        }

        v = scene->getPeopleSeen(this, SEARCH_PANIC);

        if (confuseCooldown <= 0.0f) {
            for(int i = 0; i < (int) v.size(); i++)
            {
                state              = STATE_CONFUSED;
                confusedTime       = (float) Utils::randomInt(2,4);
                confusedTimeFacing = (float) Utils::randomInt(1, 3)/4.0;
                confuseCooldown    = (float) Utils::randomInt(6,12);
            }
        }
        else {
            confuseCooldown -= delta;
        }

        break;
    }
    case STATE_PANIC:
    {
        if (panicTime > 0) panicTime -= delta;
        else {
            state = STATE_WALKING;
            hasGoal = false;
        }

        mark = MARK_EXCLAMATION;

        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].jailed) continue;

            if (knowsPlayer[i] && canSee(scene->players[i].getPosition()))
                panicTime = startPanicTime;
        }

        sf::Vector2i now = scene->city.absoluteToTilePos(position);
        sf::Vector2i best = now;
        vec2f menacePos;
        float bestd = getClosestMenace(position, menacePos);

        float velbak = vel;
        vel = 70;

        if(bestd < 50)
            moveInDir(vec2f(position - menacePos));
        else
        {
            for(int i = 0; i < 4; i++)
            {
                sf::Vector2i lol = now + dirInc[i];
                if(scene->city.occupedIJ(lol.x, lol.y)) continue;

                float d = getClosestMenace(vec2f(lol.x * 64.0f + 32.0f,
                                                        lol.y * 64.0f + 32.0f),
                                           menacePos);
                if(d > bestd) {
                    bestd = d;
                    best = lol;
                }
            }

            vec2f togo = vec2f(best.x*64+ix, best.y*64+iy);
            vec2f dir  = togo - position;
            if (Utils::norm(dir) != 0) moveInDir(dir);
        }
        vel = velbak;
        break;
    }
    case STATE_CONFUSED: {
        mark = MARK_BLUE_QUESTION;

        vel = 20.0f;
        confusedTime -= delta;
        confusedTimeFacing -= delta;

        if (confusedTime < 0) {
            setGoal(scene->city.getRandomStreet());
            state = STATE_WALKING;
        }

        if (confusedTimeFacing < 0) {
            lookAtRandomPlace();
            confusedTimeFacing = Utils::randomInt(1, 3)/4.0f;
        }

        vector<Person*> v = scene->getPeopleSeen(this, SEARCH_DEAD);
        for(int j = 0; j < v.size(); j++) {
            state = STATE_PANIC;
            panicTime = startPanicTime;
            panicSource = v[j]->getPosition();
            for(int i = 0; i < (int)scene->players.size(); i++)
                if (Utils::distance(v[j]->position, lastSawPlayer[i]) < 70) knowsPlayer[i] = true;
        }


        for(int i = 0; i < (int)scene->players.size(); i++)
        {
            if(scene->players[i].jailed) continue;
            if ((knowsPlayer[i] || scene->players[i].isDoingAction()) && canSee(scene->players[i].getPosition()))
            {
                state = STATE_PANIC;
                panicTime = startPanicTime;
                panicSource = scene->players[i].getPosition();
            }
        }

        break;
    }
    case STATE_DEAD: {
        mark = MARK_NONE;
        prio = -1;
        deathTimer -= scene->input.getFrameTime().asSeconds();
        if (deathTimer < 0) markForDelete();
        break;
    }
    }

    if (state == STATE_DEAD) {
        if      (faceDir == FACE_UP)    ensureAnim("DeadUp");
        else if (faceDir == FACE_DOWN)  ensureAnim("DeadDown");
        else if (faceDir == FACE_LEFT)  ensureAnim("DeadLeft");
        else if (faceDir == FACE_RIGHT) ensureAnim("DeadRight");
    }
    else if (state == STATE_CONFUSED) {
        if      (faceDir == FACE_UP)    ensureAnim("IdleUp");
        else if (faceDir == FACE_DOWN)  ensureAnim("IdleDown");
        else if (faceDir == FACE_LEFT)  ensureAnim("IdleLeft");
        else if (faceDir == FACE_RIGHT) ensureAnim("IdleRight");
    }
    else {
        if      (faceDir == FACE_UP)    ensureAnim("WalkingUp");
        else if (faceDir == FACE_DOWN)  ensureAnim("WalkingDown");
        else if (faceDir == FACE_LEFT)  ensureAnim("WalkingLeft");
        else if (faceDir == FACE_RIGHT) ensureAnim("WalkingRight");
    }
}

void Person::doDeath() {

    deathTimer = dissappearTime;
    state = STATE_DEAD;

    boundbox.left = boundbox.left - boundbox.width ;
    boundbox.width = (boundbox.width *2);
    boundbox.top = boundbox.top - boundbox.height ;
    boundbox.height = (boundbox.height *2);

}

void Person::onHit() {
    //dieSound.play();
    life--;

    if (life <= 0) doDeath();
    if (transHit != NULL) delete transHit;
    transHit = new TransitionLinear();

    transHit->setPos(scale.x);
    transHit->goPos(scale.x*1.5f);
    transHit->setTime(0.05f);
}


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

bool Person::is_alive() {
    return state != STATE_DEAD;
}

bool Person::knowsPlayer(int i)
{
    return knowsPlayer[i];
}

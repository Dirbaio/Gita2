#include "SceneMain.hpp"
#include "Game.hpp"
#include "TriangleObject.hpp"
#include "RegularPolygonObject.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "House.hpp"
#include "Person.hpp"

SceneMain::SceneMain(Game &parent) :
	Scene(parent), shaderExample(NULL),
	debugCounter(0.0), fpsCount(0) {
}

SceneMain::~SceneMain() {
}

bool SceneMain::loadResources() {
	//shaders
	ShaderProgram* s = new ShaderProgram();
	if(!s->makeProgram("data/shaders/sample.vert","data/shaders/sample.frag"))
		return false;
	shaderExample = s;

	ShaderProgram* s2 = new ShaderProgram();
	if(!s2->makeProgram("data/shaders/sample2.vert","data/shaders/sample2.frag"))
		return false;
	shaderExample2 = s2;

	ShaderProgram* s3 = new ShaderProgram();
	if(!s3->makeProgram("data/shaders/house.vert","data/shaders/house.frag"))
		return false;
	shaderHouse = s3;

	return true;
}

bool SceneMain::init() {
	std::cout << "* Loading new scene: Main" << std::endl;
	if (!loadResources())
		return false;
	//Center mouse
	//sf::Mouse::setPosition(sf::Vector2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());

	int playerCount = 1;
	players.resize(playerCount);
	for(int i = 0; i < playerCount; i++)
	{
		players[i] = new Player(this);
		addObject(players[i]);
	}

	playerNum = 0;

	addObject(map = new Map(this));
	addObject(new House(this,this->shaderHouse,vec3f(9,0,9),vec3f(2.5)));

	for(int i = 0; i < 500; i++)
	{
		addObject(new Person(this));
	}
	std::cout << "* Init was succesful" << std::endl;
	return true;
}

void SceneMain::update(float deltaTime) {

	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		std::cout << "FPS: " << fpsCount << ". Amount of GameObjects: " << objects.size() << std::endl;
		debugCounter -= 1;
		fpsCount = 0;
	}
	for(std::list<GameObject*>::iterator it = objects.begin();it != objects.end(); ++it) {
		(*it)->update(deltaTime);
	}
	//Erase dead game objects
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
		if (!(*it)->isAlive) {
			delete *it;
			it = objects.erase(it);;
		}
		else
			++it;
}

void SceneMain::draw() const {
	//calculate perspective matrix
	getState().projection = glm::perspective(FOV,float(SCRWIDTH)/float(SCRHEIGHT),ZNEAR,ZFAR);
	//Move matrix to position (according to player)
	getState().view = glm::lookAt(players[playerNum]->pos+vec3f(0, 10, 10), players[playerNum]->pos, vec3f(0, 1, 0));
	//models
	for(std::list<GameObject*>::const_iterator it = objects.begin();it != objects.end(); ++it)
		(*it)->draw();
}

void SceneMain::onKeyPressed(float deltaTime, sf::Keyboard::Key key) {
	(void) deltaTime; //unused parameter

	switch(key) {
		case sf::Keyboard::Escape:
			parent.close();
			break;
		default:
			break;
	}
}

void SceneMain::onKeyDown(float deltaTime, sf::Keyboard::Key key) {
	(void) deltaTime; //unused parameter

	switch(key) {
		default:
			break;
	}
}

void SceneMain::onKeyReleased(float deltaTime, sf::Keyboard::Key key) {
	(void) deltaTime; //unused parameter

	switch(key) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonPressed(float deltaTime, sf::Mouse::Button button) {
	(void) deltaTime; //unused parameter

	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonDown(float deltaTime, sf::Mouse::Button button) {
	(void) deltaTime; //unused parameter

	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonReleased(float deltaTime, sf::Mouse::Button button) {
	(void) deltaTime; //unused parameter

	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseMoved(float deltaTime, int dx, int dy) {
	(void) deltaTime; //unused parameter
	(void) dx; //unused parameter
	(void) dy; //unused parameter
}

void SceneMain::onClose() {
	std::cout << "* Deleting GameObjects on SceneMain" << std::endl;
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		delete *it;
	delete shaderExample;
	delete shaderExample2;
}

void SceneMain::addObject(GameObject* object) {
	objects.push_back(object);
}

std::vector<Person*> SceneMain::getPeopleAround(vec2f pos, float r, SceneMain::SearchType st)
{
	std::vector<Person*> res;

	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Person* p = dynamic_cast<Person*> (*it);
		if(p)
		{
			if( (st == SEARCH_ANY ||
				 (st == SEARCH_DEAD && !p->is_alive()) ||
				 (st == SEARCH_PANIC && p->getState() == Person::STATE_PANIC))
					&& glm::distance(pos, p->getPosition()) <= r)
				res.push_back(p);
		}
	}

	return res;
}

std::vector<Person*> SceneMain::getPeopleSeen(Character* c, SceneMain::SearchType st)
{

	std::vector<Person*> res;

	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Person* p = dynamic_cast<Person*> (*it);
		if(p)
		{
			if( (st == SEARCH_ANY ||
				 (st == SEARCH_DEAD && !p->is_alive()) ||
				 (st == SEARCH_PANIC && p->getState() == Person::STATE_PANIC))
					&& c->canSee(p->getPosition()))
				res.push_back(p);
		}
	}

	return res;
}

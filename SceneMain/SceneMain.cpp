#include "SceneMain.hpp"
#include "Game.hpp"
#include "TriangleObject.hpp"
#include "RegularPolygonObject.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "House.hpp"
#include "Person.hpp"
#include <cassert>
#include "Police.hpp"

SceneMain::SceneMain(Game &parent) :
	Scene(parent),
	debugCounter(0.0), fpsCount(0)
{
	std::cout << "* Loading new scene: Main" << std::endl;
	assert(loadResources());
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

	for(int i = 0; i < 100; i++)
		addObject(new Person(this));
	//for(int i = 0; i < 50; i++)
	//	addObject(new Police(this));

	int size = map->getWidth()*map->getHeight();
	estructuraPepinoPeople = new std::vector<Person*> [size];
	estructuraPepinoPolice = new std::vector<Police*> [size];

	std::cout << "* Init was succesful" << std::endl;
}

SceneMain::~SceneMain() {
}

bool SceneMain::loadResources() {
	//shaders
	ShaderProgram* s = new ShaderProgram();
	if(!s->makeProgram("data/shaders/color.vert","data/shaders/color.frag"))
		return false;
	shaderColor = s;

	ShaderProgram* s2 = new ShaderProgram();
	if(!s2->makeProgram("data/shaders/tex.vert","data/shaders/tex.frag"))
		return false;
	shaderTexture= s2;

	ShaderProgram* s3 = new ShaderProgram();
	if(!s3->makeProgram("data/shaders/house.vert","data/shaders/house.frag"))
		return false;
	shaderHouse = s3;

	if(!TextureManager::loadTexture("person","data/img/person_sheet.png"))
		return false;
	if(!TextureManager::loadTexture("player","data/img/player_sheet.png"))
		return false;
	if(!TextureManager::loadTexture("police","data/img/police_sheet.png"))
		return false;

	//Person model
	std::vector<Vertex::Element> elements;
	elements.push_back(Vertex::Element(Vertex::Attribute::Position , Vertex::Element::Float, 3));
	elements.push_back(Vertex::Element(Vertex::Attribute::TexCoord , Vertex::Element::Float, 2));

	Vertex::Format format(elements);
	Mesh* mesh = new Mesh(format,0,false);

	struct Vertex {
			Vertex(vec3f pos, vec2f tex) : pos(pos) , tex(tex) {}
			vec3f pos;
			vec2f tex;
	};

	std::vector<Vertex> data;
	data.push_back(Vertex(vec3f(-0.5, 0.0, 0), vec2f(0.0, 1.0)));
	data.push_back(Vertex(vec3f( 0.5, 0.0, 0), vec2f(1.0, 1.0)));
	data.push_back(Vertex(vec3f(-0.5, 1.0, -0.5), vec2f(0.0, 0.0)));
	data.push_back(Vertex(vec3f( 0.5, 0.0, 0), vec2f(1.0, 1.0)));
	data.push_back(Vertex(vec3f( 0.5, 1.0, -0.5), vec2f(1.0, 0.0)));
	data.push_back(Vertex(vec3f(-0.5, 1.0, -0.5), vec2f(0.0, 0.0)));

	mesh->setVertexData(&data[0],data.size());
	personModel.mesh = mesh;
	personModel.program = shaderTexture;

	glAlphaFunc(GL_GREATER, 0.5f);
	glEnable(GL_ALPHA_TEST);
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

	int width = map->getWidth();
	int height = map->getHeight();

	for(int i = 0; i < width*height; i++)
	{
		estructuraPepinoPeople[i].clear();
		estructuraPepinoPolice[i].clear();
	}

	for (std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		Person* pers = dynamic_cast<Person*> (*it);
		if(pers)
		{
			vec2i p = vec2i(pers->getPosition());
			if(!map->validTile(p.x, p.y)) continue;
			estructuraPepinoPeople[p.x+p.y*width].push_back(pers);
		}
		Police* pol = dynamic_cast<Police*> (*it);
		if(pol)
		{
			vec2i p = vec2i(pol->getPosition());
			if(!map->validTile(p.x, p.y)) continue;
			estructuraPepinoPolice[p.x+p.y*width].push_back(pol);
		}
	}


	for(std::list<GameObject*>::iterator it = objects.begin();it != objects.end(); ++it) {
		(*it)->update(deltaTime);
	}

	doCollisions();

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
//			parent.close();
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
	delete shaderTexture;
	delete shaderColor;
	delete shaderHouse;
}

void SceneMain::addObject(GameObject* object) {
	objects.push_back(object);
}

vector<Person*> SceneMain::getPeopleAround(vec2f pos, float r, SearchType st)
{
	vec2f min = pos - r;
	vec2f max = pos + r;

	vec2i imin = vec2i(min);
	vec2i imax = vec2i(max+1.0f);

	imin = glm::max(imin, vec2i(0, 0));
	imax = glm::min(imax, vec2i(map->getWidth()-1, map->getHeight()-1));

	int width = map->getWidth();

	vector<Person*> res;
	for(int x = imin.x; x <= imax.x; x++) {
		for(int y = imin.y; y <= imax.y; y++)
		{
			vector<Person*>& v = estructuraPepinoPeople[x+y*width];
			for(int i = 0; i < (int) v.size(); i++)
				if( (st == SEARCH_ANY ||
					 (st == SEARCH_DEAD && !v[i]->is_alive()) ||
					 (st == SEARCH_PANIC && v[i]->getState() == Person::STATE_PANIC))
						&& glm::distance(pos, v[i]->getPosition()) <= r)
					res.push_back(v[i]);
		}
	}

	return res;
}

std::vector<Person*> SceneMain::getPeopleSeen(Character* c, SceneMain::SearchType st)
{
	float r = 12;
	vec2f pos = c->getPosition();

	vec2f min = pos - r;
	vec2f max = pos + r;

	vec2i imin = vec2i(min);
	vec2i imax = vec2i(max+1.0f);

	imin = glm::max(imin, vec2i(0, 0));
	imax = glm::min(imax, vec2i(map->getWidth()-1, map->getHeight()-1));

	int width = map->getWidth();

	vector<Person*> res;
	for(int x = imin.x; x <= imax.x; x++) {
		for(int y = imin.y; y <= imax.y; y++)
		{
			vector<Person*>& v = estructuraPepinoPeople[x+y*width];
			for(int i = 0; i < (int) v.size(); i++)
				if( (st == SEARCH_ANY ||
					 (st == SEARCH_DEAD && !v[i]->is_alive()) ||
					 (st == SEARCH_PANIC && v[i]->getState() == Person::STATE_PANIC))
						&& c->canSee(v[i]->getPosition()))
					res.push_back(v[i]);
		}
	}
	return res;
}

/*
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
}*/

std::vector<Police*> SceneMain::getPolices()
{
	std::vector<Police*> res;

	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Police* p = dynamic_cast<Police*> (*it);
		if(p)
			res.push_back(p);
	}

	return res;
}

//FEO, MUY FEO

void collide(Character* a, Character* b)
{
	vec2f pa = a->getPosition();
	vec2f pb = b->getPosition();

	vec2f dir = pb-pa;

	float dist = 0.5;
	float len = glm::length(dir);
	if(len < dist && len > 0.01)
	{
		dir *= 1/len;
		dir *= dist*0.5;
		vec2f mean = (pa+pb)*0.5f;
		a->move(mean-dir);
		b->move(mean+dir);
	}
}

template<class A, class B>
void doCollisions2(vector<A*>& a, vector<B*> b)
{
	for(int i = 0; i < a.size(); i++)
		for(int j = 0; j < b.size(); j++)
			collide((Character*)a[i], (Character*)b[j]);
}

template<class A>
void doCollisions1(vector<A*>& a)
{
	for(int i = 0; i < a.size(); i++)
		for(int j = i+1; j < a.size(); j++)
			collide((Character*)a[i], (Character*)a[j]);
}


void SceneMain::doCollisions()
{
	int tx = map->getWidth();
	int ty = map->getHeight();
	int t = tx*ty;

	int add[4] = {1, tx-1, tx, tx+1};

	doCollisions1<Player>(players);

	for(int i = 0; i < t; i++)
	{
		doCollisions1<Police> (estructuraPepinoPolice[i]);
		doCollisions1<Person> (estructuraPepinoPeople[i]);
		doCollisions2<Police, Player>(estructuraPepinoPolice[i], players);
		doCollisions2<Person, Player>(estructuraPepinoPeople[i], players);

		for(int j = 0; j < 4; j++)
		{
			int i2 = i+add[j];
			if(i2 >= t) break;

			doCollisions2<Police, Police>(estructuraPepinoPolice[i], estructuraPepinoPolice[i2]);
			doCollisions2<Police, Person>(estructuraPepinoPolice[i], estructuraPepinoPeople[i2]);
			doCollisions2<Person, Police>(estructuraPepinoPeople[i], estructuraPepinoPolice[i2]);
			doCollisions2<Person, Person>(estructuraPepinoPeople[i], estructuraPepinoPeople[i2]);
		}
	}
}

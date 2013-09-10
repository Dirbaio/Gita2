#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "Scene.hpp"
#include "Map.hpp"

class GameObject;
class ShaderProgram;
class Player;
class Person;
class Character;

class SceneMain : public Scene {
    public:

	enum SearchType {
	    SEARCH_ANY,
	    SEARCH_DEAD,
	    SEARCH_PANIC
	};


	//virtual overrides
	SceneMain(Game &parent);
	~SceneMain();
	bool init();
	void update(float deltaTime);
	void draw() const;
	void onKeyPressed(float deltaTime, sf::Keyboard::Key key);
	void onKeyDown(float deltaTime, sf::Keyboard::Key key);
	void onKeyReleased(float deltaTime, sf::Keyboard::Key key);
	void onMouseButtonPressed(float deltaTime, sf::Mouse::Button button);
	void onMouseButtonDown(float deltaTime, sf::Mouse::Button button);
	void onMouseButtonReleased(float deltaTime, sf::Mouse::Button button);
	void onMouseMoved(float deltaTime, int dx, int dy);
	void onClose(); //close scene-wide stuff

	void addObject(GameObject* object);

	ShaderProgram* shaderTexture;
	ShaderProgram* shaderColor;
	ShaderProgram* shaderHouse;

	Map* map;
	std::vector<Player*> players;

	std::vector<Person*> getPeopleAround(vec2f pos, float r, SearchType st);
	std::vector<Person*> getPeopleSeen(Character* c, SearchType st);
    private:
	bool loadResources();

	int playerNum;
	std::list<GameObject*> objects;
	float debugCounter;
	int fpsCount;
};

#endif // SCENEMAIN_HPP

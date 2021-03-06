#ifndef INPUTENGINE_H
#define INPUTENGINE_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class InputEng
{
public:
	enum Key
	{
		PLAYER_UP, PLAYER_DOWN, PLAYER_LEFT, PLAYER_RIGHT,
		PLAYER_ACTION,
                SHOP_BUY_1, SHOP_BUY_2, SHOP_BUY_3,
                CAM_UP, CAM_DOWN, CAM_LEFT, CAM_RIGHT,
                CAM_ZOOM_IN, CAM_ZOOM_OUT,
		EXIT, NEW_SCENE,
                MENU_START,
		K_SIZE
	};

    InputEng();

	void Update();

	bool getKeyState(int key);
	bool getKeyUp(int key);
	bool getKeyDown(int key);

	bool isKeyDown(int key);

	bool getMouseDown(sf::Mouse::Button button);
	bool getMouseUp(sf::Mouse::Button button);
	sf::Vector2f getMousePos();
	sf::Vector2f getGlobalMousePos();
	void setGlobalMousePos(const sf::Vector2f& pos);

    std::string encodeToString();
    void decodeFromString(std::string str);

    bool firstUpdate;

private:

    bool KeyState[K_SIZE];
    bool KeyStateOld[K_SIZE];

    sf::Keyboard::Key KeyMap[K_SIZE];

    bool MouseStateOld[sf::Mouse::ButtonCount];
    bool MouseState[sf::Mouse::ButtonCount];

	sf::Vector2i MousePos; //Window coords
	sf::Vector2f globalMousePos; //World coords
};

#endif // INPUTENGINE_H

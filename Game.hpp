#ifndef GAME_HPP
#define GAME_HPP

//	THIS IS THE SFML BINDING OF MY ENGINE. DESIGNED TO WORK ON IT'S OWN CLASS. USAGE:
//	 - Create a class MyScene that inherits from Scene.hpp
//	   It has all types of methods for input & game loop: Key & Mouse input,
//	   Update for logic, Draw for openGL and SFML calls to draw, init, loadResources
//	   to load stuff (use the managers as to load) and an onClose method. It is pretty
//	   straightforward.
//   - Then, on main, just do this:
//
//	#include "Game.hpp"
//  #include "MyScene.hpp"
//
//	int main() {
//		Game myGame;
//		if(myGame.init()) {
//			myGame.setScene(new MyScene(myGame));
//			myGame.run();
//		}
//		return 0;
//	}
//   - It will run right off the bat. Screen size is set to fullscreen (max resolution
//     available), but you can tweak that in Game::Game() and tools.cpp


#include "graphics/TextureManager.hpp"
#include "audio/AudioManager.hpp"
#include "input/InputManager.hpp"
#include "Scene.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"

class Game {
	public:
		Game(); //creates window
		~Game();
		bool init(); // inits game-wide stuff here (init and resource loading)
		void run(); // won't run if Game::init() isn't called first
					// contains main loop, calls update() and draw()
		void setScene(Scene * scene); // sets nextScene to scene, which will
									  // move into currentScene on next update()
									  // so that there is no update() of one
									  // scene followed by a draw() method
									  // of a different scene.
		void close(); // closes app completely, closing the current scene (if there is)
					  // first, saves game-wide stuff first.

		sf::RenderWindow &getWindow() { return window; }
		
		bool isRunning;
	private:
		void update(float deltaTime); // changes scene if necessary
									  // updates fps
									  // checks for window events
									  // updates input with
									  // Game::onMouseButtonPressed(); and
									  // Game::onKeyPressed();
									  // calls currentScene.update(deltaTime)
		void draw(); // calls currentScene.draw()
		bool loadResources (); // loads game-wide resources. only called
							   // by init() once

		//context
		sf::RenderWindow window;
		Scene* currentScene;
		Scene* nextScene;
};

#endif //GAME_HPP

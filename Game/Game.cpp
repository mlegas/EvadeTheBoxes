#include "Game.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "Input.h"
#include "Player.h"
#include "GameObject.h"
#include "Button.h"
#include "SetOriginToCenter.h"

#include <random>
#include <memory>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/ext.hpp>
#include <exception>

Game::Game()
{
	m_window = nullptr;
	m_windowWidth = 800;
	m_windowHeight = 600;
	m_quitGame = false;
	m_quitProgram = false;
	m_choiceMade = false;
}


Game::~Game()
{

}


void Game::menu()
{
	// these lines load the background
	std::vector<glm::vec3> backgroundPos = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
	std::vector<glm::vec2> backgroundTexCoords = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, -1.0f), glm::vec2(0.0f, -1.0f) };
	std::shared_ptr<VertexArray> background = std::make_shared<VertexArray>(backgroundPos, backgroundTexCoords);
	std::shared_ptr<Texture> backgroundTexture = std::make_shared<Texture>("Textures/menubackground.png");
	std::shared_ptr<ShaderProgram> backgroundShader = std::make_shared<ShaderProgram>("Shaders/backgroundShader.vert", "Shaders/backgroundShader.frag");
	
	// and these load the rotating box
	std::shared_ptr<VertexArray> box = std::make_shared<VertexArray>("Models/wooden_crate.obj");
	std::shared_ptr<Texture> boxTexture = std::make_shared<Texture>("Textures/Specular_map.png");
	std::shared_ptr<ShaderProgram> boxShader = std::make_shared<ShaderProgram>("Shaders/Tex.vert", "Shaders/LightingTex.frag");

	// load the font for text
	sf::Font lora;
	lora.loadFromFile("Fonts/Lora-Regular.ttf");
	// load the texture for the button
	sf::Texture btnTexture;
	btnTexture.loadFromFile("Textures/button.png");

	// set the text
	sf::Text title("Evade the Boxes!", lora, 64);
	title.setOutlineThickness(6);
	setOriginToCenter(title); // sets the position of the text to its centre, easier for moving it around
	title.setPosition(sf::Vector2f(400, 125));

	// load up the buttons
	sf::Text buttonText1("Play the Game", lora, 36);
	sf::Text buttonText2("Instructions", lora, 36);
	sf::Text buttonText3("Quit Game", lora, 36);

	// the 1, 2, 3 values represent returned integers on button click
	Button button1(1, btnTexture, buttonText1, sf::Vector2f(200, 300));
	Button button2(2, btnTexture, buttonText2, sf::Vector2f(200, 400));
	Button button3(3, btnTexture, buttonText3, sf::Vector2f(200, 500));

	std::vector<Button> buttonList;
	std::vector<sf::Text> textList;

	buttonList.push_back(button1);
	buttonList.push_back(button2);
	buttonList.push_back(button3);

	textList.push_back(title);
	textList.push_back(buttonText1);
	textList.push_back(buttonText2);
	textList.push_back(buttonText3);

	float angle = 0.0f;

	while (!m_quitProgram)
	{
		m_choiceMade = false;
		while (m_window->isOpen() && !m_choiceMade)
		{
			// clear the view
			glViewport(0, 0, m_windowWidth, m_windowHeight);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// as SFML uses OpenGL as well to draw its sprites, the background will get covered by the buttons during depth testing as the background is only two triangles.
			// It's simpler to just disable the depth test and cull-facing instead of subdividing the triangle.
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);

			// drawing the background could have been done with NDC, but I decided to use an orthographic projection instead.
			backgroundShader->setUniform("in_Projection", glm::ortho(0.0f, (float)m_windowWidth, 0.0f, (float)m_windowHeight, 0.0f, 1.0f));
			backgroundShader->setUniform("in_Texture", backgroundTexture);

			glm::mat4 model(1.0f);
			model = glm::scale(model, glm::vec3(m_windowWidth, m_windowHeight, 1));
			backgroundShader->setUniform("in_Model", model);
			backgroundShader->draw(background);

			glEnable(GL_DEPTH_TEST); 		
			glEnable(GL_CULL_FACE);					
			boxShader->setUniform("in_Projection", glm::perspective(glm::radians(45.0f),
				(float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.f));

			// camera target for the menu
			model = glm::mat4(1.0f);
			model = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			// setting up the shaders for the rotating box
			boxShader->setUniform("in_View", glm::inverse(model));
			boxShader->setUniform("in_LightPos", glm::vec3(1.0f, 2.0f, 0.0f));
			boxShader->setUniform("in_Emissive", glm::vec3(0.1f, 0.1f, 0.1f));
			boxShader->setUniform("in_Ambient", glm::vec3(0.3f, 0.3f, 0.3f));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(1.2f, -1.25f, -5.0f));
			model = glm::scale(model, glm::vec3(0.6, 0.6, 0.6));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
			boxShader->setUniform("in_Model", model);
			boxShader->setUniform("in_Texture", boxTexture);
			boxShader->draw(box);

			angle += 0.1f;

			// reset the angle if it gets over 360, we don't want to get accidentally into FLT_MAX territory
			if (angle >= 360.0f)
			{
				angle = 0.0f;
			}

			// stores the OpenGL states before drawing SFML texts and sprites
			m_window->pushGLStates();

			sf::Event event;

			while (m_window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					// close the game (return to main) if the window is closed
					m_choiceMade = true;
					m_quitGame = true;
					m_quitProgram = true;
					m_window->close();
				}

				else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					std::vector<Button>::iterator buttonListIterator;

					// go through the button list and check if any were clicked
					for (buttonListIterator = buttonList.begin(); buttonListIterator != buttonList.end(); ++buttonListIterator)
					{
						if ((*buttonListIterator).isClicked(m_window))
						{
							m_choiceMade = true;
							switch ((*buttonListIterator).getAction())
							{
								case 1:
								{
									gameLoop();
									m_window->resetGLStates(); // reset the states storage after coming back from the function to ensure that 
															   // there is no possibility of, for example, stack overflow
									m_quitGame = false;
									break;
								}
								case 2:
								{
									instructions();
									m_window->resetGLStates();
									break;
								}
								case 3:
								{
									m_quitProgram = true;
									m_quitGame = true;
									m_window->close();
									break;
								}
								default:
								{
									break;
								}
							}
							m_choiceMade = false;
						}
					}
				}
			}

			std::vector<Button>::iterator buttonListIterator2;

			// check if the mouse is hovering over any of the buttons, if yes - put transparency on
			for (buttonListIterator2 = buttonList.begin(); buttonListIterator2 != buttonList.end(); ++buttonListIterator2)
			{
				(*buttonListIterator2).isHovered(m_window);
				m_window->draw((*buttonListIterator2).getSprite());
			}

			std::vector<sf::Text>::iterator textListIterator;

			// draw the text
			for (textListIterator = textList.begin(); textListIterator != textList.end(); ++textListIterator)
			{
				m_window->draw(*textListIterator);
			}

			// displays everything in order
			m_window->display();
			
			// remove the stored OpenGL states
			m_window->popGLStates();
		}
	}
}

void Game::instructions()
{
	// this function is quite the same as menu() in use, only changes are box and text positions, as well as strings.

	// this boolean makes sure the functions draws everything (for running safety) before returning by clicking the button.
	bool safeReturn = false;

	std::vector<glm::vec3> backgroundPos = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
	std::vector<glm::vec2> backgroundTexCoords = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, -1.0f), glm::vec2(0.0f, -1.0f) };
	std::shared_ptr<VertexArray> background = std::make_shared<VertexArray>(backgroundPos, backgroundTexCoords);
	std::shared_ptr<Texture> backgroundTexture = std::make_shared<Texture>("Textures/menubackground.png");
	std::shared_ptr<ShaderProgram> backgroundShader = std::make_shared<ShaderProgram>("Shaders/backgroundShader.vert", "Shaders/backgroundShader.frag");

	std::shared_ptr<VertexArray> box = std::make_shared<VertexArray>("Models/wooden_crate.obj");
	std::shared_ptr<Texture> boxTexture = std::make_shared<Texture>("Textures/Specular_map.png");
	std::shared_ptr<ShaderProgram> boxShader = std::make_shared<ShaderProgram>("Shaders/Tex.vert", "Shaders/LightingTex.frag");

	sf::Font lora;
	lora.loadFromFile("Fonts/Lora-Regular.ttf");
	sf::Texture btnTexture;
	btnTexture.loadFromFile("Textures/button.png");

	sf::Text text1("The objective in this game is to avoid boxes", lora, 24);
	text1.setOutlineThickness(3);
	setOriginToCenter(text1);
	text1.setPosition(sf::Vector2f(400, 50));
	sf::Text text2("falling from the sky, such as this one:", lora, 24);
	text2.setOutlineThickness(3);
	setOriginToCenter(text2);
	text2.setPosition(sf::Vector2f(400, 90));
	sf::Text text3("WSAD - Movement", lora, 36);
	text3.setOutlineThickness(3);
	setOriginToCenter(text3);
	text3.setPosition(sf::Vector2f(400, 270));
	sf::Text text4("K - Begin the fall", lora, 36);
	text4.setOutlineThickness(3);
	setOriginToCenter(text4);
	text4.setPosition(sf::Vector2f(400, 350));
	sf::Text text5("Q, ALT+F4 - Quit to menu", lora, 36);
	text5.setOutlineThickness(3);
	setOriginToCenter(text5);
	text5.setPosition(sf::Vector2f(400, 420));

	sf::Text buttonText("Back to Menu", lora, 36);
	
	Button button(1, btnTexture, buttonText, sf::Vector2f(600, 500));

	std::vector<sf::Text> textList;

	textList.push_back(text1);
	textList.push_back(text2);
	textList.push_back(text3);
	textList.push_back(text4);
	textList.push_back(text5);
	textList.push_back(buttonText);

	float angle = 0.0f;

	while (m_window->isOpen())
	{
		glViewport(0, 0, m_windowWidth, m_windowHeight);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		backgroundShader->setUniform("in_Projection", glm::ortho(0.0f, (float)m_windowWidth, 0.0f, (float)m_windowHeight, 0.0f, 1.0f));
		backgroundShader->setUniform("in_Texture", backgroundTexture);

		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(m_windowWidth, m_windowHeight, 1));
		backgroundShader->setUniform("in_Model", model);
		backgroundShader->draw(background);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		boxShader->setUniform("in_Projection", glm::perspective(glm::radians(45.0f),
			(float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.f));

		model = glm::mat4(1.0f);
		model = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		boxShader->setUniform("in_View", glm::inverse(model));
		boxShader->setUniform("in_LightPos", glm::vec3(1.0f, 2.0f, 0.0f));
		boxShader->setUniform("in_Emissive", glm::vec3(0.1f, 0.1f, 0.1f));
		boxShader->setUniform("in_Ambient", glm::vec3(0.3f, 0.3f, 0.3f));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
		boxShader->setUniform("in_Model", model);
		boxShader->setUniform("in_Texture", boxTexture);
		boxShader->draw(box);

		angle += 0.1f;

		if (angle >= 360.0f)
		{
			angle = 0.0f;
		}

		m_window->pushGLStates();

		sf::Event event;

		while (m_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_choiceMade = true;
				m_quitGame = true;
				m_quitProgram = true;
				m_window->close();
			}

			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (button.isClicked(m_window))
				{
					safeReturn = true;
				}
			}
		}

		button.isHovered(m_window);
		m_window->draw(button.getSprite());

		std::vector<sf::Text>::iterator textListIterator;

		for (textListIterator = textList.begin(); textListIterator != textList.end(); textListIterator++)
		{
			m_window->draw(*textListIterator);
		}

		m_window->display();
		m_window->popGLStates();

		if (safeReturn)
		{
			return;
		}
	}
}



void Game::gameLoop()
{
	// set the mousepos to the middle of the screen, as the mouse cursor is grabbed in the window.
	sf::Mouse::setPosition(sf::Vector2i(m_windowWidth / 2, m_windowHeight / 2), *m_window);

	// locks the mouse in the window and hides the cursor.
	m_window->setMouseCursorGrabbed(true);
	m_window->setMouseCursorVisible(false);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// initializes input, the player, the camera, the floor and a vector of boxes.
	std::shared_ptr<Input> input = std::make_shared<Input>();
	std::shared_ptr<Player> player = std::make_shared<Player>();
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(player->getSpeed(), m_windowWidth, m_windowHeight);
	std::shared_ptr<GameObject> floor = std::make_shared<GameObject>("Models/woodfloor.obj", "Textures/woodfloor.jpg", "Shaders/Tex.vert", "Shaders/LightingTex.frag");
	std::vector<std::shared_ptr<GameObject>> boxes;

	// these lines initialize the skybox, which is a cube with internal walls textured.
	std::shared_ptr<VertexArray> skybox = std::make_shared<VertexArray>("Models/bigcube.obj");
	std::shared_ptr<Texture> skyboxTexture = std::make_shared<Texture>("Textures/skybox.jpg");
	std::shared_ptr<ShaderProgram> skyboxShader = std::make_shared<ShaderProgram>("Shaders/Tex.vert", "Shaders/noLightingTex.frag");

	sf::Clock clock; // clock for deltatime
	sf::Clock crashClock; // clock for the time survived without crashing
	sf::Time idealTime = sf::seconds(0.016667f);
	sf::Time deltaTime = idealTime;
	sf::Time bestTime = sf::seconds(0.0f);

	// values which will be displayed on the screen with text
	int boxesInWorld = 0;
	int spawnedBoxes = 0;
	int fps = 0;

	bool gameStarted = false;
	
	// sound initialization
	sf::SoundBuffer crateBuffer;
	crateBuffer.loadFromFile("Sounds/crateBreak.wav");
	sf::Sound crateBreak;
	crateBreak.setBuffer(crateBuffer);

	sf::Music gameMusic;
	gameMusic.openFromFile("Sounds/gameMusic.ogg");

	gameMusic.play();
	gameMusic.setLoop(true);

	sf::Time crashTime;
	sf::Time eventTimer;

	sf::Font lora;
	lora.loadFromFile("Fonts/Lora-Regular.ttf");

	// text initialization
	sf::Text text1("Crashed: " + std::to_string(player->getCrashAmount()), lora, 20);
	text1.setOutlineThickness(3);
	setOriginToCenter(text1);
	text1.setPosition(sf::Vector2f(60, 20));
	sf::Text text2("Time without crashing: " + std::to_string(crashTime.asSeconds()) + "s", lora, 20);
	text2.setOutlineThickness(3);
	setOriginToCenter(text2);
	text2.setPosition(sf::Vector2f(170, 580));
	sf::Text text3("Falling boxes: " + std::to_string(boxesInWorld), lora, 20);
	text3.setOutlineThickness(3);
	setOriginToCenter(text3);
	text3.setPosition(sf::Vector2f(680, 580));
	sf::Text text4("Already spawned boxes: " + std::to_string(spawnedBoxes), lora, 20);
	text4.setOutlineThickness(3);
	setOriginToCenter(text4);
	text4.setPosition(sf::Vector2f(640, 20));
	sf::Text text5("FPS: " + std::to_string(fps), lora, 20);
	text5.setOutlineThickness(3);
	setOriginToCenter(text5);
	text5.setPosition(sf::Vector2f(400, 20));
	sf::Text text6("Best time: " + std::to_string(bestTime.asSeconds()) + "s", lora, 20);
	text6.setOutlineThickness(3);
	setOriginToCenter(text6);
	text6.setPosition(sf::Vector2f(470, 579));

	std::vector<sf::Text> textList;

	textList.push_back(text1);
	textList.push_back(text2);
	textList.push_back(text3);
	textList.push_back(text4);
	textList.push_back(text5);
	textList.push_back(text6);

	while (m_window->isOpen() && !m_quitGame)
	{
		glViewport(0, 0, m_windowWidth, m_windowHeight);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// we need to disable depth testing to draw the skybox first
		glDisable(GL_DEPTH_TEST);

		// this line makes sure that the skybox only rotates, not moves around
		// reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps

		glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));

		skyboxShader->setUniform("in_Projection", camera->getProjectionMatrix());
		skyboxShader->setUniform("in_View", view);
		skyboxShader->setUniform("in_Texture", skyboxTexture);

		glm::mat4 model(1.0f);
		skyboxShader->setUniform("in_Model", model);

		skyboxShader->draw(skybox);

		glEnable(GL_DEPTH_TEST);

		// check if there was any mouse and/or keyboard input
		// camera is passed as a parameter to update its mousepos if the mouse moved
		// m_quitGame - if the player closed the window
		input->checkForInput(m_window, m_quitGame, camera);

		// checks which keys have been pressed and moves the camera
		camera->update(input->getPressedKeys(), m_window, deltaTime);

		// moves the bounding box min and max values and the player's position, according to camera movement.
		player->update(camera->getPosition());

		// check if player began the fall
		if (input->getPressedKeys().at(4) == true)
		{
			if (!gameStarted) // begin running the clock with the start of the fall
			{
				crashClock.restart();
				gameStarted = true;
			}

			crashTime = crashClock.getElapsedTime();

			eventTimer += deltaTime;

			if (eventTimer.asSeconds() >= 0.08f) // spawn a new crate every 0.08 seconds
			{
				eventTimer = sf::seconds(0.0f);

				++spawnedBoxes;

				std::random_device generator;
				std::uniform_real_distribution<float> distribution(-10.0f, 10.0f); // gives the crate a random X and Z position that is located on the floor
				std::shared_ptr<GameObject> box = std::make_shared<GameObject>("Models/wooden_crate.obj", "Textures/Specular_map.png", "Shaders/Tex.vert", "Shaders/LightingTex.frag");
				box->setPosition(glm::vec3(distribution(generator), 200.0f, distribution(generator))); // sets the position of the box

				boxes.push_back(box);
			}

			if (!boxes.empty())
			{
				std::vector<std::shared_ptr<GameObject>>::iterator boxIterator;

				for (boxIterator = boxes.begin(); boxIterator != boxes.end();)
				{
					(*boxIterator)->update(deltaTime); // update the position of each box using the gravitational force and update the model matrix
					(*boxIterator)->draw(camera); // draw each box

					if ((*boxIterator)->getPosition().y < -3.0f) // remove a box if it fell low enough
					{
						boxIterator = boxes.erase(boxIterator);
					}
					else
					{
						++boxIterator;
					}
				}

				player->checkCollisions(boxes); // check if the player collided with any of the boxes

				std::vector<std::shared_ptr<GameObject>>::iterator boxIterator2;

				for (boxIterator2 = boxes.begin(); boxIterator2 != boxes.end();)
				{
					if ((*boxIterator2)->getCollision()) // if this box collided with the player:
					{
						crateBreak.play(); // play a sound
						boxIterator2 = boxes.erase(boxIterator2); // erase the box
						
						if (crashTime > bestTime)
						{
							bestTime = crashTime; // set a new best time if it has been beaten
						}

						crashTime = crashClock.restart(); // reset the crash clock
					}
					else
					{
						++boxIterator2;
					}
				}
			}
		}

		floor->update(); // update the model matrix
		floor->draw(camera); // draw the floor

		deltaTime = clock.restart(); // restarting the clock returns elapsed time as well

		boxesInWorld = boxes.size(); // get how many boxes are currently spawned
		fps = 1.0f / deltaTime.asSeconds(); // calculate fps 

		m_window->pushGLStates(); // store the current OpenGL states
		
		// update the texts
		text1.setString("Crashed: " + std::to_string(player->getCrashAmount()));
		text2.setString("Time without crashing: " + std::to_string(crashTime.asSeconds()) + "s");
		text3.setString("Falling boxes: " + std::to_string(boxesInWorld));
		text4.setString("Already spawned boxes: " + std::to_string(spawnedBoxes));
		text5.setString("FPS: " + std::to_string(fps));
		text6.setString("Best time: " + std::to_string(bestTime.asSeconds()) + "s");

		textList.at(0) = text1;
		textList.at(1) = text2;
		textList.at(2) = text3;
		textList.at(3) = text4;
		textList.at(4) = text5;
		textList.at(5) = text6;

		std::vector<sf::Text>::iterator textListIterator;

		// draw the texts
		for (textListIterator = textList.begin(); textListIterator != textList.end(); ++textListIterator)
		{
			m_window->draw(*textListIterator);
		}

		m_window->display();
		m_window->popGLStates();

	}

	// if the player quits to main menu, unlock the cursor from the window and make it visible
	m_window->setMouseCursorGrabbed(false);
	m_window->setMouseCursorVisible(true);
	// stop playing the music
	gameMusic.stop();
}

void Game::initialize()
{
	// creates a window with OpenGL settings: 24 depth bits, 8 stencil bits, 8x antialiasing, and OpenGL version 3.3.
	m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "OpenGL", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(24, 8, 8, 3, 3));

	if (glewInit() != GLEW_OK) // initializes GLEW
	{
		throw std::exception();
	}
	m_window->setActive(true); // sets the context for OpenGL

	menu(); // calls the main menu function
}


#define GAME_CLEAR

#define SET_ANTIALIASHING	// this macro allows you to set antialiashing



#include"modules/requirements.h"



// the value returned by this function will be set as the value for antialiashing

unsigned int bb::set_antialiashing()
{
	return 0;	// no antialiashing
}



/*
	infinite scrolling background

	- bg_sprite: the sprite to be used as background
	- 52: texture repeating point in x axis
	- 20: speed of scrolling in x axis
	- VIRTUAL_HEIGHT: height of the screen
*/

bb::InfScrollingBG bg(bg_sprite, 52, 20, VIRTUAL_HEIGHT);



/*
	this function gets executed after all the objects in the game are created and all
	the assets and save files are loaded but before the game loop starts
*/

inline bool bb::Game::Create()
{


	// setting window title



	bb::WINDOW.setTitle("Match 3 Neo");



	// setting window size



	bb::WINDOW.setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));

	sf::View this_view = bb::WINDOW.getView();

	this_view.reset(sf::FloatRect(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT));

	bb::WINDOW.setView(this_view);



	// other settings



	bb::WINDOW.setPosition(sf::Vector2i(100, 100));

	bb::Game::set_fps(60);



	// setting up initial state



	sm.change_to(initial);



	// starting the background music



	bg_music.setLoop(true);

	bg_music.play();



	return SUCCESS;	// initialization is a success, the game loop can start
}



inline bool bb::Game::Update(double dt)
{
	sm.Update(dt);

	bg.update(dt);

	// exit

	/*
		when the game window is crossed change state machine to null state
		to close current state properly before exiting the game loop to close
		the game properly
	*/

	if (bb::INPUT.isClosed())
		sm.change_to(bb::NULL_STATE);

	// if null state is detected stop the game loop

	if (sm.null_state())
	{
		bg_music.stop();	// stopping the background music

		return STOP_GAME_LOOP;
	}

	return !STOP_GAME_LOOP;
}



inline void bb::Game::Clear()
{
	bg.render();
}



inline void bb::Game::Render()
{
	sm.Render();

	// rendering the fps

	small_text.setString("Fps: " + std::to_string(static_cast<int>(bb::MY_GAME.get_fps() + .5)));

	bb::WINDOW.draw(small_text);
}
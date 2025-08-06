

#define GAME_CLEAR

#define SET_ANTIALIASHING	// this macro allows you to set antialiashing



#include"modules/requirements.h"



// the value returned by this function will be set as the value for antialiashing

unsigned int bb::set_antialiashing()
{
	return 0;
}



background bg;



// ~~~~ [write your statements (extra functions, classes, variables) here] ~~~~

inline bool bb::Game::Create()
{

	// setting window title


	bb::WINDOW.setTitle("CandyCrush");


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



	// creating the brickmap

	//board.generate_brickmap(sf::Vector2f(128, 16));


	return SUCCESS;
}



inline bool bb::Game::Update(double dt)
{
	// exit

	if (bb::INPUT.isClosed() || bb::INPUT.isPressed(sf::Keyboard::Scan::Escape) || sm.null_state())
		return STOP_GAME_LOOP;

	
	sm.Update(dt);

	bg.update(dt);


	return !STOP_GAME_LOOP;
}



inline void bb::Game::Clear()
{
	bg.render();
}



inline void bb::Game::Render()
{
	// rendering the fps
	
	sm.Render();

	small_text.setString("Fps: " + std::to_string(static_cast<int>(bb::MY_GAME.get_fps() + .5)));

	bb::WINDOW.draw(small_text);
}
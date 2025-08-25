#pragma once



extern class play_state play;

extern class highest_score_state highest_score;



class initial_state : public bb::BASE_STATE
{
	highest_score_type h_data;	// to store the highest score data


	sf::Vector2f offset;	// offset for the brick map

	board_class board;		// the brickmap displayed as decoration


	sf::RectangleShape bg_dimmer;	// to dim the background


	// the game menu

	bb::RoundedRectangleShape bg_menu;

	bb::MENU<bb::STR_BUTTON> menu;


	// the header

	bb::RoundedRectangleShape bg_header;

	bb::ColorText header;


	bb::ScreenFade screen;	// creates the fade in and fade out effect


	
public:

	initial_state() :
		offset(128, 16),	// place the brick map at the center of the screen
		bg_dimmer(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		screen(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		menu(
			bb::STR_BUTTON::make_menu(
				medium_text,
				{ "Continue", "Start", "Highest Score", "Quite" },	// 4 options
				sf::Color(48, 96, 130),		// ordinary color
				sf::Color(99, 155, 255),	// hover color
				10,							// gap between options
				sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + 10),	// coords of top center of the menu
				bb::TOP_CENTER
			)
		)
	{
		bg_dimmer.setFillColor(sf::Color(0, 0, 0, 128));

		// background of the menu

		bg_menu.setRadius(6);

		bg_menu.setCornerPointCount(12);

		bg_menu.setFillColor(sf::Color(255, 255, 255, 128));

		bg_menu.setSize(sf::Vector2f(160, 10 + (MEDIUM_FONT_SIZE + 10) * 4));

		bg_menu.setOrigin(bg_menu.getSize().x / 2, 0);

		bg_menu.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2));

		// background of the header

		bg_header = bg_menu;

		bg_header.setSize(sf::Vector2f(160, BRICK_HEIGHT * 2));

		bg_header.setOrigin((bg_header.getSize().x - 1) / 2, (bg_header.getSize().y - 1) / 2);

		bg_header.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - BRICK_HEIGHT * 2));

		// preparing the header

		header.set(
			large_text,
			"MATCH 3",
			3.5,	// gap between letters
			sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - BRICK_HEIGHT * 2),	// center coord of the header
			bb::CENTER
		);

		// setting colors to cycle through

		header.setColor(
			{
				{217, 87, 99},
				{95, 205, 228},
				{251, 242, 54},
				{118, 66, 138},
				{153, 229, 80},
				{223, 113, 38}
			}
		);
	}

	void Enter() override
	{
		// initial state emarges from dark, the fade in effect

		screen.setColor(sf::Color::Black);

		screen.startFadeIn();
	}

	void Update(double dt) override
	{
		if(screen.xfinal())
		{
			// xfinal changes the game state, so we return after it's executed

			return;
		}

		// always update the colorful texts of header

		header.updateRS(dt);

		// update the menu but don't use the input if the input is locked

		auto sel = bb::menu_update(menu);

		// don't take input when tween operations are running

		if (screen.isActive())
			return;

		// playing the UI sounds

		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Up) || bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			play_sound(POINTER);
		}

		if ((bb::INPUT.isReleasedM(sf::Mouse::Left) || bb::INPUT.isPressed(sf::Keyboard::Scan::Enter)) && sel != -1)
		{
			play_sound(GAME_START);
		}

		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			play_sound(GAME_START);
		}

		// continue

		if (sel == 0)
		{
			// fade out effect the screen goes white

			screen.setColor(sf::Color::White);

			screen.startFadeOut([this](){ sm.change_to(play, &h_data); });
		}

		// start (new game)

		if (sel == 1)
		{
			// fade out effect the screen goes white

			screen.setColor(sf::Color::White);

			screen.startFadeOut([this]() { sm.change_to(play, &h_data, true); });	// true, forces play state to reset its data
		}

		// high_score

		if (sel == 2)
		{
			// fade out effect the screen goes white

			screen.setColor(sf::Color::White);

			screen.startFadeOut([this]() { sm.change_to(highest_score, h_data.highest_score); });	// true, forces play state to reset its data
		}

		// quite

		if (sel == 3 || bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			// fade out effect the screen goes black slowly, before the game quites

			screen.setColor(sf::Color::Black);

			screen.startFadeOut([]() { sm.change_to(bb::NULL_STATE); });
		}
	}

	void Render() override
	{
		board.render(offset);

		bb::WINDOW.draw(bg_dimmer);

		// header background

		bb::WINDOW.draw(bg_header);

		// header shadow

		for (int i = 0; i < header.getTextCount(); i++)
		{
			bb::shadow(header.getText(i), sf::Color(34, 32, 52), sf::Vector2f{1.5, 1.5}, sf::Vector2f{1.5, 1}, sf::Vector2f{0, 1.5});
		}

		// header

		header.render();

		// menu background

		bb::WINDOW.draw(bg_menu);

		// menu shadow

		for (int i = 0; i < menu.get_bcount(); i++)
		{
			bb::shadow(menu[i].get_text(), sf::Color(34, 32, 52), sf::Vector2f{1.5, 1.5}, sf::Vector2f{1.5, 1}, sf::Vector2f{0, 1.5});
		}

		// menu

		menu.Render(bb::STR_BUTTON::color);


		// rendering the screen fade effects
		
		screen.render();
	}

	void Exit() override
	{
		// stopping all the threads before we exit this state

		screen.stop();	// stopping the thread inside the tweener of screen fade effect
	}
} initial;
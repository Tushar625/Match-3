#pragma once

extern class play_state play;

extern class highest_score_state highest_score;

class initial_state : public bb::BASE_STATE
{
	highest_score_type h_data;


	sf::Vector2f offset;

	board_class board;


	sf::RectangleShape bg_dimmer;


	bb::RoundedRectangleShape bg_menu;

	bb::MENU<bb::STR_BUTTON> menu;


	bb::RoundedRectangleShape bg_header;

	bb::ColorText header;


	bb::ScreenFade screen;	// creates the fade in and fade out effect


	
public:

	initial_state() :
		offset(128, 16),
		bg_dimmer(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		screen(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		menu(
			bb::STR_BUTTON::make_menu(
				medium_text,
				{ "Continue", "Start", "Highest Score", "Quite" },
				sf::Color(48, 96, 130),
				sf::Color(99, 155, 255),
				10,
				sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + 10),
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
			3.5,
			sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - BRICK_HEIGHT * 2),
			bb::CENTER
		);

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

		header.updateRS(dt);

		// update teh menu but don't use the input if the input is locked

		auto sel = bb::menu_update(menu);

		// don't take input when tween operations are running

		if (screen.isActive())
			return;


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

		// play = 0, high_score = 1, quit = 2

		if (sel == 0)
		{
			// fade out effect the screen goes white

			screen.setColor(sf::Color::White);

			screen.startFadeOut([this](){ sm.change_to(play, &h_data); });
		}

		if (sel == 1)
		{
			// fade out effect the screen goes white

			screen.setColor(sf::Color::White);

			screen.startFadeOut([this]() { sm.change_to(play, &h_data, true); });	// true, forces play state to reset its data
		}

		if (sel == 2)
		{
			// fade out effect the screen goes white

			screen.setColor(sf::Color::White);

			screen.startFadeOut([this]() { sm.change_to(highest_score, h_data.highest_score); });	// true, forces play state to reset its data
		}

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
			bb::textShadow(header.getText(i), { {1.5, 1.5}, {1.5, 1}, {0, 1.5} }, sf::Color(34, 32, 52));
		}

		// header

		header.render();

		// menu background

		bb::WINDOW.draw(bg_menu);

		// menu shadow

		for (int i = 0; i < menu.get_bcount(); i++)
		{
			bb::textShadow(menu[i].get_text(), { {1.5, 1.5}, {1.5, 1}, {0, 1.5} }, sf::Color(34, 32, 52));
		}

		// menu

		menu.Render(bb::STR_BUTTON::color);


		// rendering the curtain
		
		screen.render();
	}

	void Exit() override
	{
		screen.stop();
	}
} initial;
#pragma once


class initial_state : public bb::BASE_STATE
{
	sf::Vector2f offset;

	board_class board;

	sf::RectangleShape bg_dimmer;

	RoundedRectangle bg_menu;

	bb::MENU<bb::STR_BUTTON> menu;

	RoundedRectangle bg_header;

	ColorText header;

	
public:

	initial_state() :
		offset(128, 16),
		bg_dimmer(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		menu(
			bb::STR_BUTTON::make_menu(
				medium_text,
				{ "Continue", "Start", "Highest Score", "Quite" },
				sf::Color(48, 96, 130),
				sf::Color(99, 155, 255),
				10,
				VIRTUAL_WIDTH / 2,
				VIRTUAL_HEIGHT / 2 + 10,
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

		bg_header.setOrigin(bg_header.getSize().x / 2, bg_header.getSize().y / 2);

		bg_header.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - BRICK_HEIGHT * 2));

		// preparing the header

		header.set(large_text, "MATCH 3", 3, VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - BRICK_HEIGHT * 2, bb::CENTER);

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
	}

	void Update(double dt) override
	{
		auto sel = bb::menu_update(menu);

		/*if (bb::INPUT.isPressed(sf::Keyboard::Scan::Up) || bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			sound.setBuffer(sound_buffer[SELECT]);

			sound.play();
		}

		if ((bb::INPUT.isReleasedM(sf::Mouse::Left) || bb::INPUT.isPressed(sf::Keyboard::Scan::Enter)) && sel != -1)
		{
			sound.setBuffer(sound_buffer[CONFIRM]);

			sound.play();
		}*/

		// play = 0, high_score = 1, quit = 2

		/*if (sel == 0)
		{
			game_state.change_to(serve, &i_data);
		}

		if (sel == 1)
		{
			game_state.change_to(highest_score, i_data.highest_score);
		}*/

		if (sel == 3/* || bb::INPUT.isPressed(sf::Keyboard::Scan::Escape)*/)
		{
			sm.change_to(bb::NULL_STATE);
		}

		header.updateRS(dt);
	}

	void Render() override
	{
		board.render(offset);

		bb::WINDOW.draw(bg_dimmer);

		// header

		bb::WINDOW.draw(bg_header);

		header.render();

		// menu background

		bb::WINDOW.draw(bg_menu);

		// menu shadow

		for (int i = 0; i < menu.get_bcount(); i++)
		{
			auto text = menu[i].get_text();

			text.setFillColor(sf::Color(34, 32, 52));

			auto pos = text.getPosition();

			//text.setOutlineThickness(.5);

			text.setPosition(pos + sf::Vector2f(1.5, 1)); bb::WINDOW.draw(text);

			text.setPosition(pos + sf::Vector2f(1.5, 1.5)); bb::WINDOW.draw(text);

			text.setPosition(pos + sf::Vector2f(1, 1)); bb::WINDOW.draw(text);

			text.setPosition(pos + sf::Vector2f(0, 1)); bb::WINDOW.draw(text);

			text.setPosition(pos + sf::Vector2f(0, 1.5)); bb::WINDOW.draw(text);

			text.setPosition(pos + sf::Vector2f(1, 1.5)); bb::WINDOW.draw(text);
		}

		menu.Render(bb::STR_BUTTON::color);
	}

	void Exit() override
	{
	}
} initial;
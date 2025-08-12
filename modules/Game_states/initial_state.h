#pragma once

extern class play_state play;

class initial_state : public bb::BASE_STATE
{
	sf::Vector2f offset;

	board_class board;


	sf::RectangleShape bg_dimmer;
	
	
	sf::RectangleShape curtain;

	int curtain_alpha;

	TWEENER tween;



	RoundedRectangle bg_menu;

	bb::MENU<bb::STR_BUTTON> menu;


	RoundedRectangle bg_header;

	ColorText header;


	
public:

	initial_state() :
		offset(128, 16),
		bg_dimmer(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		curtain(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		curtain_alpha(0),
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
		curtain_alpha = 255;

		curtain.setFillColor(sf::Color::Black);

		tween.start(
			1,
			twn(curtain_alpha, 255, 0)
		);
	}

	void Update(double dt) override
	{
		if(tween.xfinal())
		{
			// xfinal changes the game state, so we return after it's executed

			return;
		}

		header.updateRS(dt);

		// don't take input when tween operations are running

		if (tween.is_running())
			return;


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

		if (sel == 0)
		{
			curtain.setFillColor(sf::Color::White);

			tween.start(
				1,
				twn(curtain_alpha, 0, 255),
				[](double dt)
				{
					sm.change_to(play);
				}
			);
		}

		if (sel == 1)
		{
			curtain.setFillColor(sf::Color::White);

			tween.start(
				1,
				twn(curtain_alpha, 0, 255),
				[](double dt)
				{
					sm.change_to(play, true);	// request to start a new game
				}
			);
		}

		if (sel == 3 || bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			curtain.setFillColor(sf::Color::Black);

			tween.start(
				1,
				twn(curtain_alpha, 0, 255),
				[](double dt)
				{
					sm.change_to(bb::NULL_STATE);	// exit game
				}
			);
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
			textShadow(header.getText(i), { {1.5, 1.5}, {1.5, 1}, {0, 1.5} }, sf::Color(34, 32, 52));
		}

		// header

		header.render();

		// menu background

		bb::WINDOW.draw(bg_menu);

		// menu shadow

		for (int i = 0; i < menu.get_bcount(); i++)
		{
			textShadow(menu[i].get_text(), { {1.5, 1.5}, {1.5, 1}, {0, 1.5} }, sf::Color(34, 32, 52));
		}

		// menu

		menu.Render(bb::STR_BUTTON::color);


		// rendering the curtain
		
		tween.lock();

		auto color = curtain.getFillColor();

		color.a = curtain_alpha;

		curtain.setFillColor(sf::Color(color));

		tween.unlock();

		bb::WINDOW.draw(curtain);
	}

	void Exit() override
	{
	}
} initial;
#pragma once


class game_over_state : public bb::BASE_STATE
{
	/*
		used to create fade in/out effect
	*/

	bb::ScreenFade screen;	// creates the fade in and fade out effect


	// banner animation for new record

	bb::Banner banner;

	bool new_record;


	// score board

	bb::RoundedRectangleShape board;


	// texts

	sf::Text text, score_text;


	// the button

	bb::STR_BUTTON enter;


public:



	game_over_state() :
		enter(
			medium_text,
			"Enter",
			sf::Color(99, 155, 255),	// normal color
			sf::Color::Cyan,			// hover color
			// bottom center coordinates of the button
			sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + ((10 + (MEDIUM_FONT_SIZE + 10) * 4) * 1.5) / 2 - 16),
			bb::BOTTOM_CENTER
		),
		banner(
			large_text,
			sf::Vector2f(VIRTUAL_WIDTH, BRICK_HEIGHT * 1.5),	// banner size
			VIRTUAL_HEIGHT,		// fall height
			sf::Color(255, 255, 255),		// text color
			sf::Color(95, 205, 228, 200)	// banner color
		)
	{
		// rounded rectangle

		board.setSize(sf::Vector2f(160, 10 + (MEDIUM_FONT_SIZE + 10) * 4) * 1.5f);

		board.setRadius(6);

		board.setCornerPointCount(12);

		board.setFillColor(sf::Color(63, 63, 116, 245));

		board.setOrigin(board.getSize() / 2.0f);

		board.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2));
		
		// the info text

		text = medium_text;

		text.setFillColor(sf::Color(99, 155, 255));

		// the score text

		score_text = medium_text;

		score_text.setFillColor(sf::Color(99, 155, 255));
	}



	void init(int score, bool new_record)
	{
		this->new_record = new_record;

		// setting info text

		if (new_record)
		{
			text.setString("Congrats, New Record");
		}
		else
		{
			text.setString("Here's Your Score");
		}

		bb::textCenterOrigin(text);

		text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - MEDIUM_FONT_SIZE / 2 - 5));

		// setting score text
		
		score_text.setString(std::to_string(score));

		bb::textCenterOrigin(score_text);

		score_text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + MEDIUM_FONT_SIZE / 2 + 5));
	}



	void Enter()	// initialze this state
	{
		// game over state emarges from blood red, the fade in effect

		screen.setColor(sf::Color(138, 3, 3));

		screen.startFadeIn([this]()
			{
				if (new_record)
				{
					// if new record then show banner animation

					banner.setFallDuration(1);

					banner.setPauseDuration(2);

					banner.startAnimation("New Record",
						[this]()
						{
							// game over and new record music has finished so start background music

							bg_music.play();
						}
					);

					// play new record sound

					play_sound(NEW_RECORD);

					return;
				}

				// game over music has finished so start background music

				bg_music.play();
			}
		);
	}



	void Update(double dt)	// update this state
	{
		banner.xfinalUpdate(dt);

		// updating the button but the input is not used if some visual effects are being displayed

		auto mpos = bb::INPUT.pointer();

		auto enter_button_clicked = enter.is_clicked(
			mpos.x,
			mpos.y,
			bb::INPUT.isPressedM(sf::Mouse::Button::Left),
			bb::INPUT.isReleasedM(sf::Mouse::Button::Left)
		);

		if (screen.xfinal())
		{
			// xfinal changes the game state, so we return after it's executed

			return;
		}

		// ignoring inputs if some visual effects are being displayed

		if (screen.isActive() || banner.isActive())
		{
			return;
		}

		if (enter_button_clicked || bb::INPUT.isPressed(sf::Keyboard::Scan::Enter) || bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			play_sound(BUTTON);

			// fade out effect the screen goes black slowly

			screen.setColor(sf::Color::Black);

			screen.startFadeOut([this]() { sm.change_to(initial); });
		}
	}



	void Render()	// render this state
	{
		bb::WINDOW.draw(board);

		bb::WINDOW.draw(text);

		bb::WINDOW.draw(score_text);

		enter.Render();

		banner.render();

		// adding shadow to banner text

		bb::shadow(banner.text(), sf::Color(34, 32, 52), sf::Vector2f{ 1.5, 1.5 }, sf::Vector2f{ 1.5, 1 }, sf::Vector2f{ 0, 1.5 });

		bb::WINDOW.draw(banner.text());

		screen.render();
	}



	void Exit()		// destroy this state
	{
		// stopping all the threads before we exit this state

		screen.stop();

		banner.stop();
	}
} game_over;
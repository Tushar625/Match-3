#pragma once


class highest_score_state : public bb::BASE_STATE
{
	/*
		used to create fade in/out effect
	*/

	bb::ScreenFade screen;	// creates the fade in and fade out effect


	// score board

	RoundedRectangle board;


	// texts

	sf::Text text, score_text;


	// the button

	bb::STR_BUTTON enter;


public:



	highest_score_state() :
		enter(
			medium_text,
			"Enter",
			sf::Color(99, 155, 255),
			sf::Color::Cyan,
			sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + ((10 + (MEDIUM_FONT_SIZE + 10) * 4) * 1.5) / 2 - 16),
			bb::BOTTOM_CENTER
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

		text.setString("Your Highest Score");

		bb::textCenterOrigin(text);

		text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 - MEDIUM_FONT_SIZE / 2 - 5));

		// the score text

		score_text = medium_text;

		score_text.setFillColor(sf::Color(99, 155, 255));
	}



	void init(int score)
	{
		score_text.setString(std::to_string(score));

		bb::textCenterOrigin(score_text);

		score_text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT / 2 + MEDIUM_FONT_SIZE / 2 + 5));
	}



	void Enter()	// initialze this state
	{
		screen.setColor(sf::Color::White);

		screen.startFadeIn();
	}



	void Update(double dt)	// update this state
	{
		auto mpos = bb::INPUT.pointer();

		auto mouse_clicked = enter.is_clicked(
			mpos.x,
			mpos.y,
			bb::INPUT.isPressedM(sf::Mouse::Button::Left),
			bb::INPUT.isReleasedM(sf::Mouse::Button::Left)
		);

		if (screen.xfinal())
		{
			return;
		}

		if (screen.isActive())
		{
			return;
		}

		if (mouse_clicked || bb::INPUT.isPressed(sf::Keyboard::Scan::Enter) || bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			play_sound(BUTTON);

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

		screen.render();
	}



	void Exit()		// destroy this state
	{
		screen.stop();
	}
} highest_score;
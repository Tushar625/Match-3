#pragma once


class banner_class
{
	sf::Text text;
	sf::Vector2f pos;
	sf::RectangleShape board;


public:


	float y_pos;


	banner_class()
	{
		// initialize the banner with large text and a rectangle shape as wide as the screen

		text = large_text;

		text.setFillColor(sf::Color(255, 255, 255));


		board.setSize(sf::Vector2f(VIRTUAL_WIDTH, BRICK_HEIGHT * 2));

		board.setFillColor(sf::Color(95, 205, 228, 200));


		reset();	// place the banner at the top of the screen
	}


	// set the y position of the banner, top of the screen

	void reset()
	{
		y_pos = -board.getSize().y;
	}


	void set_text(const std::string& str)
	{
		// setting new text string and set the origin at the center

		text.setString(str);

		bb::setCenterOrigin(text);
	}


	sf::Vector2f get_size() const
	{
		return board.getSize();
	}


	void render()
	{
		/*
			origin of the board is at the top left corner, but the origin of the
			text is at its center
		*/

		board.setPosition(sf::Vector2f(0, y_pos));

		bb::WINDOW.draw(board);

		text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, y_pos + board.getSize().y / 2));

		bb::WINDOW.draw(text);
	}
};
#pragma once


/*class banner_class
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


		board.setSize(sf::Vector2f(VIRTUAL_WIDTH, BRICK_HEIGHT * 1.5));

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
		/

		board.setPosition(sf::Vector2f(0, y_pos));

		bb::WINDOW.draw(board);

		text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, y_pos + board.getSize().y / 2));

		bb::WINDOW.draw(text);
	}
};*/



class banner_class
{
	sf::Text text;
	sf::Vector2f pos;
	sf::RectangleShape board;
	float y_pos;

	TWEENER tween;
	DELAY_TIMER after;


	bool banner_active;	// indicates if the banner is active or not


public:


	banner_class() : banner_active(false)
	{
		// initialize the banner with large text and a rectangle shape as wide as the screen

		text = large_text;

		text.setFillColor(sf::Color(255, 255, 255));


		board.setSize(sf::Vector2f(VIRTUAL_WIDTH, BRICK_HEIGHT * 1.5));

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


	void start(const std::string& str, std::function<void(void)> final_func)
	{
		banner_active = true;

		reset();	// banner placed top the screen

		set_text(str);

		tween.start(	// banner comes down to middle of the screen
			.25,
			twn(y_pos, float(VIRTUAL_HEIGHT / 2 - get_size().y / 2)),
			[this, final_func](double dt)
			{
				after.start(	// banner stays at the middle of the screen for 1 second
					1,
					[this, final_func](double dt)
					{
						tween.start(	// banner comes down to bottom of the screen
							.25,
							twn(y_pos, float(VIRTUAL_HEIGHT)),
							[this, final_func](double dt)
							{
								banner_active = false;

								// execite the final function after banner is down

								final_func();
							}
						);
					}
				);
			}
		);
	}


	bool xfinal(double dt)
	{
		if (banner_active)
		{
			after.update(dt);

			return tween.xfinal() && !banner_active;
		}
	}


	bool is_active() const noexcept
	{
		return banner_active;
	}


	void render()
	{
		if (!banner_active)
		{
			return;	// no need to render if the banner is not active
		}

		tween.lock();

		/*
			origin of the board is at the top left corner, but the origin of the
			text is at its center
		*/

		board.setPosition(sf::Vector2f(0, y_pos));

		bb::WINDOW.draw(board);

		text.setPosition(sf::Vector2f(VIRTUAL_WIDTH / 2, y_pos + board.getSize().y / 2));

		bb::WINDOW.draw(text);

		tween.unlock();
	}
};
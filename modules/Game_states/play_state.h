#pragma once

class play_state : public bb::BASE_STATE
{


	const sf::Vector2f offset;



	// pointer data

	/*
		points to a brick in the grid, it is used to select a brick in
		the grid for swapping
	*/

	int point;	// index of the brick pointed by pointer

	RoundedRectangle pointer;	// to draw the pointer on the screen



	// selection data

	/*
		a pointed brick can be selected for swapping with another brick
	*/

	int selected;	// index of the selected brick, -1 if no brick is selected

	RoundedRectangle selecter;	// to draw the selecter on the screen, when a brick is selected



	// tweener

	TWEENER tween;



public:



	play_state() : offset(sf::Vector2f(128, 16)), point(0), selected(-1)	// initializing the pointer and selecter
	{
		// preparing the pointer, an empty rounded rectangle

		pointer.setPosition(offset);	// setting the pointer on the frst brick

		pointer.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		pointer.setFillColor(sf::Color::Transparent);

		pointer.setOutlineColor(sf::Color::Red);

		pointer.setOutlineThickness(3);

		pointer.setRadius(6);

		pointer.setCornerPointCount(12);

		// preparing the selecter, a translucent rounded rectangle

		selecter.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		selecter.setFillColor(sf::Color{ 255, 255, 255, 100 });

		selecter.setRadius(6);

		selecter.setCornerPointCount(12);
	}



private:



	// setting the position of pointer and selector according to the offset

	void set_pointer_pos(const sf::Vector2f& pos)
	{
		pointer.setPosition(pos + offset);
	}

	void set_selecter_pos(const sf::Vector2f& pos)
	{
		selecter.setPosition(pos + offset);
	}



	void Enter() override
	{
		//offset = sf::Vector2f(128, 16);
	}



	void Update(double dt) override
	{
		// don't allow any input, when the tween thread is running

		tween.xfinal();

		if (tween.is_running())
			return;


		// pointer up if pointer is not on the first row

		if (point >= GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Up))
		{
			point -= GRID_WIDTH;

			set_pointer_pos(board[point].pos);
		}

		// pointer down if pointer is not on the last row

		if (point <= (GRID_WIDTH * (GRID_HEIGHT - 1) - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;

			set_pointer_pos(board[point].pos);
		}

		// pointer left if pointer is not on the first column

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;

			set_pointer_pos(board[point].pos);
		}

		// pointer right if pointer is not on the last column

		if (point != (GRID_WIDTH * GRID_HEIGHT - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;

			set_pointer_pos(board[point].pos);
		}


		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter))
		{
			if (selected == -1)
			{
				// nothing is selected currently so select the pointed brick

				set_selecter_pos(board[point].pos);

				selected = point;
			}
			else if (selected == point)
			{
				selected = -1;	// deselect the selected brick
			}
			else if (adj_brick(selected, point))
			{
				// swapped the selected brick with the pointed brick

				// swap bricks in brickmap vector

				auto temp = board[point];

				board[point] = board[selected];

				board[selected] = temp;

				// swaping brickmap indices

				board[point].index = point;

				board[selected].index = selected;

				// swap positions of the bricks on screen with tween animation

				auto& pos_p = board[point].pos;

				auto& pos_s = board[selected].pos;

				tween.start(.2,
					twn_list(
						twn(pos_p.x, pos_s.x),
						twn(pos_p.y, pos_s.y),
						twn(pos_s.x, pos_p.x),
						twn(pos_s.y, pos_p.y)
					),
					[this](double dt)
					{
						calculate_match();
					}
				);

				selected = -1;	// deselect the selected brick
			}
		}
	}



	// check if the selected brick is adjacent to the pointed brick

	bool adj_brick(int selected, int pointed)
	{
		return ((selected / GRID_WIDTH == pointed / GRID_WIDTH) && std::abs(selected - pointed) == 1) || std::abs(selected - pointed) == GRID_WIDTH;
	}



	void calculate_match()
	{
		auto matches = board.find_matches();

		if (matches.size())
		{
			// matches are found

			board.remove_matches(matches);

			auto twn_vector = board.fill_matches();

			tween.start(.5,
				twn_vector.get_twn(),
				[this](double dt)
				{
					calculate_match();
				}
			);
		}
	}



	void Render() override
	{
		// safely access the data being tweened using lock() and unlock()

		tween.lock();

		board.render(offset);

		tween.unlock();

		// always draw the pointer on the selected brick

		bb::WINDOW.draw(pointer);

		// if a brick is selected, draw the selecter on it

		if (selected > -1)
		{
			bb::WINDOW.draw(selecter);
		}
	}



	void Exit() override
	{}
} play;
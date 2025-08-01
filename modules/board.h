#pragma once


/*
	this class is used to manage an 8 x 8 grid of bricks, allowing for
	selection, rendering, and swapping of the bricks in the game.
*/

class board_class
{

	std::vector<brick_struct> brick_map;	// vector to store the brickmap

	sf::Vector2f offset;


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



	board_class() : offset(sf::Vector2f(0, 0)), point(0), selected(-1)	// initializing the pointer and selecter
	{
		// preparing the pointer, an empty rounded rectangle

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


	// setting the position of pointer and selector according to the offset

	void set_pointer_pos(const sf::Vector2f& pos)
	{
		pointer.setPosition(pos + offset);
	}

	void set_selecter_pos(const sf::Vector2f& pos)
	{
		selecter.setPosition(pos + offset);
	}


	/*
		generate_brickmap() creates a grid of bricks with random colors and types.

		It fills the bmap vector with brick_struct objects, each representing a
		brick in the grid.

		offset is used to place the grid at a certain position on the screen
	*/

	void generate_brickmap(const sf::Vector2f& offset)
	{
		this->offset = offset;

		int x, y;

		// preparing the brickmap vector

		brick_map.clear();

		brick_map.reserve(GRID_WIDTH * GRID_HEIGHT);

		// creating the grid of bricks

		y = 0;

		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			// a row of bricks

			x = 0;

			for (int j = 0; j < GRID_WIDTH; j++)
			{
				// placing each brick

				brick_map.push_back(
					brick_struct(
						brick_map.size(),
						rand() % BRICK_COLORS,
						rand() % BRICK_TYPES,
						sf::Vector2f(x, y)
					)
				);

				//std::cout << "(" << brick_map.back().color << ", " << brick_map.back().type << ")";

				x += BRICK_WIDTH;	// placing NEXT brick
			}

			//std::cout << "\n";

			y += BRICK_HEIGHT;	// GOING TO NEXT line of brick
		}

		// setting the pointer on the frst brick

		set_pointer_pos(brick_map[0].pos);

		if (find_matches().size())
		{
			generate_brickmap(offset);
		}
	}

	void update()
	{
		// don't allow any input, when the tween thread is running

		tween.xfinal();

		if (tween.is_running())
			return;


		// pointer up if pointer is not on the first row

		if (point >= GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Up))
		{
			point -= GRID_WIDTH;

			set_pointer_pos(brick_map[point].pos);
		}

		// pointer down if pointer is not on the last row

		if (point <= (GRID_WIDTH * (GRID_HEIGHT - 1) - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;

			set_pointer_pos(brick_map[point].pos);
		}

		// pointer left if pointer is not on the first column

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;

			set_pointer_pos(brick_map[point].pos);
		}

		// pointer right if pointer is not on the last column

		if (point != (GRID_WIDTH * GRID_HEIGHT - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;

			set_pointer_pos(brick_map[point].pos);
		}


		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter))
		{
			if (selected == -1)
			{
				// nothing is selected currently so select the pointed brick

				set_selecter_pos(brick_map[point].pos);

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

				auto temp = brick_map[point];

				brick_map[point] = brick_map[selected];

				brick_map[selected] = temp;

				// swaping brickmap indices

				brick_map[point].index = point;

				brick_map[selected].index = selected;

				// swap positions of the bricks on screen with tween animation

				auto& pos_p = brick_map[point].pos;

				auto& pos_s = brick_map[selected].pos;

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
		auto matches = find_matches();

		if (matches.size())
		{
			// matches are found

			remove_matches(matches);

			auto twn_vector = fill_matches();

			tween.start(.5,
				twn_vector.get_twn(),
				[this](double dt)
				{
					calculate_match();
				}
			);
		}
	}


	std::vector<std::vector<brick_struct>> find_matches()
	{
		std::vector<std::vector<brick_struct>> matches;



		// horizontal matching

		for (int y = 0; y < GRID_HEIGHT; y++)
		{
			auto match_color = brick_map[y * GRID_WIDTH + 0].color;

			std::vector<brick_struct> match;

			match.push_back(brick_map[y * GRID_WIDTH + 0]);

			//std::cout << "\nRow " << y;

			// checking a row

			for (int x = 1; x < GRID_WIDTH; x++)	// we already accounted for first brick, so starting from index 1
			{
				auto& curr_brick = brick_map[y * GRID_WIDTH + x];

				//std::cout << "\ncurrent color: " << match_color << " Brick color: " << curr_brick.color;

				if (match_color == curr_brick.color)
				{
					// color of current brick matches the current match color

					match.push_back(curr_brick);
				}
				else
				{
					// color of current brick doesn't match the current match color

					if (match.size() >= MIN_MATCH_NUM)
					{
						// match size is more than minimum match threshold so, we keep the match

						matches.push_back(match);
					}

					// the match, if found, is listed so clear the match vector to prepare for next match

					match.clear();

					if (GRID_WIDTH - x < MIN_MATCH_NUM)
					{
						// if there are 2 or less bricks left in the row, no matches can be formed, break the loop

						break;
					}

					// prepare to take new matchs

					match.push_back(curr_brick);

					match_color = curr_brick.color;
				}

				//std::cout << "\nSize of match vector " << match.size();
			}

			/*
				if we have a nonempty match vector here, that means the loop has ended
				naturally and there can be a match to collect
			*/

			if (match.size() >= MIN_MATCH_NUM)
			{
				// match size is more than minimum match threshold so, we keep the match

				matches.push_back(match);
			}
		}


		//std::cout << "\n changing direction";


		// vertical matching

		for (int x = 0; x < GRID_WIDTH; x++)
		{
			auto match_color = brick_map[0 * GRID_WIDTH + x].color;

			std::vector<brick_struct> match;

			match.push_back(brick_map[0 * GRID_WIDTH + x]);

			// checking a column

			for (int y = 1; y < GRID_HEIGHT; y++)	// we already accounted for first brick, so starting from index 1
			{
				auto& curr_brick = brick_map[y * GRID_WIDTH + x];

				if (match_color == curr_brick.color)
				{
					// color of current brick matches the current match color

					match.push_back(curr_brick);
				}
				else
				{
					// color of current brick doesn't match the current match color

					if (match.size() >= MIN_MATCH_NUM)
					{
						// match size is more than minimum match threshold so, we keep the match

						matches.push_back(match);
					}

					// the match, if found, is listed so clear the match vector to prepare for next match

					match.clear();

					if (GRID_HEIGHT - y < MIN_MATCH_NUM)
					{
						// if there are 2 or less bricks left in the column, no matches can be formed, break the loop

						break;
					}

					// prepare to take new matchs

					match.push_back(curr_brick);

					match_color = curr_brick.color;
				}
			}

			/*
				if we have a nonempty match vector here, that means the loop has ended
				naturally and there can be a match to collect
			*/

			if (match.size() >= MIN_MATCH_NUM)
			{
				// match size is more than minimum match threshold so, we keep the match

				matches.push_back(match);
			}
		}



		//std::cout << "\n<*>matches size: " << matches.size();

		return matches;
	}


	bool is_space_grid(int x, int y) const noexcept
	{
		return brick_map[y * GRID_WIDTH + x].pos == sf::Vector2f(-100, -100);
	}

	bool not_space_grid(int x, int y) const noexcept
	{
		return brick_map[y * GRID_WIDTH + x].pos != sf::Vector2f(-100, -100);
	}

	void empty_grid(int x, int y) noexcept
	{
		brick_map[y * GRID_WIDTH + x].pos = sf::Vector2f(-100, -100);
	}

	void empty_grid(int index) noexcept
	{
		brick_map[index].pos = sf::Vector2f(-100, -100);
	}


	void remove_matches(const std::vector<std::vector<brick_struct>>& matches)
	{
		//std::cout << "matches size: " << matches.size();

		for (const auto& match : matches)
		{
			for (const auto& brick : match)
			{
				brick_map[brick.index].pos = sf::Vector2f(-100, -100);
			}
		}
	}


	MULTI_TWN<float> fill_matches()
	{
		MULTI_TWN<float> twn_vector;



		// dropping bricks to fill the spaces in the grid

		for(int x = 0; x < GRID_WIDTH; x++)
		{
			int y = GRID_HEIGHT - 1;


			// find the first empty space in the column starting from bottom

			for (; y >= 0 && brick_map[y * GRID_WIDTH + x].pos != sf::Vector2f(-100, -100); y--);

			int space_y = y;	// y of the first empty space


			// now dropping bricks to fillup the space

			y--;	// starting from the grid on top of space grid

			for (; y >= 0; y--)
			{
				auto& curr_grid = brick_map[y * GRID_WIDTH + x];

				// trying to fill the space grid at space_y

				if (curr_grid.pos != sf::Vector2f(-100, -100))
				{
					// current grid is not a space, so we can fill the space with this grid

					auto& space_grid = brick_map[space_y * GRID_WIDTH + x];

					// brick in the current grid will be placed at the space grid so,

					// copy all data from the current grid to the space grid

					space_grid = curr_grid;

					// update the index of the space grid

					space_grid.index = space_y * GRID_WIDTH + x;

					// tween the position of the space grid, from current position to the position of the space

					twn_vector.set_twn(&space_grid.pos.y, space_y * BRICK_HEIGHT);

					// empty current grid

					curr_grid.pos = sf::Vector2f(-100, -100);

					// the grid on top of former space grid is now a space so we decrement space_y

					space_y--;
				}
			}
		}



		// adding new bricks

		for (int x = 0; x < GRID_WIDTH; x++)
		{
			int y = GRID_HEIGHT - 1;


			// find the first empty space in the column starting from bottom

			for (; y >= 0 && brick_map[y * GRID_WIDTH + x].pos != sf::Vector2f(-100, -100); y--);

			int y_start = -BRICK_HEIGHT;	// y of the first brick to fall


			// now we fillup the space from the bottom

			/*
				notice "y_start -= BRICK_HEIGHT", we are placing falling bricks on top of
				eachother, to create a nice cascading effect when they falls
			*/

			for(; y >=0; y--, y_start -= BRICK_HEIGHT)
			{
				auto& curr_grid = brick_map[y * GRID_WIDTH + x];

				curr_grid.index = y * GRID_WIDTH + x;
				
				curr_grid.color = rand() % BRICK_COLORS;
				
				curr_grid.type = rand() % BRICK_TYPES;
				
				curr_grid.pos = sf::Vector2f(x * BRICK_WIDTH, y_start);
				
				twn_vector.set_twn(&curr_grid.pos.y, y * BRICK_HEIGHT);
			}
		}



		return twn_vector;
	}


	void render()
	{
		// safely access the data being tweened using lock() and unlock()

		tween.lock();

		for (const auto& brick : brick_map)
		{
			brick.render(offset);
		}

		tween.unlock();

		// always draw the pointer on the selected brick

		bb::WINDOW.draw(pointer);

		// if a brick is selected, draw the selecter on it

		if (selected > -1)
		{
			bb::WINDOW.draw(selecter);
		}
	}
} board;
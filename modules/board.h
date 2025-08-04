#pragma once


/*
	this class is used to manage an 8 x 8 grid of bricks, allowing for
	selection, rendering, and swapping of the bricks in the game.
*/

class board_class
{

	std::array<brick_struct, GRID_WIDTH * GRID_HEIGHT> brick_map;	// vector to store the brickmap


public:


	board_class()
	{
		if (!bb::load_local_appdata("candycrush_brick_map.bin", *this))
		{
			generate_brickmap();
		}
	}


	~board_class()
	{
		bb::store_local_appdata("candycrush_brick_map.bin", *this);
	}


	brick_struct& operator[](int index)
	{
		return brick_map[index];
	}


	/*
		generate_brickmap() creates a grid of bricks with random colors and types.

		It fills the bmap vector with brick_struct objects, each representing a
		brick in the grid.

		offset is used to place the grid at a certain position on the screen
	*/

	void generate_brickmap()
	{
		int x, y, brick_count;		

		// creating the grid of bricks

		brick_count = 0;

		y = 0;

		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			// a row of bricks

			x = 0;

			for (int j = 0; j < GRID_WIDTH; j++)
			{
				// placing each brick

				brick_map[brick_count] = brick_struct(
					brick_count,
					rand() % BRICK_COLORS,
					rand() % BRICK_TYPES,
					sf::Vector2f(x, y)
				);

				//std::cout << "(" << brick_map[brick_count].color << ", " << brick_map[brick_count].type << ")";

				brick_count++;

				x += BRICK_WIDTH;	// placing NEXT brick
			}

			//std::cout << "\n";

			y += BRICK_HEIGHT;	// GOING TO NEXT line of brick
		}

		if (find_matches().size())
		{
			generate_brickmap();
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
				empty_grid(brick.index);
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

			for (; y >= 0 && not_space_grid(x, y); y--);

			int space_y = y;	// y of the first empty space


			// now dropping bricks to fillup the space

			y--;	// starting from the grid on top of space grid

			for (; y >= 0; y--)
			{
				// trying to fill the space grid at space_y

				if (not_space_grid(x, y))
				{
					// current grid is not a space, so we can fill the space with this grid

					auto& curr_grid = brick_map[y * GRID_WIDTH + x];

					auto& space_grid = brick_map[space_y * GRID_WIDTH + x];

					// brick in the current grid will be placed at the space grid so,

					// copy all data from the current grid to the space grid

					space_grid = curr_grid;

					// empty current grid

					empty_grid(x, y);

					// update the index of the space grid

					space_grid.index = space_y * GRID_WIDTH + x;

					// tween the position of the space grid, from current position to the position of the space

					twn_vector.set_twn(&space_grid.pos.y, space_y * BRICK_HEIGHT);

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

			for (; y >= 0 && not_space_grid(x, y); y--);

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


	void render(const sf::Vector2f& offset)
	{
		for (const auto& brick : brick_map)
		{
			brick.render(offset);
		}
	}
} board;
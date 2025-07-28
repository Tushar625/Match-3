#pragma once


/*
	this class is used to manage an 8 x 8 grid of bricks, allowing for
	selection, rendering, and swapping of the bricks in the game.
*/

class board_class
{

	std::vector<brick_struct> brick_map;	// vector to store the brickmap


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



	board_class() : point(0), selected(-1)	// initializing the pointer and selecter
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



	/*
		generate_brickmap() creates a grid of bricks with random colors and types.

		It fills the bmap vector with brick_struct objects, each representing a
		brick in the grid.

		offset is used to place the grid at a certain position on the screen
	*/

	void generate_brickmap(sf::Vector2f offset)
	{
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
						rand() % BRICK_COLORS,
						rand() % BRICK_TYPES,
						sf::Vector2f(x, y) + offset
					)
				);

				x += BRICK_WIDTH;	// placing NEXT brick
			}

			y += BRICK_HEIGHT;	// GOING TO NEXT line of brick
		}

		// setting the pointer on the frst brick

		pointer.setPosition(brick_map[0].pos);
	}

	void update()
	{
		// don't allow any input, when the tween thread is running

		if (tween.is_running())
			return;


		// pointer up if pointer is not on the first row

		if (point >= GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Up))
		{
			point -= GRID_WIDTH;

			pointer.setPosition(brick_map[point].pos);
		}

		// pointer down if pointer is not on the last row

		if (point <= (GRID_WIDTH * (GRID_HEIGHT - 1) - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;

			pointer.setPosition(brick_map[point].pos);
		}

		// pointer left if pointer is not on the first column

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;

			pointer.setPosition(brick_map[point].pos);
		}

		// pointer right if pointer is not on the last column

		if (point != (GRID_WIDTH * GRID_HEIGHT - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;

			pointer.setPosition(brick_map[point].pos);
		}


		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter))
		{
			if (selected == -1)
			{
				// nothing is selected currently so select the pointed brick

				selecter.setPosition(brick_map[point].pos);

				selected = point;
			}
			else if (adj_brick(selected, point))
			{
				// swapped the selected brick with the pointed brick

				// swap bricks in brickmap vector

				auto temp = brick_map[point];

				brick_map[point] = brick_map[selected];

				brick_map[selected] = temp;

				// swap positions of the bricks on screen with tween animation

				auto& pos_p = brick_map[point].pos;

				auto& pos_s = brick_map[selected].pos;

				tween.start(.2,
					twn_list(
						twn(pos_p.x, pos_s.x),
						twn(pos_p.y, pos_s.y),
						twn(pos_s.x, pos_p.x),
						twn(pos_s.y, pos_p.y)
					)

				);

				selected = -1;	// deselect the selected brick
			}
		}
	}

	bool adj_brick(int selected, int pointed)
	{
		return std::abs(selected - pointed) == 1 || std::abs(selected - pointed) == GRID_WIDTH;
	}

	void render()
	{
		// safely access the data being tweened using lock() and unlock()

		tween.lock();

		for (const auto& brick : brick_map)
		{
			brick.render();
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
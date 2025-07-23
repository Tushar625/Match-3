#pragma once


/*
	this class is used to manage an 8 x 8 grid of bricks, allowing for
	selection, rendering, and swapping of the bricks in the game.
*/

class brickmap_class
{


	/*
		brick_struct is a structure that represents a single brick in the grid.
		it contains:
			-color and type of the brick,
			-its position on the screen

		color and type are used to determine the index (color * BRICK_TYPES +
		type) of the brick sprite from brick_sprite vector, which contains all
		the brick sprites

			-render() method is used to draw the brick at its position on the
			screen.
	*/
	
	struct brick_struct
	{
		int color, type;

		sf::Vector2f pos;

		void render() const noexcept
		{
			auto& brick = brick_sprite[color * BRICK_TYPES + type];
			
			brick.setPosition(pos);

			bb::WINDOW.draw(brick);
		}
	};

	std::vector<brick_struct> bmap;	// vector to store the brickmap



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



	brickmap_class() : point(0), selected(-1)	// initializing the pointer and selecter
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

		selecter.setFillColor(sf::Color{255, 255, 255, 100});

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

		bmap.clear();

		bmap.reserve(GRID_WIDTH * GRID_HEIGHT);

		// creating the grid of bricks

		y = 0;

		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			// a row of bricks

			x = 0;

			for (int j = 0; j < GRID_WIDTH; j++)
			{
				// placing each brick

				bmap.push_back(
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

		pointer.setPosition(bmap[0].pos);
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

			pointer.setPosition(bmap[point].pos);
		}

		// pointer down if pointer is not on the last row

		if (point <= (GRID_WIDTH * (GRID_HEIGHT - 1) - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;

			pointer.setPosition(bmap[point].pos);
		}

		// pointer left if pointer is not on the first column

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;

			pointer.setPosition(bmap[point].pos);
		}

		// pointer right if pointer is not on the last column

		if (point != (GRID_WIDTH * GRID_HEIGHT - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;

			pointer.setPosition(bmap[point].pos);
		}


		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter))
		{
			if (selected == -1)
			{
				// nothing is selected currently so select the pointed brick

				selecter.setPosition(bmap[point].pos);

				selected = point;
			}
			else
			{
				// swapped the selected brick with the pointed brick

				// swap bricks in brickmap vector

				auto temp = bmap[point];

				bmap[point] = bmap[selected];

				bmap[selected] = temp;

				// swap positions of the bricks on screen with tween animation

				auto& pos_p = bmap[point].pos;

				auto& pos_s = bmap[selected].pos;

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

	void render()
	{
		// safely access the data being tweened using lock() and unlock()

		tween.lock();

		for (const auto& brick : bmap)
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
} brickmap;
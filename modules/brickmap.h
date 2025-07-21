#pragma once


// 8 x 8 matrix each cell is a brick

class brickmap_class
{
	std::vector<sf::Sprite> bmap;

	int point;

	RoundedRectangle pointer;


public:

	brickmap_class() : bmap(GRID_WIDTH * GRID_HEIGHT), point(0)
	{
		// preparing the pointer

		pointer.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		pointer.setFillColor(sf::Color::Transparent);

		pointer.setOutlineColor(sf::Color::Red);

		pointer.setOutlineThickness(3);

		pointer.setRadius(6);

		pointer.setCornerPointCount(12);
	}

	void generate_brickmap(sf::Vector2f offset)
	{
		int x, y;


		// creating the grid of bricks

		y = 0;

		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			// a row of bricks

			x = 0;

			for (int j = 0; j < GRID_WIDTH; j++)
			{
				// each brick

				bmap[i * GRID_WIDTH + j] = brick_sprite[rand() % brick_sprite.size()][rand() % brick_sprite[0].size()];

				bmap[i * GRID_WIDTH + j].setPosition(sf::Vector2f(x, y) + offset);

				x += BRICK_WIDTH;	// placing NEXT brick
			}

			y += BRICK_HEIGHT;	// GOING TO NEXT line of brick
		}
	}

	void update()
	{
		// pointer up

		if (point >= GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Up))
		{
			point -= GRID_WIDTH;
		}

		// pointer down

		if (point <= GRID_WIDTH * GRID_HEIGHT - GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;
		}

		// pointer left

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;
		}

		// pointer right

		if (point != GRID_WIDTH * GRID_HEIGHT && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;
		}
	}

	void render()
	{
		for (const auto& brick : bmap)
		{
			bb::WINDOW.draw(brick);
		}

		auto pos = bmap[point].getPosition();

		pointer.setPosition(pos);

		bb::WINDOW.draw(pointer);
	}
} brickmap;
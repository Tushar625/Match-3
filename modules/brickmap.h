#pragma once


// 8 x 8 matrix each cell is a brick

class brickmap_class
{
	std::vector<sf::Sprite> bmap;

	// pointer data

	int point;

	RoundedRectangle pointer;

	// selection data

	int selected;

	RoundedRectangle selecter;

	// tweening data

	TWEENER tween;

	sf::Vector2f pos_p, pos_s;

	int tween_selected;


public:

	brickmap_class() : bmap(GRID_WIDTH * GRID_HEIGHT), point(0), selected(-1)
	{
		// preparing the pointer

		pointer.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		pointer.setFillColor(sf::Color::Transparent);

		pointer.setOutlineColor(sf::Color::Red);

		pointer.setOutlineThickness(3);

		pointer.setRadius(6);

		pointer.setCornerPointCount(12);

		// preparing the selecter

		selecter.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		selecter.setFillColor(sf::Color{255, 255, 255, 100});

		selecter.setRadius(6);

		selecter.setCornerPointCount(12);
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

		// setting the pointer

		pointer.setPosition(bmap[0].getPosition());
	}

	void update()
	{
		if (tween.is_running())
			return;


		// pointer up

		if (point >= GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Up))
		{
			point -= GRID_WIDTH;

			pointer.setPosition(bmap[point].getPosition());
		}

		// pointer down

		if (point <= (GRID_WIDTH * (GRID_HEIGHT - 1) - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;

			pointer.setPosition(bmap[point].getPosition());
		}

		// pointer left

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;

			pointer.setPosition(bmap[point].getPosition());
		}

		// pointer right

		if (point != (GRID_WIDTH * GRID_HEIGHT - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;

			pointer.setPosition(bmap[point].getPosition());
		}


		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter))
		{
			if (selected == -1)
			{
				// nothing is selected currently

				selecter.setPosition(bmap[point].getPosition());

				selected = point;
			}
			else
			{
				// a brick is already selected

				// swap bricks

				auto temp = bmap[point];

				bmap[point] = bmap[selected];

				bmap[selected] = temp;

				// swap positions with tween

				pos_p = bmap[point].getPosition();

				pos_s = bmap[selected].getPosition();

				tween.start(.2,
					twn_list(
						twn(pos_p.x, pos_s.x),
						twn(pos_p.y, pos_s.y),
						twn(pos_s.x, pos_p.x),
						twn(pos_s.y, pos_p.y)
					)

				);

				tween_selected = selected;

				/*auto temp_pos = bmap[point].getPosition();

				bmap[point].setPosition(bmap[selected].getPosition());

				bmap[selected].setPosition(temp_pos);*/

				selected = -1;
			}
		}
	}

	void render()
	{
		if (tween.is_running())
		{
			tween.lock();

			bmap[tween_selected].setPosition(pos_s);

			bmap[point].setPosition(pos_p);

			tween.unlock();
		}

		for (const auto& brick : bmap)
		{
			bb::WINDOW.draw(brick);
		}

		bb::WINDOW.draw(pointer);

		if (selected > -1)
		{
			bb::WINDOW.draw(selecter);
		}
	}
} brickmap;
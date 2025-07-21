#pragma once



std::vector<std::vector<sf::Sprite>> get_bricks(const sf::Texture& texture)
{
	/*
		Initializes a 2D vector 'brick' with 18 rows and 6 columns.
		Each element is an sf::Sprite constructed with the given texture.
	*/

	std::vector<std::vector<sf::Sprite>> brick(18, std::vector<sf::Sprite>(6, sf::Sprite(texture)));

	int x, y;


	// collecting first 9 bricks

	y = 0;

	for (int i = 0; i < 9; i++)
	{
		// a row of bricks

		x = 0;

		for (int j = 0; j < 6; j++)
		{
			// each brick

			brick[i][j].setTextureRect(sf::IntRect(x, y, BRICK_WIDTH, BRICK_HEIGHT));

			x += BRICK_WIDTH;	// GOING TO NEXT brick
		}

		y += BRICK_HEIGHT;	// GOING TO NEXT line of brick
	}


	// collecting next 9 bricks

	y = 0;

	for (int i = 9; i < 18; i++)
	{
		// a row of bricks

		x = 192;

		for (int j = 0; j < 6; j++)
		{
			// each brick

			brick[i][j].setTextureRect(sf::IntRect(x, y, BRICK_WIDTH, BRICK_HEIGHT));

			x += BRICK_WIDTH;	// GOING TO NEXT brick
		}

		y += BRICK_HEIGHT;	// GOING TO NEXT line of brick
	}

	return brick;
}
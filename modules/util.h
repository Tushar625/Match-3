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



std::vector<sf::Sprite> generateSpriteVector(const sf::Texture& texture, unsigned int spriteWidth, unsigned int spriteHeight, sf::IntRect textureArea = sf::IntRect(-1, -1, 0, 0))
{

	if(textureArea.top < 0 || textureArea.left < 0 || textureArea.width <= 0 || textureArea.height <= 0)
	{
		// need to initialize textureArea

		textureArea = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
	}


	unsigned int gridWidth = textureArea.width / spriteWidth;

	unsigned int gridHeight = textureArea.height / spriteHeight;


	std::vector<sf::Sprite> sprites(gridWidth * gridHeight, sf::Sprite(texture));


	unsigned int spriteCount = 0;

	unsigned int textureY = textureArea.top;

	for (unsigned int gridY = 0; gridY < gridHeight; gridY++)
	{
		unsigned int textureX = textureArea.left;

		for (unsigned int gridX = 0; gridX < gridWidth; gridX++)
		{
			sprites[spriteCount++].setTextureRect(sf::IntRect(textureX, textureY, spriteWidth, spriteHeight));
			
			textureX += spriteWidth;
		}

		textureY += spriteHeight;
	}
    
	return sprites;
}
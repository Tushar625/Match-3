#pragma once



sf::Font font;

sf::Text small_text, medium_text, large_text;

std::vector<sf::Texture> texture{ TEXTURE_COUNT };

std::vector<std::vector<sf::Sprite>> brick_sprite;

sf::Sprite bg_sprite;



class ASSET_LOADER
{
	// all the assets are loaded before the game states are initialized

public:

	ASSET_LOADER()
	{
		

		if (!font.loadFromFile("font/font.ttf"))
		{
			std::cout << "can't load font\n";
		}

		font.setSmooth(false);

		small_text.setFont(font);	small_text.setCharacterSize(SMALL_FONT_SIZE);

		medium_text.setFont(font);	medium_text.setCharacterSize(MEDIUM_FONT_SIZE);

		large_text.setFont(font);	large_text.setCharacterSize(LARGE_FONT_SIZE);
		


		// loading the textures

		if (!texture[MAIN].loadFromFile("image/match3.png"))
		{
			std::cout << "can't load match3.png\n";
		}

		if (!texture[BACKGROUND].loadFromFile("image/background.png"))
		{
			std::cout << "can't load background.png\n";
		}



		// creating the sprites

		brick_sprite = get_bricks(texture[MAIN]);

		bg_sprite.setTexture(texture[BACKGROUND]);

	}
} obj;
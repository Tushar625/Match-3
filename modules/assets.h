#pragma once



// some assets to be used all over the game



sf::Font font;

sf::Text small_text, medium_text, large_text;

std::vector<sf::Texture> texture{ TEXTURE_COUNT };

std::vector<sf::Sprite> brick_sprite;	// list of all brick sprites

sf::Sprite bg_sprite;	// sprite for infinite scrolling background

std::vector<sf::SoundBuffer> sound_buffer{ SOUND_COUNT };

sf::Music bg_music;

bb::STATE_MACHINE sm;



/*
	constructor of this class loads and initializes all the assets before
	the game states are initialized

	this is also the first piece of code executed in this game
*/

class ASSET_LOADER
{

public:

	ASSET_LOADER()
	{

		srand(time(0));

		

		// loading the fonts

		if (!font.loadFromFile("font/font.ttf"))
		{
			std::cout << "can't load font\n";
		}

		font.setSmooth(false);



		// setting the texts

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



		// setting the sprites

		brick_sprite = bb::generateSpriteVector(texture[MAIN], BRICK_WIDTH, BRICK_HEIGHT);

		texture[BACKGROUND].setSmooth(false);

		bg_sprite.setTexture(texture[BACKGROUND]);

		// the area of the background texture we want to display

		bg_sprite.setTextureRect(sf::IntRect(20, 46, texture[BACKGROUND].getSize().x - 20, texture[BACKGROUND].getSize().y - 46));



		// loading the sounds

		if (!sound_buffer[POINTER].loadFromFile("sound/pointer.wav"))
		{
			std::cout << "can't load pointer.wav\n";
		}

		if (!sound_buffer[BUTTON].loadFromFile("sound/button.mp3"))
		{
			std::cout << "can't load button.mp3\n";
		}

		if (!sound_buffer[GAME_START].loadFromFile("sound/game-start.mp3"))
		{
			std::cout << "can't load game-start.mp3\n";
		}

		if (!sound_buffer[BRICK_SWAP].loadFromFile("sound/brick_swap.mp3"))
		{
			std::cout << "can't load brick_swap.mp3\n";
		}

		if (!sound_buffer[BRICK_BREAK].loadFromFile("sound/brick_breaking.mp3"))
		{
			std::cout << "can't load brick_breaking.mp3\n";
		}

		if (!sound_buffer[LEVEL_UP].loadFromFile("sound/level_up.mp3"))
		{
			std::cout << "can't load level_up.mp3\n";
		}

		if (!sound_buffer[GAME_OVER].loadFromFile("sound/game_over.mp3"))
		{
			std::cout << "can't load game_over.mp3\n";
		}

		if (!sound_buffer[NEW_RECORD].loadFromFile("sound/new_record.mp3"))
		{
			std::cout << "can't load new_record.mp3\n";
		}



		// opening the background music

		if (!bg_music.openFromFile("sound/bg_sound.mp3"))
		{
			std::cout << "can't load bg_sound.mp3\n";
		}
	}
} obj;
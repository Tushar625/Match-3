#pragma once



struct game_data_type
{
	int score;	// current score of the game
	int level;	// current level of the game
	int time;	// time left in seconds
	int goal;	// target score of the game

	game_data_type() : score(0), level(1), time(60), goal(1000)
	{
		bb::load_local_appdata("candycrush_game_data.bin", *this);
	}

	~game_data_type()
	{
		bb::store_local_appdata("candycrush_game_data.bin", *this);
	}
};
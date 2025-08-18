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

	void reset()
	{
		score = 0;

		level = 1;

		time = 60;

		goal = 1000;
	}

	~game_data_type()
	{
		bb::store_local_appdata("candycrush_game_data.bin", *this);
	}
};


struct highest_score_type
{
	int highest_score;	// current highest score of the game
	
	highest_score_type() : highest_score(0)
	{
		//bb::load_local_appdata("candycrush_highest_score_data.bin", *this);
	}

	bool update(int new_score)
	{
		if (highest_score < new_score)
		{
			highest_score = new_score;

			return true;
		}

		return false;
	}

	~highest_score_type()
	{
		//bb::store_local_appdata("candycrush_highest_score_data.bin", *this);
	}
};
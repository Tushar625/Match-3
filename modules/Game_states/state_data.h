#pragma once


// these classes are used by multiple game states to store and retrieve important state data


// game data used by play state

struct game_data_type
{
	int score;	// current score of the game
	int level;	// current level of the game
	int time;	// time left in seconds
	int goal;	// target score of the game

	game_data_type() : score(0), level(1), time(60), goal(1000)
	{
		// load saved game data if any

		bb::load_local_appdata("candycrush_game_data.bin", *this);
	}

	void reset()
	{
		// default values

		score = 0;

		level = 1;

		time = 60;

		goal = 1000;
	}

	~game_data_type()
	{
		// save game data

		bb::store_local_appdata("candycrush_game_data.bin", *this);
	}
};


// highest score data used by initial state

struct highest_score_type
{
	int highest_score;	// current highest score of the game
	
	highest_score_type() : highest_score(0)
	{
		// load saved highest score data if any
		
		//bb::load_local_appdata("candycrush_highest_score_data.bin", *this);
	}

	/*
		update highest score if new_score is greater than highest_score
		returns true if highest score is updated, otherwise false
	*/

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
		// save highest score data

		//bb::store_local_appdata("candycrush_highest_score_data.bin", *this);
	}
};
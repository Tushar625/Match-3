#pragma once

class score_board_class
{
	sf::Text level_text, score_text, goal_text, timer_text;

	sf::Vector2f level_pos, score_pos, goal_pos, timer_pos;

	RoundedRectangle board;


public:


	score_board_class(sf::Vector2f pos)
	{
		// rounded rectangle

		board.setPosition(pos);

		board.setSize(sf::Vector2f(150, 10 + (MEDIUM_FONT_SIZE + 10) * 4));

		board.setRadius(6);

		board.setCornerPointCount(12);

		board.setFillColor(sf::Color(63, 63, 116, 245));

		pos.x += board.getLocalBounds().width / 2;

		pos.y += 10 + MEDIUM_FONT_SIZE / 2;

		// texts

		level_text = medium_text;

		score_text = medium_text;

		goal_text = medium_text;

		timer_text = medium_text;

		level_text.setFillColor(sf::Color(99, 155, 255));

		score_text.setFillColor(sf::Color(99, 155, 255));

		goal_text.setFillColor(sf::Color(99, 155, 255));

		timer_text.setFillColor(sf::Color(99, 155, 255));

		text_setup(level_text, "Level: 1", pos);

		pos.y += MEDIUM_FONT_SIZE + 10;

		text_setup(score_text, "Score: 0", pos);
		
		pos.y += MEDIUM_FONT_SIZE + 10;
		
		text_setup(goal_text, "Goal: 0", pos);

		pos.y += MEDIUM_FONT_SIZE + 10;

		text_setup(timer_text, "Time: 60", pos);
	}

	void set_pos(sf::Vector2f pos)
	{
		level_pos.x = score_pos.x = goal_pos.x = timer_pos.x = pos.x;


	}


	void text_setup(sf::Text& text, const std::string& str, const sf::Vector2f& pos)
	{
		text.setString(str);

		bb::textCenterOrigin(text);

		text.setPosition(pos);
	}


	void render(int level, int score, int goal, int time)
	{
		static int prev_level = 1, prev_score = 0, prev_goal = 0, prev_time = 60;

		if(prev_level != level)
		{
			prev_level = level;

			text_setup(level_text, "Level: " + std::to_string(level), level_text.getPosition());
		}

		if(prev_score != score)
		{
			prev_score = score;

			text_setup(score_text, "Score: " + std::to_string(score), score_text.getPosition());
		}

		if(prev_goal != goal)
		{
			prev_goal = goal;

			text_setup(goal_text, "Goal: " + std::to_string(goal), goal_text.getPosition());
		}

		if(prev_time != time)
		{
			prev_time = time;

			text_setup(timer_text, "Time: " + std::to_string(time), timer_text.getPosition());
		}

		bb::WINDOW.draw(board);

		bb::WINDOW.draw(level_text);

		bb::WINDOW.draw(score_text);

		bb::WINDOW.draw(goal_text);

		bb::WINDOW.draw(timer_text);
	}
};
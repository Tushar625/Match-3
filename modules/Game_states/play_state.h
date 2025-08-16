#pragma once

class play_state : public bb::BASE_STATE
{
	game_data_type g_data;



	// the brickmap

	board_class board;



	// offset for the brick map

	const sf::Vector2f offset;



	// the score board

	score_board_class score_board;



	// pointer data

	/*
		points to a brick in the grid, it is used to select a brick in
		the grid for swapping
	*/

	int point;	// index of the brick pointed by pointer

	RoundedRectangle pointer;	// to draw the pointer on the screen

	bool pointer_color;	// decide the pointer color



	// selection data

	/*
		a pointed brick can be selected for swapping with another brick
	*/

	int selected;	// index of the selected brick, -1 if no brick is selected

	RoundedRectangle selecter;	// to draw the selecter on the screen, when a brick is selected



	/*
		used to create fade in/out effect
	*/

	ScreenFade screen;	// creates the fade in and fade out effect



	/*
		to show the level banner, like "Level 1"
	*/

	Banner banner;



	// tweener

	bb::TWEENER tween;



	// interval timer

	bb::INTERVAL_TIMER timer;	// decreases the timer of game data every second
	
	bb::INTERVAL_TIMER pointer_color_timer;	// to change the pointer color periodically, to make it blink



	// delay timer

	bb::DELAY_TIMER after;



public:



	play_state() :
		board(true),	// we save this board data
		score_board(sf::Vector2f(24, 24)),
		offset(sf::Vector2f(VIRTUAL_WIDTH - 272, 16)),
		point(0),
		pointer_color(false),
		selected(-1),	// initializing the pointer and selecter
		screen(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		banner(
			large_text,
			sf::Vector2f(VIRTUAL_WIDTH, BRICK_HEIGHT * 1.5),
			VIRTUAL_HEIGHT,
			sf::Color(255, 255, 255),
			sf::Color(95, 205, 228, 200)
		)
	{
		// preparing the pointer, an empty rounded rectangle

		pointer.setPosition(offset);	// setting the pointer on the frst brick

		pointer.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		pointer.setFillColor(sf::Color::Transparent);

		pointer.setOutlineThickness(4);

		pointer.setRadius(6);

		pointer.setCornerPointCount(12);

		// preparing the selecter, a translucent rounded rectangle

		selecter.setSize(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));

		selecter.setFillColor(sf::Color{ 255, 255, 255, 100 });

		selecter.setRadius(6);

		selecter.setCornerPointCount(12);
	}



private:



	// setting the position of pointer and selector according to the offset

	void set_pointer_pos(const sf::Vector2f& pos)
	{
		pointer.setPosition(pos + offset);
	}

	void set_selecter_pos(const sf::Vector2f& pos)
	{
		selecter.setPosition(pos + offset);
	}



public:



	void init(bool new_game)
	{
		// starting a new game

		g_data.score = 0;
		
		g_data.level = 1;
		
		g_data.time = 60;
		
		g_data.goal = 1000;

		// new brickmap

		board.generate_brickmap();

		point = 0;

		set_pointer_pos(board[point].pos);
		
		selected = -1;
	}



private:


	void level_banner()
	{
		banner.startAnimation("Level " + std::to_string(g_data.level), [this]()
			{
				// starting the game timer countdown after banner is down

				timer.start(1, [this](double dt) -> bool
					{
						if (g_data.time <= 0)
						{
							return false;	// stop the timer
						}

						g_data.time--;

						return true;	// continue the timer
					}
				);

				// pointer blinking

				pointer_color_timer.start(0.5, [this](double dt)
					{
						pointer_color = !pointer_color;

						return true;
					}
				);
			}
		);
	}



	void Enter() override
	{
		// play state emarges from light, the fade in effect

		screen.setColor(sf::Color::White);

		screen.startFadeIn([this]() {level_banner(); });
	}



	void Update(double dt) override
	{
		if (banner.xfinal(dt))
		{
			// returns true once after the banner display operation is finished

			return;
		}

		if (screen.xfinal())
		{
			// xfinal changes the game state, so we return after it's executed

			return;
		}

		if(tween.xfinal())
		{
			// final() of tween may change the state, in other cases there's no harm in return

			return;
		}

		/*
			don't allow any input or processing operations, when the tween thread is running
			or some other visual effects are being displayed
		*/

		if (tween.is_running() || screen.isActive() || banner.isActive())
			return;


		// goal check

		if (g_data.score > g_data.goal)
		{
			// goal reached so level is finished

			// stop the game timer and pointer color blinking

			timer.stop();

			pointer_color_timer.stop();

			// update the game data

			g_data.level++;	// increase level

			g_data.time += 60;	// resetting the timer

			g_data.goal += g_data.goal * 1.25 + 200;	// increase goal

			// display the level banner and start the game timer countdown and pointer color blinking

			level_banner();
		}


		// remaining time check

		timer.lock();

		if (g_data.time == 0)
		{
			// game over

			pointer_color_timer.stop();	// stop the pointer color blinking

			banner.startAnimation("Game Over", [this]()
				{
					/*
						after displaying the game over banner, start a fade out effect
						the screen goes black slowly
					*/

					screen.setColor(sf::Color::Black);

					//screen.setColor(sf::Color(217, 87, 99, 255));

					screen.startFadeOut([this]() { init(true); /* reset the game data*/ sm.change_to(initial); });
				}
			);

			return;
		}

		timer.unlock();



		// ===== taking inputs =====



		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			// pause the game

			// fade out effect the screen goes black slowly

			screen.setColor(sf::Color::Black);

			screen.startFadeOut([this]() { sm.change_to(initial); });

			return;
		}


		// pointer up if pointer is not on the first row

		if (point >= GRID_WIDTH && bb::INPUT.isPressed(sf::Keyboard::Scan::Up))
		{
			point -= GRID_WIDTH;

			set_pointer_pos(board[point].pos);
		}

		// pointer down if pointer is not on the last row

		if (point <= (GRID_WIDTH * (GRID_HEIGHT - 1) - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Down))
		{
			point += GRID_WIDTH;

			set_pointer_pos(board[point].pos);
		}

		// pointer left if pointer is not on the first column

		if (point != 0 && bb::INPUT.isPressed(sf::Keyboard::Scan::Left))
		{
			point -= 1;

			set_pointer_pos(board[point].pos);
		}

		// pointer right if pointer is not on the last column

		if (point != (GRID_WIDTH * GRID_HEIGHT - 1) && bb::INPUT.isPressed(sf::Keyboard::Scan::Right))
		{
			point += 1;

			set_pointer_pos(board[point].pos);
		}

		// checking which brick is pointed by the mouse pointer

		auto pointM = mouse_on_brick(bb::INPUT.pointer().x, bb::INPUT.pointer().y);

		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter) || (pointM != -1 && bb::INPUT.isPressedM(sf::Mouse::Button::Left)))
		{
			int pointed;

			if (bb::INPUT.isPressed(sf::Keyboard::Scan::Enter))
			{
				// pointing a brick using keyboard

				pointed = point;	// use the pointed brick
			}

			if (pointM != -1 && bb::INPUT.isPressedM(sf::Mouse::Button::Left))
			{
				// pointing a brick using mouse

				pointed = pointM;	// use the brick under the mouse pointer
			}

			if (selected == -1)
			{
				// nothing is selected currently so select the pointed brick

				set_selecter_pos(board[pointed].pos);

				selected = pointed;
			}
			else if (selected == pointed)
			{
				selected = -1;	// deselect the selected brick
			}
			else if (adj_brick(selected, pointed))
			{
				// swapping the selected brick with the pointed brick

				// swap bricks in brickmap vector

				auto temp = board[pointed];

				board[pointed] = board[selected];

				board[selected] = temp;

				// swaping brickmap indices

				board[pointed].index = pointed;

				board[selected].index = selected;

				// swap positions of the bricks on screen with tween animation

				auto& pos_p = board[pointed].pos;

				auto& pos_s = board[selected].pos;

				tween.start(.2,
					bb::twn_list(
						bb::twn(pos_p.x, pos_s.x),
						bb::twn(pos_p.y, pos_s.y),
						bb::twn(pos_s.x, pos_p.x),
						bb::twn(pos_s.y, pos_p.y)
					),
					[this](double dt)
					{
						calculate_match();
					}
				);

				selected = -1;	// deselect the selected brick
			}
		}
	}



	// check if the selected brick is adjacent to the pointed brick

	bool adj_brick(int selected, int pointed)
	{
		return ((selected / GRID_WIDTH == pointed / GRID_WIDTH) && std::abs(selected - pointed) == 1) || std::abs(selected - pointed) == GRID_WIDTH;
	}


	bool mouse_on_brickmap(int x, int y)
	{
		return bb::in_rng(offset.x, (float)x, offset.x + GRID_WIDTH * BRICK_WIDTH - 1) && bb::in_rng(offset.y, (float)y, offset.y + GRID_HEIGHT * BRICK_HEIGHT - 1);
	}


	int mouse_on_brick(int x, int y)
	{
		if (!mouse_on_brickmap(x, y))
			return -1;	// mouse is not on the brickmap

		int mx = (x - (int)offset.x) / BRICK_WIDTH;

		int my = (y - (int)offset.y) / BRICK_HEIGHT;
		
		return my * GRID_WIDTH + mx;	// return the index of the brick under the mouse
	}


	/*
		after each swap this function is used to check for matches, update score,
		delete bricks and spawn new bricks
	*/

	void calculate_match()
	{
		auto matches = board.find_matches();

		if (matches.size())
		{
			// matches are found

			// updating the score

			for(const auto& match : matches)
			{
				g_data.score += match.size() * 50;	// 10 points for each brick in the match
			}

			// removing matched bricks

			board.remove_matches(matches);

			// filling new bricks in the empty spaces

			auto twn_vector = board.fill_matches();

			tween.start(.3,
				twn_vector.get_twn(),
				[this](double dt)
				{
					calculate_match();
				}
			);
		}
	}



	void Render() override
	{
		// safely access the data being updated by interval timer

		timer.lock();

		score_board.render(g_data.level, g_data.score, g_data.goal, g_data.time);

		timer.unlock();


		// safely access the data being tweened using lock() and unlock()

		tween.lock();

		board.render(offset);

		tween.unlock();


		// always draw the pointer on the selected brick

		pointer_color_timer.lock();

		// render select rect color based on timer
		
		if (pointer_color)
			pointer.setOutlineColor(sf::Color(217, 87, 99, 255));
		else
			pointer.setOutlineColor(sf::Color(172, 50, 50, 255));

		pointer_color_timer.unlock();

		bb::WINDOW.draw(pointer);


		// if a brick is selected, draw the selecter on it

		if (selected > -1)
		{
			bb::WINDOW.draw(selecter);
		}

		// rendering the screen fade effects

		screen.render();

		// rendering the banner

		banner.render();
	}



	void Exit() override
	{
		timer.stop();

		pointer_color_timer.stop();
	}
} play;
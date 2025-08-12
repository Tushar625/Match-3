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
		used to create curtain effect
	*/

	sf::RectangleShape curtain;

	int curtain_alpha;

	bool curtain_active;



	/*
		to show the level banner, like "Level 1"
	*/

	banner_class banner;

	bool banner_active;



	// tweener

	TWEENER tween;



	// interval timer

	INTERVAL_TIMER timer;	// decreases the timer of game data every second
	
	INTERVAL_TIMER pointer_color_timer;	// to change the pointer color periodically, to make it blink



	// delay timer

	DELAY_TIMER after;



public:



	play_state() :
		board(true),	// we save this board data
		score_board(sf::Vector2f(24, 24)),
		offset(sf::Vector2f(VIRTUAL_WIDTH - 272, 16)),
		point(0),
		pointer_color(false),
		selected(-1),	// initializing the pointer and selecter
		curtain(sf::Vector2f(VIRTUAL_WIDTH, VIRTUAL_HEIGHT)),
		curtain_alpha(0),
		curtain_active(false)
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



public:



	void Enter() override
	{
		curtain_active = true;

		curtain.setFillColor(sf::Color::White);

		tween.start(	// pulling up the curtain
			1,
			twn(curtain_alpha, 255, 0),
			[this](double dt)
			{
				curtain_active = false;

				// display the banner after the curtain is up

				banner_active = true;

				banner.reset();	// banner placed top the screen

				banner.set_text("Level " + std::to_string(g_data.level));

				tween.start(	// banner comes down to middle of the screen
					.25,
					twn(banner.y_pos, float(VIRTUAL_HEIGHT / 2 - banner.get_size().y / 2)),
					[this](double dt)
					{
						after.start(	// banner stays at the middle of the screen for 1 second
							1,
							[this](double dt)
							{
								tween.start(	// banner comes down to bottom of the screen
									.25,
									twn(banner.y_pos, float(VIRTUAL_HEIGHT)),
									[this](double dt)
									{
										banner_active = false;

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
						);
					}
				);
			}
		);
	}



	void Update(double dt) override
	{
		if (banner_active)
		{
			// only banner uses delay timer "after"

			after.update(dt);
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

		if (tween.is_running() || curtain_active || banner_active)
			return;


		// remaining time check

		timer.lock();

		if (g_data.time == 0)
		{
			// game over

			init(true);	// reset the game data

			sm.change_to(initial);

			return;
		}

		timer.unlock();


		// goal check

		if (g_data.score > g_data.goal)
		{
			// goal reached so level is finished

			timer.lock();

			g_data.time += 60;	// resetting the timer

			timer.unlock();

			g_data.goal += g_data.goal * 1.25 + 200;	// increase goal
		}



		// ===== taking inputs =====



		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Escape))
		{
			// pause the game

			curtain_active = true;

			curtain.setFillColor(sf::Color::Black);

			tween.start(
				1,
				twn(curtain_alpha, 0, 255),
				[this](double dt)
				{
					curtain_active = false;

					sm.change_to(initial);
				}
			);

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


		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Space))
		{
			if (selected == -1)
			{
				// nothing is selected currently so select the pointed brick

				set_selecter_pos(board[point].pos);

				selected = point;
			}
			else if (selected == point)
			{
				selected = -1;	// deselect the selected brick
			}
			else if (adj_brick(selected, point))
			{
				// swapping the selected brick with the pointed brick

				// swap bricks in brickmap vector

				auto temp = board[point];

				board[point] = board[selected];

				board[selected] = temp;

				// swaping brickmap indices

				board[point].index = point;

				board[selected].index = selected;

				// swap positions of the bricks on screen with tween animation

				auto& pos_p = board[point].pos;

				auto& pos_s = board[selected].pos;

				tween.start(.2,
					twn_list(
						twn(pos_p.x, pos_s.x),
						twn(pos_p.y, pos_s.y),
						twn(pos_s.x, pos_p.x),
						twn(pos_s.y, pos_p.y)
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

		// rendering the curtain

		if(curtain_active)
		{
			tween.lock();

			auto color = curtain.getFillColor();

			color.a = curtain_alpha;

			curtain.setFillColor(sf::Color(color));

			tween.unlock();

			bb::WINDOW.draw(curtain);
		}

		// rendering the banner

		if (banner_active)
		{
			tween.lock();

			banner.render();

			tween.unlock();
		}
	}



	void Exit() override
	{
		timer.stop();
	}
} play;
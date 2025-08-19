#pragma once

extern class game_over_state game_over;

class play_state : public bb::BASE_STATE
{
	highest_score_type *ph_data;

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

	bb::ScreenFade screen;	// creates the fade in and fade out effect



	/*
		to show the level banner, like "Level 1"
	*/

	bb::Banner banner;



	// the pause button

	bb::STR_BUTTON pause;



	// explosion effect

	bb::Firecracker explo;



	// tweener

	bb::TWEENER tween;



	// interval timer

	bb::INTERVAL_TIMER timer;	// decreases the timer of game data every second
	
	bb::INTERVAL_TIMER pointer_color_timer;	// to change the pointer color periodically, to make it blink

	bb::INTERVAL_TIMER explo_timer;



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
		),
		pause(
			medium_text,
			"Pause (Esc)",
			sf::Color(48, 96, 130),
			sf::Color(99, 155, 255),
			sf::Vector2f(24, VIRTUAL_HEIGHT - 24),
			bb::BOTTOM_LEFT
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



	void init(highest_score_type* ph_data, bool new_game = false)
	{
		this->ph_data = ph_data;

		// starting a new game

		if (new_game)
		{
			reset();
		}
	}



private:


	void reset()
	{
		// reset game data

		g_data.reset();

		// new brickmap

		board.generate_brickmap();

		// place the pointer on the top left brick

		point = 0;

		set_pointer_pos(board[point].pos);

		// no brick is selected

		selected = -1;
	}


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

		// start the explosion timer

		explo_timer.start(
			1.0 / 120,
			[this](double dt) -> bool
			{
				explo.update(dt);

				return true;
			}
		);
	}



	void Update(double dt) override
	{
		// updating the button

		auto mpos = bb::INPUT.pointer();

		auto pause_button_clicked = pause.is_clicked(
			mpos.x,
			mpos.y,
			bb::INPUT.isPressedM(sf::Mouse::Button::Left),
			bb::INPUT.isReleasedM(sf::Mouse::Button::Left)
		);

		if (banner.xfinalUpdate(dt))
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

					//screen.setColor(sf::Color::Black);

					screen.setColor(sf::Color(138, 3, 3));

					screen.startFadeOut([this]()
						{
							auto score = g_data.score;

							reset();	// reset the game

							sm.change_to(game_over, score, ph_data->update(score));	/*update highest score*/
						}
					);
				}
			);

			return;
		}

		timer.unlock();



		// ===== taking inputs =====



		if (bb::INPUT.isPressed(sf::Keyboard::Scan::Escape) || pause_button_clicked)
		{
			// pause the game

			// stop the game timer and pointer color blinking

			timer.stop();

			pointer_color_timer.stop();

			// fade out effect the screen goes black slowly

			screen.setColor(sf::Color::Black);

			screen.startFadeOut([this]() { sm.change_to(initial); });

			return;
		}


		// pointer up if pointer is not on the first row

		auto temp_point = point;	// to check if point changes

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

		// sound of pointer movement

		if (temp_point != point)
		{
			play_sound(POINTER);
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
		return bb::in_rng_open(offset.x, (float)x, offset.x + GRID_WIDTH * BRICK_WIDTH) && bb::in_rng_open(offset.y, (float)y, offset.y + GRID_HEIGHT * BRICK_HEIGHT);
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

			// updating the score and adding explosions

			explo_timer.lock();

			for(const auto& match : matches)
			{
				g_data.score += match.size() * 50;	// 10 points for each brick in the match

				// adding explosion

				for(const auto& brick : match)
				{
					auto pos = offset + brick.pos + sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT) / 2.0f;

					// color of the explosion is determined by the brick color

					auto color = BRICK_COLOR_FLASHY[brick.color];

					auto threshold = 130;

					if (color.r > threshold || color.g > threshold || color.b > threshold)
					{
						auto factor = 1.6;

						color.r = std::min(255, int(color.r * factor));

						color.g = std::min(255, int(color.g * factor));

						color.b = std::min(255, int(color.b * factor));
					}
					else
					{
						auto factor = 2.25;

						color.r = std::min(255, int(color.r * factor));

						color.g = std::min(255, int(color.g * factor));

						color.b = std::min(255, int(color.b * factor));
					}

					explo.create(pos, color, 4800, 115);
				}
			}

			explo_timer.unlock();

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


		// if a brick is selected, draw the selecter on it

		if (selected > -1)
		{
			bb::WINDOW.draw(selecter);
		}


		// always draw the pointer on the selected brick

		pointer_color_timer.lock();

		// render select rect color based on timer
		
		if (pointer_color)
			pointer.setOutlineColor(sf::Color(217, 87, 99, 255));
		else
			pointer.setOutlineColor(sf::Color(172, 50, 50, 255));

		pointer_color_timer.unlock();

		bb::WINDOW.draw(pointer);


		// the explosion

		explo_timer.lock();

		bb::WINDOW.draw(explo);

		explo_timer.unlock();

		// the pause button

		textShadow(pause.get_text(), { {1.5, 1.5}, {1.5, 1}, {0, 1.5} }, sf::Color(34, 32, 52));

		pause.Render();

		// rendering the screen fade effects

		screen.render();

		// rendering the banner

		banner.render();
	}



	void Exit() override
	{
		tween.stop();

		timer.stop();

		pointer_color_timer.stop();

		explo_timer.stop();

		screen.stop();

		banner.stop();
	}
} play;
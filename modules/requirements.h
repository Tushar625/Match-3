#pragma once



// game engine components

#include"BBS\game.h"

#include"BBS\asset\effect\banner.h"

#include"BBS\asset\effect\screen_fade.h"

#include"BBS\asset\button\all.h"

#include"BBS\asset\text\color_text.h"

#include"BBS\asset\particle_system\firecracker.h"

#include"BBS\asset\bg\inf_scrolling_bg.h"

#include"BBS\asset\effect\shadow.h"



// SFML components

#include<SFML\Audio.hpp>



// standard c++ components

#include<iostream>	// only for debugging

#include<string>

#include<array>

#include<vector>



// game modules

#include"constants.h"

#include"util.h"

#include"assets.h"

#include"brick.h"

#include"board.h"

#include"score_board.h"

#include"game_states/state_data.h"

// game states

#include"game_states/initial_state.h"

#include"game_states/play_state.h"

#include"Game_states/game_over_state.h"

#include"Game_states/highest_score_state.h"
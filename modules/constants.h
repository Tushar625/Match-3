#pragma once



#define WINDOW_WIDTH 1280

#define WINDOW_HEIGHT 720

#define VIRTUAL_WIDTH 512

#define VIRTUAL_HEIGHT 288



#define SMALL_FONT_SIZE 8

#define MEDIUM_FONT_SIZE 16

#define LARGE_FONT_SIZE 32



#define BRICK_WIDTH 32

#define BRICK_HEIGHT 32

#define BRICK_COLORS 18

#define BRICK_TYPES 6

#define MIN_MATCH_NUM 3



#define GRID_WIDTH 8

#define GRID_HEIGHT 8



enum TEXTURE_ENUM {MAIN, BACKGROUND, TEXTURE_COUNT};

//enum SPRITE_ENUM {BLOCK, SPRITE_COUNT};



const std::array<sf::Color, 18> BRICK_COLOR{
    sf::Color(217, 160, 102),
    sf::Color(217, 87, 99),
    sf::Color(138, 111, 48),
    sf::Color(217, 87, 99),
    sf::Color(82, 75, 36),
    sf::Color(172, 50, 50),
    sf::Color(75, 105, 47),
    sf::Color(102, 57, 49),
    sf::Color(55, 148, 110),
    sf::Color(143, 86, 59),
    sf::Color(91, 110, 225),
    sf::Color(223, 113, 38),
    sf::Color(48, 96, 130),
    sf::Color(132, 126, 135),
    sf::Color(63, 63, 116),
    sf::Color(105, 106, 106),
    sf::Color(118, 66, 138),
    sf::Color(89, 86, 82)
};


// flashy alternative for the brick colors, used for the explosion

const std::array<sf::Color, 18> BRICK_COLOR_FLASHY{
    sf::Color(255, 185, 120),  // brighter orange
    sf::Color(255, 130, 150),  // vibrant pink-red
    sf::Color(170, 135, 60),   // golden olive
    sf::Color(255, 120, 140),  // hot pink-red
    sf::Color(70, 125, 60),    // earthy yellow-green
    sf::Color(255, 100, 100),    // strong red
    sf::Color(95, 170, 55),    // vivid green
    sf::Color(175, 100, 75),    // warm brown
    sf::Color(65, 185, 135),   // teal
    sf::Color(175, 105, 70),   // brighter tan
    sf::Color(115, 135, 255),  // strong blue
    sf::Color(250, 135, 55),   // glowing orange
    sf::Color(70, 125, 170),   // aqua blue
    sf::Color(160, 150, 165),  // lighter gray
    sf::Color(110, 110, 165),    // violet
    sf::Color(135, 135, 135),  // silver-gray
    sf::Color(200, 110, 170),   // vivid purple
    sf::Color(115, 110, 105)   // warm gray
};

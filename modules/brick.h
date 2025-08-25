#pragma once

/*
	brick_struct is a structure that represents a single brick in the grid.
	
	it contains:
	
		-index of the brick in the brickmap or grid,
		-color and type of the brick,
		-its position on the screen

	The color and type values are combined to calculate the index (color * BRICK_TYPES + type)
	of the brick sprite in the brick_sprite vector, which stores all available brick sprites.

		-render() method is used to draw the brick at its position on the screen.
*/

struct brick_struct
{
	int index;	// index of the brick in the brickmap or grid

	int color, type;

	sf::Vector2f pos;

	brick_struct() = default;

	brick_struct(int _index, int _color, int _type, sf::Vector2f _pos)
	{
		index = _index;

		color = _color;

		type = _type;

		pos = _pos;
	}

	/*
		The offset represents the displacement of the brickmap from its default
		origin at (0, 0). When the brickmap is created, all bricks are initially
		positioned with the top-left corner of the brickmap at (0, 0). The offset
		is then added to each brick’s position to place them correctly on the screen.
	*/

	void render(const sf::Vector2f& offset) const noexcept
	{
		auto& brick = brick_sprite[color * BRICK_TYPES + type];

		brick.setPosition(pos + offset);

		bb::shadow(brick, sf::Color(30, 30, 30, 200), sf::Vector2f{ 2, 2 });	// the shadow of the brick

		bb::WINDOW.draw(brick);	// the brick
	}
};
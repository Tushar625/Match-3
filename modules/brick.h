/*
		brick_struct is a structure that represents a single brick in the grid.
		it contains:
			-color and type of the brick,
			-its position on the screen

		color and type are used to determine the index (color * BRICK_TYPES +
		type) of the brick sprite from brick_sprite vector, which contains all
		the brick sprites

			-render() method is used to draw the brick at its position on the
			screen.
	*/

struct brick_struct
{
	int index;	// index of the brick in the brickmap or grid

	int color, type;

	sf::Vector2f pos;

	brick_struct(int _index, int _color, int _type, sf::Vector2f _pos)
	{
		index = _index;

		color = _color;

		type = _type;

		pos = _pos;
	}

	void render(const sf::Vector2f& offset) const noexcept
	{
		auto& brick = brick_sprite[color * BRICK_TYPES + type];

		// render the shadow

		brick.setColor(sf::Color{ 30, 30, 30 });

		brick.setPosition(pos + sf::Vector2f(2, 2) + offset);

		bb::WINDOW.draw(brick);

		// render the brick

		brick.setColor(sf::Color{ 255, 255, 255 });

		brick.setPosition(pos + offset);

		bb::WINDOW.draw(brick);
	}
};
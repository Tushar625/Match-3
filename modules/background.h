#pragma once

class background
{
	float x, scale;

public:

	background() : x(0), scale(0)
	{
		// scaling in a way that fits entire height of the picture in the screen

		scale = VIRTUAL_HEIGHT / bg_sprite.getGlobalBounds().height;

		bg_sprite.setScale(scale, scale);
	}

	void update(double dt)
	{
		x -= 20 * dt;	// moving bg right to left

		if (x < -52 * scale)
		{
			/*
				after certain position we set bg to it's x = 0 as this "position"
				is identical to the front of this sprite, this creates the looping
				bg effect
			*/

			x = 0;
		}
	}

	void render()
	{
		bg_sprite.setPosition(sf::Vector2f(x, 0));

		bb::WINDOW.draw(bg_sprite);
	}
} bg;
#pragma once

#include<SFML/Graphics.hpp>

//#include"../../../system/window.h"



class BGLoopH
{

	sf::Sprite m_bgSprite;

	float m_x;
	
	float m_scale;
	
	float m_speed;
	
	float m_xRepeat;	// at this point we reset m_x to 0, to create the infinite scrolling animation


public:


	BGLoopH(const sf::Sprite& bgSprite, float xRepeat, float speed, float screenHeight) :
		m_bgSprite(bgSprite),
		m_x(0),
		m_scale(screenHeight / bgSprite.getGlobalBounds().height),
		m_speed(speed),
		m_xRepeat(xRepeat * m_scale)
	{
		// scaling bgSprite in a way that fits entire height of the picture in the screen

		m_bgSprite.setScale(m_scale, m_scale);
	}


	// the setters


	void setSpeed(float speed) noexcept
	{
		m_speed = speed;
	}


	// the getters


	float getSpeed() const noexcept
	{
		return m_speed;
	}


	void update(double dt) noexcept
	{
		m_x -= m_speed * dt;	// moving bg right to left

		if (m_x < -m_xRepeat)
		{
			/*
				after certain position we set bg to it's x = 0 as this "position"
				is identical to the front of this sprite, this creates the looping
				bg effect
			*/

			m_x = 0;
		}
	}


	void render() noexcept
	{
		m_bgSprite.setPosition(m_x, 0);

		bb::WINDOW.draw(m_bgSprite);
	}
};
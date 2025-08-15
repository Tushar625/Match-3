#pragma once


class ScreenFade
{
	sf::RectangleShape m_screen;

	sf::Color m_color;


	double m_duration;

	uint8_t m_alpha;

	bool m_active;


	TWEENER m_tween;



public:



	ScreenFade(const sf::Vector2f& size = { 800, 600 }) :
		m_screen(size),
		m_color(sf::Color::White),
		m_duration(1.0),
		m_alpha(0),
		m_active(false)
	{}


	// the setters


	void setColor(const sf::Color& newColor) noexcept
	{
		m_color = newColor;
	}


	void setDuration(double duration) noexcept
	{
		m_duration = duration;
	}


	void setPosition(const sf::Vector2f& position) noexcept
	{
		m_screen.setPosition(position);
	}


	void setSize(const sf::Vector2f& size) noexcept
	{
		m_screen.setSize(size);
	}


	// the getters


	sf::Color getColor() const noexcept
	{
		return m_color;
	}


	double getDuration() const noexcept
	{
		return m_duration;
	}


	sf::Vector2f getPosition() const noexcept
	{
		return m_screen.getPosition();
	}


	sf::Vector2f getSize() const noexcept
	{
		return m_screen.getSize();
	}


	// operational methods


	bool isActive() const noexcept
	{
		return m_active;
	}


	bool startFadeIn(std::function<void(void)> final = nullptr) noexcept
	{
		if(m_active)
		{
			return false;	// already active
		}

		m_active = true;

		m_tween.start(
			m_duration,
			twn(m_alpha, (uint8_t)255, (uint8_t)0),
			[this, final](double dt)
			{
				m_active = false;

				if (final)
				{
					final();
				}
			}
		);

		return true;
	}


	bool startFadeOut(std::function<void(void)> final = nullptr) noexcept
	{
		if (m_active)
		{
			return false;	// already active
		}

		m_active = true;

		m_tween.start(
			m_duration,
			twn(m_alpha, (uint8_t)0, (uint8_t)255),
			[this, final](double dt)
			{
				m_active = false;

				if (final)
				{
					final();
				}
			}
		);

		return true;
	}


	bool xfinal() noexcept
	{
		return m_tween.xfinal();
	}


	void render() noexcept
	{
		if (m_active)
		{
			m_tween.lock();

			m_screen.setFillColor(sf::Color(m_color.r, m_color.g, m_color.b, m_alpha));

			m_tween.unlock();

			bb::WINDOW.draw(m_screen);
		}
	}
};
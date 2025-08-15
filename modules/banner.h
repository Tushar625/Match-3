#pragma once






class Banner
{
	sf::Text m_text;
	
	sf::RectangleShape m_bg;


	double m_fallDuration;	// duration of the fall animation, from top to middle, and middle to bottom
	
	double m_pauseDuration;	// duration of the pause after the banner reaches the middle of the screen

	float m_yPos;	//y position of the banner, it's tweened to make the banner fall through the screen

	float m_screenHeight;	// height of the screen, used to calculate the final position of the banner

	bool m_active;	// indicates if the banner animation is running or not


	TWEENER m_tween;

	DELAY_TIMER m_after;



public:



	/*
		constructor, initializes the banner with the given text, background size,
		screen height, text color and background color
	*/

	Banner(const sf::Text& text, const sf::Vector2f& bgSize, float screenHeight, const sf::Color& textColor = sf::Color::White, const sf::Color& bgColor = sf::Color::Black) :
		m_text(text),
		m_bg(bgSize),
		m_fallDuration(.25),	// default fall duration is 0.25 seconds
		m_pauseDuration(1),		// default pause duration is 1 second
		m_yPos(-bgSize.y),		// "-bgSize.y", a position above the top edge of the screen
		m_screenHeight(screenHeight),
		m_active(false)
	{
		m_text.setFillColor(textColor);
		
		m_bg.setFillColor(bgColor);
	}


	
	// the setters



	void setTextColor(const sf::Color& textColor) noexcept
	{
		m_text.setFillColor(textColor);
	}


	void setBgColor(const sf::Color& bgColor) noexcept
	{
		m_text.setFillColor(bgColor);
	}


	void setBgSize(const sf::Vector2f& bgSize) noexcept
	{
		m_bg.setSize(bgSize);
	}


	void setScreenHeight(float screenHeight) noexcept
	{
		m_screenHeight = screenHeight;
	}


	void setFallDuration(double fallDuration) noexcept
	{
		m_fallDuration = fallDuration;
	}


	void setPauseDuration(double pauseDuration) noexcept
	{
		m_pauseDuration = pauseDuration;
	}


	
	// the getters



	const sf::Color& getTextColor() const noexcept
	{
		return m_text.getFillColor();
	}


	const sf::Color& getBgColor() const noexcept
	{
		return m_bg.getFillColor();
	}


	const sf::Vector2f& getBgSize() const noexcept
	{
		return m_bg.getSize();
	}


	float getScreenHeight() const noexcept
	{
		return m_screenHeight;
	}


	double getFallDuration() const noexcept
	{
		return m_fallDuration;
	}


	double getPauseDuration() const noexcept
	{
		return m_pauseDuration;
	}



	// directly access the text and bg rectangle



	sf::Text& text() noexcept
	{
		return m_text;
	}


	sf::RectangleShape& bg() noexcept
	{
		return m_bg;
	}



	// main operational methods



	/*
		start the animation with the given string, and an optional final function
		to execute after the animation is over

		returns true if the animation is started successfully, false if the animation is already running
	*/

	bool startAnimation(const std::string& str, std::function<void(void)> final = nullptr)
	{
		if (m_active)
		{
			return false;	// already active
		}

		m_active = true;

		m_text.setString(str);	// set banner string

		bb::setCenterOrigin(m_text);	// center origin the string

		m_tween.start(
			
			m_fallDuration,
			
			// place the banner beyond top edge of the screen and bring it down to middle of the screen
			
			twn(m_yPos, -m_bg.getSize().y, float(m_screenHeight / 2.0 - m_bg.getSize().y / 2)),

			[this, final](double dt)
			{
				// after the banner reaches the middle, it pauses for 1 second before it falls down

				m_after.start(
					
					m_pauseDuration,
					
					[this, final](double dt)
					{
						// after the pause, we start the tween to bring the banner down

						m_tween.start(
							
							m_fallDuration,

							// banner falls beyond the bottom edge of the screen
							
							twn(m_yPos, float(m_screenHeight)),

							[this, final](double dt)
							{
								// the animation is over

								m_active = false;

								// execute the final function after the animation is over

								if (final)
								{
									final();
								}
							}
						);
					}
				);
			}
		);

		return true;
	}


	/*
		execute the final() function after the banner animation is over

		returns true only when final() is executed after the banner animation is over
	*/

	bool xfinal(double dt)
	{
		if (m_active)
		{
			m_after.update(dt);

			/*
				return true only after the banner animation is over

				after the animation is over, xfinal() of tween returns true and sets m_active to false,
				so this function returns true
			*/

			return m_tween.xfinal() && !m_active;
		}

		return false;
	}


	// check if the animation is running or not

	bool isActive() const noexcept
	{
		return m_active;
	}


	// render the banner when the animation is running

	void render()
	{
		if (m_active)
		{
			m_tween.lock();

			// origin of the bg rectangle is at the top left corner

			m_bg.setPosition(sf::Vector2f(0, m_yPos));

			// origin of the text is at its center, we place the text in the middle of the bg rectangle

			m_text.setPosition(sf::Vector2f(m_bg.getSize().x / 2, m_yPos + m_bg.getSize().y / 2));

			m_tween.unlock();

			bb::WINDOW.draw(m_bg);

			bb::WINDOW.draw(m_text);
		}
	}
};
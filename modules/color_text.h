#pragma once


#include<SFML/Graphics.hpp>
//#include"../../utility/pos_fun.h"
#include<vector>
#include<string>
#include<cctype>

#include<iostream>


class ColorText
{
	std::vector<sf::Text> m_textList;

	double m_time, m_interval;

public:

	ColorText() : m_time(0), m_interval(.075)
	{ }

	void set(sf::Text text, const std::string& str, int gap, int x, int y, bb::COORD_POSITION pos = bb::TOP_LEFT)
	{
		int X, Y, width, height;

		m_textList.clear();

		X = height = 0;

		for (const auto& ch : str)
		{
			text.setString(std::string(1, ch));

			if (!std::isspace(ch))
			{
				text.setOrigin(text.getLocalBounds().getPosition());

				text.setPosition(X, 0);

				m_textList.push_back(text);
			}

			// update position X

			X += text.getLocalBounds().width + gap;

			// update height

			if (height < text.getLocalBounds().height)
			{
				height = text.getLocalBounds().height;
			}
		}

		width = X - gap;

		// Now we have height and width, lets get the top left

		bb::to_top_left(X, Y, x, y, height, width, pos);

		// moving the texts to proper position

		for (auto& text : m_textList)
		{
			text.move(X, Y);
		}
	}

	void setColor(const std::vector<sf::Color>& colors)
	{
		int i = 0;

		for (auto& text : m_textList)
		{
			text.setFillColor(colors[i++ % colors.size()]);
		}
	}

	void setInterval(double interval)
	{
		m_interval = interval;
	}

	void colorLS()
	{
		auto temp = m_textList[0].getFillColor();

		for (int i = 1; i < m_textList.size(); i++)
		{
			m_textList[i - 1].setFillColor(m_textList[i].getFillColor());
		}

		m_textList[m_textList.size() - 1].setFillColor(temp);
	}

	void colorRS()
	{
		auto temp = m_textList[m_textList.size() - 1].getFillColor();

		for (int i = m_textList.size() - 1; i > 0; i--)
		{
			m_textList[i].setFillColor(m_textList[i - 1].getFillColor());
		}

		m_textList[0].setFillColor(temp);
	}

	void updateRS(double dt)
	{
		m_time += dt;

		if (m_time > .075)
		{
			m_time = 0;

			colorRS();
		}
	}

	void updateLS(double dt)
	{
		m_time += dt;

		if (m_time > .075)
		{
			m_time = 0;

			colorLS();
		}
	}

	sf::Text getText(int index) const noexcept
	{
		return m_textList[index];
	}

	int getTextCount() const noexcept
	{
		return static_cast<int>(m_textList.size());
	}

	void render()
	{
		for (const auto& text : m_textList)
		{
			bb::WINDOW.draw(text);
		}
	}
};


void textShadow(sf::Text text, const std::vector<sf::Vector2f>& offsets, const sf::Color& color = sf::Color(0, 0, 0))
{
	text.setFillColor(color);

	auto pos = text.getPosition();

	for (const auto& offset : offsets)
	{
		text.setPosition(pos + offset);

		bb::WINDOW.draw(text);
	}
}
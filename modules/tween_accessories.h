#pragma once

#include<tuple>

#include<cmath>

#include<numbers>


/*
	these functions can be optimized with inverse_sqrt() trick
*/

// source: https://easings.net/

struct TWN_TYPE
{
	using func = std::function<double(double)>;


	// linar


	static constexpr double linear(double x) noexcept
	{
		return x;
	}


	// sine


	static double in_sine(double x) noexcept
	{
		return 1.0 - std::cos((x * std::numbers::pi) / 2.0);
	}

	static double out_sine(double x) noexcept
	{
		return std::sin((x * std::numbers::pi) / 2.0);
	}

	static double in_out_sine(double x) noexcept
	{
		return -(std::cos(std::numbers::pi * x) - 1.0) / 2.0;
	}


	// quad


	static double in_quad(double x) noexcept
	{
		return x * x;
	}

	static double out_quad(double x) noexcept
	{
		return 1.0 - (1.0 - x) * (1.0 - x);
	}

	static double in_out_quad(double x) noexcept
	{
		return (x < 0.5)
			? 2.0 * x * x
			: 1.0 - std::pow(-2.0 * x + 2.0, 2.0) / 2.0;
	}


	// cubic


	static double in_cubic(double x) noexcept
	{
		return x * x * x;
	}

	static double out_cubic(double x) noexcept
	{
		return 1.0 - std::pow(1.0 - x, 3.0);
	}

	static double in_out_cubic(double x) noexcept
	{
		return (x < 0.5)
			? 4.0 * x * x * x
			: 1.0 - std::pow(-2.0 * x + 2.0, 3.0) / 2.0;
	}


	// quart


	static double in_quart(double x) noexcept
	{
		return x * x * x * x;
	}

	static double out_quart(double x) noexcept
	{
		return 1.0 - std::pow(1.0 - x, 4.0);
	}

	static double in_out_quart(double x) noexcept
	{
		return (x < 0.5)
			? 8.0 * x * x * x * x
			: 1.0 - std::pow(-2.0 * x + 2.0, 4.0) / 2.0;
	}


	// quint


	static double in_quint(double x) noexcept
	{
		return x * x * x * x * x;
	}

	static double out_quint(double x) noexcept
	{
		return 1.0 - std::pow(1.0 - x, 5.0);
	}

	static double in_out_quint(double x) noexcept
	{
		return (x < 0.5)
			? 16.0 * x * x * x * x * x
			: 1.0 - std::pow(-2.0 * x + 2.0, 5.0) / 2.0;
	}


	// expo


	static double in_expo(double x) noexcept
	{
		return (x == 0.0)
			? 0.0
			: std::pow(2.0, 10.0 * x - 10.0);
	}

	static double out_expo(double x) noexcept
	{
		return (x == 1.0)
			? 1.0
			: 1.0 - std::pow(2.0, -10.0 * x);
	}

	static double in_out_expo(double x) noexcept
	{
		return (x == 0.0)
			? 0.0
			: (x == 1.0)
			? 1.0
			: (x < 0.5)
			? std::pow(2.0, 20.0 * x - 10.0) / 2.0
			: (2.0 - std::pow(2.0, -20.0 * x + 10.0)) / 2.0;
	}


	// circ


	static double in_circ(double x) noexcept
	{
		return 1.0 - std::sqrt(1.0 - std::pow(x, 2.0));
	}

	static double out_circ(double x) noexcept
	{
		return std::sqrt(1.0 - std::pow(x - 1.0, 2.0));
	}

	static double in_out_circ(double x) noexcept
	{
		return (x < 0.5)
			? (1.0 - std::sqrt(1.0 - std::pow(2.0 * x, 2.0))) / 2.0
			: (std::sqrt(1.0 - std::pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0;
	}


	// back


	static double in_back(double x) noexcept
	{
		constexpr double c1 = 1.70158;
		constexpr double c3 = c1 + 1.0;

		return c3 * x * x * x - c1 * x * x;
	}

	static double out_back(double x) noexcept
	{
		constexpr double c1 = 1.70158;
		constexpr double c3 = c1 + 1.0;

		return 1.0 + c3 * std::pow(x - 1.0, 3.0) + c1 * std::pow(x - 1.0, 2.0);
	}

	static double in_out_back(double x) noexcept
	{
		constexpr double c1 = 1.70158;
		constexpr double c2 = c1 * 1.525;

		return (x < 0.5)
			? (std::pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0
			: (std::pow(2.0 * x - 2.0, 2.0) * ((c2 + 1.0) * (2.0 * x - 2.0) + c2) + 2.0) / 2.0;
	}


	// elastic


	static double in_elastic(double x) noexcept
	{
		constexpr double c4 = (2.0 * std::numbers::pi) / 3.0;

		return (x == 0.0)
			? 0.0
			: (x == 1.0)
			? 1.0
			: -std::pow(2.0, 10.0 * x - 10.0) * std::sin((x * 10.0 - 10.75) * c4);
	}

	static double out_elastic(double x) noexcept
	{
		constexpr double c4 = (2.0 * std::numbers::pi) / 3.0;

		return (x == 0.0)
			? 0.0
			: (x == 1.0)
			? 1.0
			: std::pow(2.0, -10.0 * x) * std::sin((x * 10.0 - 0.75) * c4) + 1.0;
	}

	static double in_out_elastic(double x) noexcept
	{
		constexpr double c5 = (2.0 * std::numbers::pi) / 4.5;

		return (x == 0.0)
			? 0.0
			: (x == 1.0)
			? 1.0
			: (x < 0.5)
			? -std::pow(2.0, 20.0 * x - 10.0) * std::sin((20.0 * x - 11.125) * c5) / 2.0
			: std::pow(2.0, -20.0 * x + 10.0) * std::sin((20.0 * x - 11.125) * c5) / 2.0 + 1.0;
	}


	// bounce


	static double out_bounce(double x) noexcept
	{
		constexpr double n1 = 7.5625;
		constexpr double d1 = 2.75;

		if (x < 1.0 / d1)
		{
			return n1 * x * x;
		}
		else if (x < 2.0 / d1)
		{
			x -= 1.5 / d1;
			return n1 * x * x + 0.75;
		}
		else if (x < 2.5 / d1)
		{
			x -= 2.25 / d1;
			return n1 * x * x + 0.9375;
		}
		else
		{
			x -= 2.625 / d1;
			return n1 * x * x + 0.984375;
		}
	}

	static double in_bounce(double x) noexcept
	{
		return 1.0 - out_bounce(1.0 - x);
	}

	static double in_out_bounce(double x) noexcept
	{
		return (x < 0.5)
			? (1.0 - out_bounce(1.0 - 2.0 * x)) / 2.0
			: (1.0 + out_bounce(2.0 * x - 1.0)) / 2.0;
	}


	// initializing default type

	inline static func default_type = linear;
};


/*
	The twn() function takes a reference to a variable, an optional start value, an end value, and
	optionally an ease function or a type of tween. It then returns a tuple containing all this
	information. The Tweener object uses this to modify the variable's value smoothly from start to
	end over the specified duration.
*/

template <typename TYPE>

constexpr std::tuple<TYPE&, TYPE, TYPE, TWN_TYPE::func> twn(TYPE& data, TYPE end, TWN_TYPE::func ease = nullptr) noexcept
{
	// assuming that data is already initialized

	if (!ease)
	{
		ease = TWN_TYPE::default_type;
	}

	return std::tuple<TYPE&, TYPE, TYPE, TWN_TYPE::func>(
		data	/*ref*/,
		data	/*initial value*/,
		end		/*end value*/,
		ease	/*easing function*/
	);
}


template <typename TYPE>

constexpr std::tuple<TYPE&, TYPE, TYPE, TWN_TYPE::func> twn(TYPE& data, TYPE start, TYPE end, TWN_TYPE::func ease = nullptr) noexcept
{
	data = start;	// initialize data with start

	return twn(data, end, ease);
}


/*
	This function accepts several tuples created by twn() and combines them into a
	single tuple of tuples
*/

template <typename... TYPE>

constexpr std::tuple<TYPE...> twn_list(TYPE... twn_tuple) noexcept
{
	return std::tuple<TYPE...>(twn_tuple...);
}
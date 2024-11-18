#pragma once
#ifndef TEMPLATE_VALUE_TYPES_H
#define TEMPLATE_VALUE_TYPES_H

struct Color
{
	constexpr Color(): r(0), g(0), b(0) {}
	constexpr Color(int red, int green, int blue): r(red), g(green), b(blue) {}
	int r;
	int g;
	int b;

	constexpr inline bool operator==(Color o) const
	{
		return IsEqual(o);
	}
	constexpr inline bool operator!=(Color o) const
	{
		return !IsEqual(o);
	}
private:
	constexpr inline bool IsEqual(const Color& o) const {
		return r == o.r && g == o.g && b == o.b;
	}
};

template <typename N>
struct NumberRange
{
	constexpr NumberRange(): min(), max() {}
	constexpr NumberRange(N mini, N maxi): min(mini), max(maxi) {}
	constexpr NumberRange(N val): min(val), max(val) {}
	N min;
	N max;

	constexpr inline bool operator==(const NumberRange<N>& o) const {
		return IsEqual(o);
	}
	constexpr inline bool operator==(const N& o) const {
		return IsEqual(o);
	}
	constexpr inline bool operator!=(const NumberRange<N>& o) const {
		return !IsEqual(o);
	}
	constexpr inline bool operator!=(const N& o) const {
		return !IsEqual(o);
	}
private:
	constexpr inline bool IsEqual(const NumberRange<N>& o) const {
		return min == o.min && max == o.max;
	}
	constexpr inline bool IsEqual(const N& o) const {
		return min == o && max == o;
	}
};

typedef NumberRange<float> FloatRange;
typedef NumberRange<int> IntRange;

#endif

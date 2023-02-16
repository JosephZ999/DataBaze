#pragma once

struct Size2D
{
	Size2D() {}
	Size2D(const Size2D& OtherSize)
		: X(OtherSize.X)
		, Y(OtherSize.Y)
	{
	}
	Size2D(int InX, int InY)
		: X(InX)
		, Y(InY)
	{
	}

	int X = 0;
	int Y = 0;

	Size2D& operator=(const Size2D& NewSize);
	Size2D	operator/(int Value) const;
	Size2D	operator-(const Size2D& OtherSize) const;
};

inline Size2D& Size2D::operator=(const Size2D& NewSize)
{
	X = NewSize.X;
	Y = NewSize.Y;
	return *this;
}

inline Size2D Size2D::operator/(int Value) const
{
	if (Value == 0)
	{
		return Size2D(*this);
	}
	return Size2D(X / Value, Y / Value);
}

inline Size2D Size2D::operator-(const Size2D & OtherSize) const
{
	return Size2D(X - OtherSize.X, Y - OtherSize.Y);
}

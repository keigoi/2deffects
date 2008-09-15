#ifndef _SYD_GEOMETRY_H_
#define _SYD_GEOMETRY_H_

namespace syd
{
class Point;
class Rect;

bool operator ==(const Point& lhs, const Point& rhs);
bool operator !=(const Point& lhs, const Point& rhs);
bool operator ==(const Rect& lhs, const Rect& rhs);
bool operator !=(const Rect& lhs, const Rect& rhs);

class Point
{
	friend bool operator ==(const Point& lhs, const Point& rhs);
	friend bool operator !=(const Point& lhs, const Point& rhs);
	int x_,y_;

public:
	int x() const;
	int y() const;

public:
	Point& setX(int x);
	Point& setY(int y);

public:
	Point(int x, int y);
	Point();	//(0,0)Ç≈ê∂ê¨
};


class Rect
{
	friend bool operator ==(const Rect& lhs, const Rect& rhs);
	friend bool operator !=(const Rect& lhs, const Rect& rhs);
	Point topLeft_,
		  bottomRight_;

public:
	Point topLeft() const;
	Point bottomRight() const;
	int size() const;
	int width() const;
	int height() const;

public:
	Rect& setTopLeft(const Point& topLeft);
	Rect& setBottomRight(const Point& bottomRight);
	Rect& setWidth(int w);
	Rect& setHeight(int h);
public:
	Rect();
	Rect(const Point& topLeft, const Point& bottomRight);
	Rect(int left, int top, int right, int bottom);

};


} // namespace syd

#endif //(_SYD_GEOMETRY_H_)

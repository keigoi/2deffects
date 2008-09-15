#include "utils/geometry.h"

namespace syd {

//Point Class --------------------------
int Point::x() const {
	return x_;
}

int Point::y() const {
	return y_;
}

Point& Point::setX(int x) {
	x_ = x;
	return *this;
}

Point& Point::setY(int y) {
	y_ = y;
	return *this;
}

Point::Point() {
	x_ = y_ = 0;
}

Point::Point(int x, int y) {
	x_ = x; y_ = y;
}
//Point Class --------------------------

//Rect Class --------------------
Point Rect::topLeft() const {
	return topLeft_;
}

Point Rect::bottomRight() const {
	return bottomRight_;
}

int Rect::size() const {
	return this->width() * this->height();
}

int Rect::width() const {
	return (bottomRight_.x() - topLeft_.x());
}

int Rect::height() const {
	return (bottomRight_.y() - topLeft_.y());
}

Rect& Rect::setTopLeft(const Point& topLeft) {
	topLeft_ = topLeft;
	return *this;
}

Rect& Rect::setBottomRight(const Point& bottomRight) {
	bottomRight_ = bottomRight;
	return *this;
}
Rect& Rect::setWidth(int w) {
	bottomRight_.setX(topLeft_.x() + w);
	return *this;
}
Rect& Rect::setHeight(int h) {
	bottomRight_.setY(topLeft_.y() + h);
	return *this;
}

Rect::Rect() : topLeft_(0,0), bottomRight_(0,0) {

}

Rect::Rect(const Point& topLeft, const Point& bottomRight) {
	topLeft_ = topLeft;
	bottomRight_ = bottomRight;
}

Rect::Rect(int left, int top, int right, int bottom) {
	topLeft_ = Point(left, top);
	bottomRight_ = Point(right, bottom);
}


bool operator !=(const Point& lhs, const Point& rhs) {
	return !operator==(lhs,rhs);
}

bool operator ==(const Point& lhs, const Point& rhs) {
	return ( (lhs.x_ == rhs.x_) && (lhs.y_ == rhs.y_) );
}

bool operator !=(const Rect& lhs, const Rect& rhs) {
	return !operator==(lhs,rhs);
}

bool operator ==(const Rect& lhs, const Rect& rhs) {
	return ( (lhs.topLeft_ == rhs.topLeft_) &&
			(lhs.bottomRight_ == rhs.bottomRight_) );
}


} // namespace syd
#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

class Point
{
public:
    int x;
    int y;

    Point() {}
    Point(int x, int y) : x(x), y(y) {}

    // copy constructor
    Point(const Point &p) : x(p.x), y(p.y) {}

    virtual ~Point() {}

    // // Point is small, so choose pass-by-value
    // inline bool operator==(const Point p2)
    // {
    //     return x == p2.x && y == p2.y;
    // };

    // inline const Point operator-(const Point p2)
    // {
    //     return Point(x - p2.x, y - p2.y);
    // };

    friend std::ostream &operator<<(std::ostream &, const Point &);
};

bool operator==(const Point p1, const Point p2);
const Point operator-(const Point p1, const Point p2);
long dist_square(const Point p1, const Point p2);

#endif
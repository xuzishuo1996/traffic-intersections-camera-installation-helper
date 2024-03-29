#include <iostream>
#include "Point.hpp"
#include "Segment.hpp"

std::ostream &operator<<(std::ostream &os, const Point &p)
{
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

bool operator==(const Point p1, const Point p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

const Point operator-(const Point p1, const Point p2)
{
    return Point(p1.x - p2.x, p1.y - p2.y);
};

long dist_square(const Point p1, const Point p2)
{
    long x_diff = p1.x - p2.x;
    long y_diff = p1.y - p2.y;
    return x_diff * x_diff + y_diff * y_diff;
}

/*int main()
{
    // test Point
    Point p1 = Point(1, 2);
    Point p2 = Point(1, 2);
    Point *p3 = new Point(1, 2);
    Point p4 = Point(2, 1);
    Point *p5 = new Point(1, 2);

    std::cout << "p1 == p2: " << (p1 == p2) << std::endl;
    std::cout << "p1 == *p3: " << (p1 == *p3) << std::endl;
    std::cout << "p1 == p4: " << (p1 == p4) << std::endl;
    std::cout << "*p3 == *p5: " << (*p3 == *p5) << std::endl;

    // output:
    // p1 == p2: 1
    // p1 == *p3: 1
    // p1 == p4: 0
    // *p3 == *p5: 1

    std::cout << p1 << p1 << std::endl;

    std::cout << Point(2, 3) - Point(0, 2) << std::endl;
}*/
#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Point.hpp"

class Segment
{
public:
    Point p1;
    Point p2;

    Segment(const Point &a1, const Point &a2) : p1(a1), p2(a2) {}
    virtual ~Segment() {}
};

#endif
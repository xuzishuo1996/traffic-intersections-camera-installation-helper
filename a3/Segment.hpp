#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "Point.hpp"

class Segment
{
public:
    Point p1;
    Point p2;

    Segment() {}
    Segment(const Point &a1, const Point &a2) : p1(a1), p2(a2) {}
    virtual ~Segment() {}

    // Segment is small, so choose pass-by-value
    inline bool operator==(const Segment s2)
    {
        return ((p1 == s2.p1) and (p2 == s2.p2)) || ((p1 == s2.p2) and (p2 == s2.p1));
    };
};

#endif
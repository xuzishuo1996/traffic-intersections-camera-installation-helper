#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <forward_list>
#include "Point.hpp"
#include "Segment.hpp"
#include "SegUtility.hpp"

/* calculate the cross product of 2 vectors. */
int cross_product(const Point p1, const Point p2)
{
    return p1.x * p2.y - p2.x * p1.y;
}

/* calculate the dot product of 2 vectors. */
int dot_product(const Point p1, const Point p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}

bool three_points_on_same_line(const Point p1, const Point p2, const Point p3)
{
    return cross_product(p2 - p1, p3 - p1) == 0;
}

// Segment is small, so pass-by-value
bool segs_on_same_line(const Segment s1, const Segment s2)
{
    return three_points_on_same_line(s1.p1, s1.p2, s2.p1) && three_points_on_same_line(s1.p1, s1.p2, s2.p2);
}

/* for non-adjacent segs in the same street: use it only when segs are on the same line */
bool same_line_intersect(const Segment s1, const Segment s2)
{
    return (std::min(s1.p1.x, s1.p2.x) <= std::max(s2.p1.x, s2.p2.x)) && (std::max(s1.p1.x, s1.p2.x) >= std::min(s2.p1.x, s2.p2.x)) && (std::min(s1.p1.y, s1.p2.y) <= std::max(s2.p1.y, s2.p2.y)) && (std::max(s1.p1.y, s1.p2.y) >= std::min(s2.p1.y, s2.p2.y));
}

/* for adjacent segs in the same street: use it only when segs are on the same line */
bool adj_same_line_overlap(const Segment s1, const Segment s2)
{
    return dot_product(s1.p2 - s1.p1, s2.p2 - s2.p1) < 0;
}

/* for segs in diff streets: use it only when segs are on the same line and intersect */
bool same_line_overlap(Segment s1, Segment s2)
{
    if (s1 == s2)
    {
        return true;
    }
    std::forward_list<Point> point_lst{Point(s1.p1), Point(s1.p2), Point(s2.p1), Point(s2.p2)};
    long max_dist = 0;
    Point far_ends[2];
    for (std::forward_list<Point>::const_iterator i = point_lst.begin(); i != point_lst.end(); ++i)
    {
        // Loop over all members up to, but excluding, the current outer-loop member.
        for (std::forward_list<Point>::const_iterator j = point_lst.begin(); j != i; ++j)
        {
            // std::cout << *i << "," << *j << std::endl;
            long dist = dist_square(*i, *j);
            if (dist > max_dist)
            {
                max_dist = dist;
                far_ends[0] = *i;
                far_ends[1] = *j;
            }
        }
    }

    point_lst.remove(far_ends[0]);
    point_lst.remove(far_ends[1]);
    point_lst.unique();
    unsigned size = 0;
    for (std::forward_list<Point>::const_iterator i = point_lst.begin(); i != point_lst.end(); ++i)
    {
        ++size;
    }
    if (size == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// /* hash function of Point: for same_line_overlap() below */
// size_t hf(const Point &p)
// {
//     return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
// }

/* for segs in diff streets: use it only when segs are on the same line and intersect */
/*bool same_line_overlap(Segment s1, Segment s2)
{
    if (s1 == s2)
    {
        return true;
    }
    std::forward_list<Point> point_lst{Point(s1.p1), Point(s1.p2), Point(s2.p1), Point(s2.p2)};
    // lambda hash function for unordered_set<Point>
    auto hf = [](const Point &p) { return std::hash<int>()(p.x) ^ std::hash<int>()(p.y); };
    std::unordered_set<Point, decltype(hf)>
        point_set(point_lst.begin(), point_lst.end());

    long max_dist = 0;
    Point far_ends[2];
    for (std::unordered_set<Point>::const_iterator i = point_set.begin(); i != point_set.end(); ++i)
    {
        // Loop over all members up to, but excluding, the current outer-loop member.
        for (std::unordered_set<Point>::const_iterator j = point_set.begin(); j != i; ++j)
        {
            std::cout << *i << "," << *j << std::endl;
            long dist = dist_square(*i, *j);
            if (dist > max_dist)
            {
                max_dist = dist;
                far_ends[0] = *i;
                far_ends[1] = *j;
            }
        }
    }

    point_lst.remove(far_ends[0]);
    point_lst.remove(far_ends[1]);
    std::unordered_set<Point, decltype(hf)> res{point_lst.begin(), point_lst.end()};
    if (res.size() == 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}*/

bool straddle(const Segment s1, const Segment s2)
{
    Point p0 = Point(s1.p2) - Point(s1.p1);
    Point p1 = Point(s2.p1) - Point(s1.p1);
    Point p2 = Point(s2.p2) - Point(s1.p1);
    return cross_product(p1, p0) * cross_product(p2, p0) <= 0;
}

/* use it only when segs are not on the same line. 
   is_intersected algorithm reference: https://zhuanlan.zhihu.com/p/37360022
   judge if both segment cross the other line using cross product. */
bool is_intersected(const Segment s1, const Segment s2)
{
    return straddle(s1, s2) && straddle(s2, s1);
}

/* valid: return true */
bool non_adj_segs_valid(const Segment s1, const Segment s2)
{
    if (segs_on_same_line(s1, s2))
    {
        return !same_line_intersect(s1, s2);
    }
    else
    {
        return !is_intersected(s1, s2);
    }
}

/* valid: return true */
bool adj_segs_valid(const Segment s1, const Segment s2)
{
    if (segs_on_same_line(s1, s2))
    {
        return !adj_same_line_overlap(s1, s2);
    }
    else
    {
        return true;
    }
}

int main()
{
    Point p1 = Point(1, 1);
    Point p2 = Point(2, 3);
    std::cout << cross_product(p1, p2) << std::endl; // 1

    Point p3 = Point(1, 1);
    Point p4 = Point(2, 2);
    Point p5 = Point(3, 3);
    std::cout << three_points_on_same_line(p3, p4, p5) << std::endl; // 1
    std::cout << three_points_on_same_line(p2, p4, p5) << std::endl; // 0

    Segment s1 = Segment(Point(1, 1), Point(3, 3));
    Segment s2 = Segment(Point(2, 1), Point(2, 5));
    Segment s3 = Segment(Point(2, 2), Point(4, 4));
    Segment s4 = Segment(Point(4, 4), Point(5, 5));
    std::cout << segs_on_same_line(s1, s2) << std::endl;   // 0
    std::cout << same_line_intersect(s1, s3) << std::endl; // 1
    std::cout << same_line_intersect(s1, s4) << std::endl; // 0
    std::cout << same_line_intersect(s3, s4) << std::endl; // 1

    Segment vertical0 = Segment(Point(0, 0), Point(0, 2));
    Segment vertical1 = Segment(Point(1, 0), Point(1, 2));
    Segment horizontal1 = Segment(Point(0, 1), Point(1, 1));
    Segment horizontal2 = Segment(Point(0, 2), Point(1, 2));
    std::cout << segs_on_same_line(vertical0, vertical1) << std::endl; // 0

    std::cout << is_intersected(s1, s2) << std::endl;                 // 1
    std::cout << is_intersected(vertical0, vertical1) << std::endl;   // 0
    std::cout << is_intersected(vertical0, horizontal1) << std::endl; // 1
    std::cout << is_intersected(vertical0, horizontal2) << std::endl; // 1
    Segment unrelated = Segment(Point(100, 0), Point(110, 1));
    std::cout << is_intersected(vertical0, unrelated) << std::endl; // 0

    std::cout << adj_same_line_overlap(s3, s4) << std::endl; // 0

    Segment s5 = Segment(Point(2, 2), Point(4, 4));
    Segment s6 = Segment(Point(4, 4), Point(3, 3));
    std::cout << adj_same_line_overlap(s5, s6) << std::endl; // 1

    Segment s7 = Segment(Point(3, 3), Point(4, 4));
    Segment s8 = Segment(Point(4, 4), Point(3, 3));
    std::cout << "s6 == s7: " << (s6 == s7) << std::endl;
    std::cout << "s6 == s8: " << (s6 == s8) << std::endl;
    std::cout << "s5 == s6: " << (s5 == s6) << std::endl;

    std::cout << "===== Test same_line_overlap =====" << std::endl;
    Segment s9 = Segment(Point(0, 0), Point(5, 5));
    Segment s10 = Segment(Point(2, 2), Point(6, 6));
    Segment s11 = Segment(Point(5, 5), Point(6, 6));
    Segment s12 = Segment(Point(2, 2), Point(4, 4));
    Segment s13 = Segment(Point(7, 7), Point(8, 8));
    std::cout << same_line_overlap(s9, s10) << std::endl;  //1
    std::cout << same_line_overlap(s9, s11) << std::endl;  //0
    std::cout << same_line_overlap(s9, s12) << std::endl;  //1
    std::cout << same_line_overlap(s10, s11) << std::endl; //1
    std::cout << same_line_overlap(s9, s13) << std::endl;  //0
}

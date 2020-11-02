#ifndef SEGUTILITY_HPP
#define SEGUTILITY_HPP

#include "Point.hpp"
#include "Segment.hpp"

/* calculate the cross product of 2 vectors. */
int cross_product(const Point p1, const Point p2);

/* calculate the dot product of 2 vectors. */
int dot_product(const Point p1, const Point p2);

bool three_points_on_same_line(const Point p1, const Point p2, const Point p3);

// Segment is small, so pass-by-value
bool segs_on_same_line(const Segment s1, const Segment s2);

/* for non-adjacent segs in the same street: use it only when segs are on the same line */
bool same_line_intersect(const Segment s1, const Segment s2);

/* for adjacent segs in the same street: use it only when segs are on the same line */
bool adj_same_line_overlap(const Segment s1, const Segment s2);

bool straddle(const Segment s1, const Segment s2);

/* use it only when segs are not on the same line. 
   is_intersected algorithm reference: https://zhuanlan.zhihu.com/p/37360022
   judge if both segment cross the other line using cross product. */
bool is_intersected(const Segment s1, const Segment s2);

/* valid: return true */
bool adj_segs_valid(const Segment s1, const Segment s2);

/* valid: return true */
bool non_adj_segs_valid(const Segment s1, const Segment s2);

#endif
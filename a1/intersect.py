from itertools import combinations
import sys

def pp(x):
    """Returns a pretty-print string representation of a number.
       A float number is represented by an integer, if it is whole,
       and up to two decimal places if it isn't
    """
    if isinstance(x, float):
        if x.is_integer():
            return str(int(x))
        else:
            return "{0:.2f}".format(x)
    return str(x)


def cross_product(v1, v2):
    """
    calculate the cross product of 2 vectors
    :param v1: vector (represented by Point)
    :param v2: vector (represented by Point)
    """
    return v1.x * v2.y - v2.x * v1.y


class Point(object):
    """A point in a two dimensional space"""
    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __sub__(self, other):
        """
        :param other: another point
        :return: the vector from the other point to self
        """
        return Point(self.x - other.x, self.y - other.y)

    def __repr__(self):
        return '(' + pp(self.x) + ',' + pp(self.y) + ')'

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.x == other.x and self.y == other.y
        return False

    def __hash__(self):
        return self.x.__hash__() + self.y.__hash__()

    def dist(self, other):
        """
        calculate the square of distance between 2 Points
        :param other: the other
        :return:
        """
        return (self.x - other.x) ** 2 + (self.y - other.y) ** 2


def three_points_on_same_line(p1, p2, p3):
    """
    :param p1: Point
    :param p2: Point
    :param p3: Point
    :return: bool
    """
    return cross_product(p2 - p1, p3 - p1) == 0


class Segment(object):
    """
    A line segment between two points
    is_intersected algorithm reference: https://zhuanlan.zhihu.com/p/37360022
                                        judge if both segment cross the other line using cross product
    """
    def __init__(self, point1, point2):
        self.point1 = point1
        self.point2 = point2

    def __repr__(self):
        return '[' + str(self.point1) + '-->' + str(self.point2) + ']'

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return (self.point1 == other.point1 and self.point2 == other.point2) or \
                   (self.point1 == other.point2 and self.point2 == other.point1)
        return False

    def __hash__(self):
        return self.point1.__hash__() + self.point2.__hash__()

    def segs_on_same_line(self, seg):
        """
        :param seg: another segment
        :return: bool
        """
        return three_points_on_same_line(self.point1, self.point2, seg.point1) and \
            three_points_on_same_line(self.point1, self.point2, seg.point2)

    def overlaps(self, seg):
        """
        use it only when segments are on the same line
        :param seg: another segment
        :return: set(): intersection points.
                 empty when not overlap
                 len = 1 when overlaps at the end
                 len = 2 when overlaps a range
        """
        if self == seg:
            return {self.point1, self.point2}
        point_list = [self.point1, self.point2, seg.point1, seg.point2]
        point_set = set(point_list)     # set literal
        far_ends = set()
        max_dist = 0
        for pair in combinations(point_set, 2):
            dist = pair[0].dist(pair[1])
            if dist >= max_dist:
                max_dist = dist
                far_ends = {pair[0], pair[1]}

        for point in far_ends:
            point_list.remove(point)
        return set(point_list)

    def straddle(self, seg):
        """
        :param seg: another segment
        :return if the other segment cross the corresponding line of self, return true.
        """
        v = self.point2 - self.point1
        v1 = seg.point1 - self.point1
        v2 = seg.point2 - self.point1
        # == 0: when one end of a segment is on the other segment
        return cross_product(v1, v) * cross_product(v2, v) <= 0
        #     return True
        # else:
        #     return False

    def is_intersected(self, seg):
        """
        use it only when segments are not on the same line
        :param seg: another segment
        """
        if self.straddle(seg) and seg.straddle(self):
            return True
        else:
            return False

    def contains_point(self, point):
        """
        judge whether a point is on the segment
        :param point: another point
        :return: bool
        """
        v1 = point - self.point1
        v2 = point - self.point2
        if cross_product(v1, v2) != 0:  # point is not on the line of self
            return False

        x_min = min(self.point1.x, self.point2.x)
        x_max = max(self.point1.x, self.point2.x)
        if point.x < x_min or point.x > x_max:
            return False
        y_min = min(self.point1.y, self.point2.y)
        y_max = max(self.point1.y, self.point2.y)
        if point.y < y_min or point.y > y_max:
            return False
        return True


def intersect(l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.point1.x, l1.point1.y
    x2, y2 = l1.point2.x, l1.point2.y

    x3, y3 = l2.point1.x, l2.point1.y
    x4, y4 = l2.point2.x, l2.point2.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor = xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    return Point(xcoor, ycoor)


if __name__ == '__main__':
    # l1 = Segment(Point(1, 4), Point(5, 8))
    # l2 = Segment(Point(5, 6), Point(3, 8))
    # l3 = Segment(Point(1, 5), Point(5, 8))
    #
    # l4 = Segment(Point(0, 0), Point(0, 1))
    # l5 = Segment(Point(1, 1), Point(2, 2))
    # l6 = Segment(Point(-3, -4), Point(0, 1))
    #
    # print('Intersection of', l1, 'with', l2, 'is', intersect(l1, l2))
    # print('Intersection of', l2, 'with', l3, 'is', intersect(l2, l3))
    # print('Intersection of', l4, 'with', l5, 'is', intersect(l4, l5))   # wrong when not intersected
    # print('Intersection of', l4, 'with', l6, 'is', intersect(l4, l6))   # intersect at the end
    #
    # # test contains_point
    # l7 = Segment(Point(1, 1), Point(5, 5))
    # print(l7.contains_point(Point(1, 1)))
    # print(l7.contains_point(Point(2, 2)))
    # print(l7.contains_point(Point(6, 6)))
    # print(l7.contains_point(Point(2, 6)))
    #
    # # my own intersection tests
    # l8 = Segment(Point(1, 1), Point(5, 5))
    # l9 = Segment(Point(1, 1), Point(6, 7))
    # print(intersect(l8, l9))

    # print(Point(1, 2) == Point(1, 2))
    # print((1, 2) == Point(1, 2))

    # points = {Point(1, 2): 1, Point(3, 4): 2, Point(5, 6): 3}
    # print(Point(3, 4) in points)
    # print(Point(7, 8) in points)

    # segments are on the same line and overlap
    l10 = Segment(Point(0, 0), Point(1, 1))
    # l11 = Segment(Point(0, 0), Point(2, 2))
    # print(l10.is_intersected(l11)) # correct result: true
    # print(intersect(l10, l11))    # ZeroDivisionError

    # segments are on the same line and not overlap
    # l12 = Segment(Point(5, 5), Point(6, 6))
    # print(l10.is_intersected(l12))  # should not invoke this
    # print(intersect(l10, l12))    # ZeroDivisionError

    # parallel segments
    # l13 = Segment(Point(1, 0), Point(2, 1))
    # print(l10.is_intersected(l13))  # correct result: false
    # print(intersect(l10, l13))  # ZeroDivisionError, but does not matter. not invoke it after is_intersected returns False

    # l16 = Segment(Point(0, 0), Point(5, 5))
    # l17 = Segment(Point(5, 5), Point(0, 0))
    # print(l16 == l17)     # True

    # test overlaps: passed
    # l14 = Segment(Point(0, 0), Point(5, 5))
    # l15 = Segment(Point(1, 1), Point(6, 6))
    # l16 = Segment(Point(1, 1), Point(6, 6))
    # l17 = Segment(Point(0, 0), Point(8, 8))
    # l18 = Segment(Point(0, 0), Point(-1, -1))
    # print(l14.overlaps(l15))    # [(1,1), (5,5)]
    # print(l15.overlaps(l16))    # [(1,1), (6,6)]
    # print(l14.overlaps(l17))    # [(0,0), (5,5)]
    # print(l14.overlaps(l18))    # [(0,0)]

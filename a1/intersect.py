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
        return '(' + pp(self.x) + ', ' + pp(self.y) + ')'


class Segment(object):
    """
    A line segment between two points
    Reference: https://zhuanlan.zhihu.com/p/37360022
    is_intersected algorithm: judge if both segment cross the other line using cross product
    """
    def __init__(self, point1, point2):
        self.point1 = point1
        self.point2 = point2

    def __repr__(self):
        return '[' + str(self.point1) + '-->' + str(self.point2) + ']'

    def straddle(self, another_segment):
        """
        :return: if the other segment cross the corresponding line of self, return true.
        """
        v1 = another_segment.point1 - self.point1
        v2 = another_segment.point2 - self.point1
        vm = self.point2 - self.point1
        # == 0: when one end of a segment is on the other segment
        if cross_product(v1, vm) * cross_product(v2, vm) <= 0:
            return True
        else:
            return False

    def is_intersected(self, another_segment):
        if self.straddle(another_segment) and another_segment.straddle(self):
            return True
        else:
            return False


def intersect(l1, l2):
    """Returns a point at which two lines intersect"""
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y

    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y

    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    xcoor = xnum / xden

    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    ycoor = ynum / yden

    return Point(xcoor, ycoor)


if __name__ == '__main__':
    l1 = Segment(Point(1, 4), Point(5, 8))
    l2 = Segment(Point(5, 6), Point(3, 8))
    l3 = Segment(Point(1, 5), Point(5, 8))

    l4 = Segment(Point(0, 0), Point(0, 1))
    l5 = Segment(Point(1, 1), Point(2, 2))
    l6 = Segment(Point(-3, -4), Point(0, 1))

    print('Intersection of', l1, 'with', l2, 'is', intersect(l1, l2))
    print('Intersection of', l2, 'with', l3, 'is', intersect(l2, l3))
    print('Intersection of', l4, 'with', l5, 'is', intersect(l4, l5))   # wrong when not intersected
    print('Intersection of', l4, 'with', l6, 'is', intersect(l4, l6))   # intersect at the end

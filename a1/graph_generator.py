from street_db import StreetDB
from graph import Graph
from intersect import Point, Segment, intersect
from itertools import combinations


def gen_graph(street_db):
    """
    :param street_db
    :return Graph
    generate graph from scratch, not incrementally
    """
    count = 1
    streets = street_db.streets
    street_segs = street_db.convert_to_segs()   # to be completed
    graph = Graph()
    vertices = graph.vertices
    edges = graph.edges
    for pair in combinations(street_segs.items(), 2):
        # a pair example: ( ([Line(Point(12, 13), Point((14, 15)), Line(Point(14, 15), Point((16, 17))], "Street1"),
        #                   ([Line(Point(22, 23), Point((24, 25))], "Street2)
        #                  )
        for seg1 in pair[0][0]:
            for seg2 in pair[1][0]:
                if seg1.is_intersected(seg2):
                    intersection = intersect(seg1, seg2)
                    # if intersection not in vertices:
                    #     vertices[intersection] = count
                    #     count += 1
                    # if seg1.point1 not in vertices:
                    #     vertices[seg1.point1] = count
                    #     count += 1
                    # if seg1.point2 not in vertices:
                    #     vertices[seg1.point2] = count
                    #     count += 1
                    # if seg2.point1 not in vertices:
                    #     vertices[seg2.point1] = count
                    #     count += 1
                    # if seg2.point2 not in vertices:
                    #     vertices[seg2.point2] = count
                    #     count += 1
                    for point in (intersection, seg1.point1, seg1.point2, seg2.point1, seg2.point2):
                        if point not in vertices:
                            vertices[point] = count
                            count += 1
                        if point != intersection:
                            edges.append(Segment(point, intersection))
            # remove redundant edges
    return graph


if __name__ == '__main__':
    street_db = StreetDB()

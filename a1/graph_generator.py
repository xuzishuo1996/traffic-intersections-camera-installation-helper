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
    street_segs = street_db.convert_to_segs()   # to be completed
    graph = Graph()
    vertices = graph.vertices
    edges = graph.edges
    count = 1
    for pair in combinations(street_segs.items(), 2):
        # a pair example: ( ("Street1", [Line(Point(12, 13), Point((14, 15)), Line(Point(14, 15), Point((16, 17))]),
        #                   ("Street2", [Line(Point(22, 23), Point((24, 25))])
        #                  )
        for seg1 in pair[0][1]:
            for seg2 in pair[1][1]:
                on_same_line = seg1.segs_on_same_line(seg2)     # bool
                if on_same_line:
                    overlap_points = seg1.overlaps(seg2)
                    for point in overlap_points:
                        vertices[point] = count
                        count += 1
                    if len(overlap_points) == 1:
                        # edges.add(seg)  # edges: set
                        # edges: dict
                        if seg not in edges:  # do not repeat when 3 lines cross at a single point
                            edges.append(seg)
                elif seg1.is_intersected(seg2):
                    intersection = intersect(seg1, seg2)
                    for point in (intersection, seg1.point1, seg1.point2, seg2.point1, seg2.point2):
                        if point not in vertices:
                            vertices[point] = count
                            count += 1
                        if point != intersection:
                            seg = Segment(point, intersection)
                            # edges.add(seg)  # edges: set
                            # edges: dict
                            if seg not in edges:    # do not repeat when 3 lines cross at a single point
                                edges.append(seg)
    # remove redundant edges
    return graph


if __name__ == '__main__':
    street_db = StreetDB()

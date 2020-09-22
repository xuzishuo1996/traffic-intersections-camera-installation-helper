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
        for seg1 in pair[0][1]:
            for seg2 in pair[1][1]:
                if seg1.is_intersected(seg2):
                    intersection = intersect(seg1, seg2)
                    if intersection not in street_segs.values():
                        vertices[count] = intersection
                        count += 1
                    if intersection not in street_segs.values():
                        vertices[count] = seg1.point1
                        count += 1
                    if intersection not in street_segs.values():
                        vertices[count] = seg1.point2
                        count += 1
                    if intersection not in street_segs.values():
                        vertices[count] = seg2.point1
                        count += 1
                    if intersection not in street_segs.values():
                        vertices[count] = seg2.point2
                        count += 1
            # add edges: to be completed
    return graph


if __name__ == '__main__':
    street_db = StreetDB()
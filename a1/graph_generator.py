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
    streets = street_db.streets
    street_segs = street_db.convert_to_segs()   # to be completed
    graph = Graph()
    vertices = graph.vertices
    edges = graph.edges
    count = 1

    # add vertices and edges
    for pair in combinations(street_segs.items(), 2):
        # a pair example: ( ("Street1", [Line(Point(12, 13), Point((14, 15)), Line(Point(14, 15), Point((16, 17))]),
        #                   ("Street2", [Line(Point(22, 23), Point((24, 25))])
        #                  )
        for seg1 in pair[0][1]:
            for seg2 in pair[1][1]:
                if seg1.is_intersected(seg2):
                    intersection = intersect(seg1, seg2)
                    # deal with V
                    points1 = {intersection, seg1.point1, seg1.point2, seg2.point1, seg2.point2}
                    points2 = {seg1.point1, seg1.point2, seg2.point1, seg2.point2}
                    for point in points1:
                        if point not in vertices:
                            vertices[point] = count
                            count += 1
                            if point != intersection:
                                edges.add(Segment(point, intersection))
                                points2.remove(point)

                    # deal with E
                    for point in points2:
                        if point != intersection:
                            seg = Segment(point, intersection)
                            if seg not in edges:    # do not repeat when 3 lines cross at a single point
                                need_adding = True  # bool flag
                                for edge in edges:
                                    # if seg contains another edge
                                    if seg.contains_point(edge.point1) and seg.contains_point(edge.point2):
                                        need_adding = False
                                        break

                                    # if another edge contains seg
                                    elif edge.contains_point(intersection):     # edge cannot contain point
                                        if point == edge.point1:
                                            edges.add(seg)  # seg: Segment(edge.point1, intersection)
                                            if edge.point2 != intersection:
                                                edges.add(Segment(intersection, edge.point2))
                                            edges.remove(edge)
                                            need_adding = False
                                            break
                                        elif point == edge.point2:
                                            edges.add(seg)  # seg: Segment(point, intersection)
                                            if edge.point1 != intersection:
                                                edges.add(Segment(intersection, edge.point1))
                                            edges.remove(edge)
                                            need_adding = False
                                            break
                                if need_adding:
                                    edges.add(seg)
    return graph


# def gen_graph_edges_list(street_db):
#     """
#     :param street_db
#     :return Graph
#     generate graph from scratch, not incrementally
#     """
#     streets = street_db.streets
#     street_segs = street_db.convert_to_segs()   # to be completed
#     graph = Graph()
#     vertices = graph.vertices
#     edges = graph.edges
#     count = 1
#
#     # add vertices and edges
#     for pair in combinations(street_segs.items(), 2):
#         # a pair example: ( ("Street1", [Line(Point(12, 13), Point((14, 15)), Line(Point(14, 15), Point((16, 17))]),
#         #                   ("Street2", [Line(Point(22, 23), Point((24, 25))])
#         #                  )
#         for seg1 in pair[0][1]:
#             for seg2 in pair[1][1]:
#                 # on_same_line = seg1.segs_on_same_line(seg2)     # bool
#                 # if on_same_line:
#                 #     overlap_points = seg1.overlaps(seg2)
#                 #     if len(overlap_points) > 0:
#                 #         for point in overlap_points:
#                 #             if point not in vertices:
#                 #                 vertices[point] = count
#                 #                 count += 1
#                 #         for point in {seg1.point1, seg1.point2, seg2.point1, seg2.point2}:
#                 #             if point not in vertices:
#                 #                 vertices[point] = count
#                 #                 count += 1
#                 #             if point not in overlap_points:     # != intersections
#                 #                 for intersection in overlap_points:
#                 #                     seg = Segment(point, intersection)
#                 #                     # edges.add(seg)  # edges: set
#                 #                     # edges: dict
#                 #                     if seg not in edges:  # do not repeat when 3 lines cross at a single point
#                 #                         split = False   # bool flag
#                 #                         for edge in edges:
#                 #                             if edge.contains_point(intersection):
#                 #                                 if point == edge.point1:
#                 #                                     # if seg not in edges:
#                 #                                     edges.append(seg)  # Segment(point, intersection)
#                 #                                     if edge.point2 != intersection:
#                 #                                         another_seg = Segment(intersection, edge.point2)
#                 #                                         # if another_seg not in edges:
#                 #                                         edges.append(another_seg)
#                 #                                         edges.remove(edge)
#                 #                                     split = True
#                 #                                     break
#                 #                                 else:   # point == edge.point2:
#                 #                                     # if seg not in edges:
#                 #                                     edges.append(seg)  # Segment(point, intersection)
#                 #                                     if edge.point1 != intersection:
#                 #                                         another_seg = Segment(intersection, edge.point1)
#                 #                                         # if another_seg not in edges:
#                 #                                         edges.append(another_seg)
#                 #                                     edges.remove(edge)
#                 #                                     split = True
#                 #                                     break
#                 #                         if not split:
#                 #                             edges.append(seg)
#                 #
#                 # elif seg1.is_intersected(seg2):
#
#                 if seg1.is_intersected(seg2):
#                     intersection = intersect(seg1, seg2)
#                     if intersection not in vertices:
#                         vertices[intersection] = count
#                         count += 1
#                     points = {seg1.point1, seg1.point2, seg2.point1, seg2.point2}
#                     for point in points:
#                         if point not in vertices:
#                             vertices[point] = count
#                             count += 1
#                         if point != intersection:
#                             seg = Segment(point, intersection)
#                             edges.add(seg)  # edges: set
#                             # edges: dict
#                             if seg not in edges:    # do not repeat when 3 lines cross at a single point
#                                 split = False  # bool flag
#                                 for edge in edges:
#                                     # if egde contains seg
#                                     if edge.contains_point(intersection):
#                                         if point == edge.point1:
#                                             # if seg not in edges:
#                                             edges.append(seg)  # Segment(point, intersection)
#                                             if edge.point2 != intersection:
#                                                 another_seg = Segment(intersection, edge.point2)
#                                                 # if another_seg not in edges:
#                                                 edges.append(another_seg)
#                                             edges.remove(edge)
#                                             split = True
#                                             break
#                                         else:   # point == edge.point2:
#                                             # if seg not in edges:
#                                             edges.append(seg)  # Segment(point, intersection)
#                                             if edge.point1 != intersection:
#                                                 another_seg = Segment(intersection, edge.point1)
#                                                 # if another_seg not in edges:
#                                                 edges.append(another_seg)
#                                             edges.remove(edge)
#                                             split = True
#                                             break
#                                 if not split:
#                                     edges.append(seg)
#
#     # remove redundant edges
#
#     return graph


# if __name__ == '__main__':
#     street_db = StreetDB()

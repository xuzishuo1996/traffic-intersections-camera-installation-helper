class Graph:
    def __init__(self):
        self.vertices = {}  # dict (key: Point, value: int - sequence number)
        self.edges = []     # list of Segments

    def output(self):   # or __str__, __repr__ ?
        print("V = {")
        for item in self.vertices.items():
            # print("  " + str(item[0]) + ":  (" + str(item[1][0]) + "," + str(item[1][1]) + ")")
            print("  " + str(item[1]) + ":  (" + str(item[0].x) + "," + str(item[0].y) + ")")
        print("}")

        print("E = {")
        for item in self.edges:
            # print("  <" + str(item[0]) + "," + str(item[1]) + ">")
            print("  <" + str(self.vertices[item.point1]) + "," + str(self.vertices[item.point2]) + ">")
        print("}")


# # for test only - test graph.output()
# graph = Graph()
# # graph.vertices = {1: (1, 2), 2: (2, 3), 3: (3, 4)}
# # graph.edges = [(1, 3), (2, 3)]
# graph.vertices = {1: [1, 2], 2: [2, 3], 3: [3, 4]}
# graph.edges = [[1, 3], [2, 3]]
# graph.output()

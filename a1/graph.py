class Graph:
    def __init__(self):
        self.vertices = {}  # dict
        self.edges = []     # list

    def output(self):   # or __str__ ?
        print("V = {")
        for item in self.vertices.items():
            print("  " + str(item[0]) + ":  (" + str(item[1][0]) + "," + str(item[1][1]) + ")")
        print("}")

        print("E = {")
        for item in self.edges:
            print("  <" + str(item[0]) + "," + str(item[1]) + ">")
        print("}")


# for test only
graph = Graph()
# graph.vertices = {1: (1, 2), 2: (2, 3), 3: (3, 4)}
# graph.edges = [(1, 3), (2, 3)]
graph.vertices = {1: [1, 2], 2: [2, 3], 3: [3, 4]}
graph.edges = [[1, 3], [2, 3]]
graph.output()

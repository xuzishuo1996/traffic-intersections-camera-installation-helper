#!/usr/bin/env python3
import sys
from command_parser import parse_line
from street_db import StreetDB
from graph_generator import gen_graph
from graph import Graph

# YOUR CODE GOES HERE


def main():
    # # for test only
    # print("I am a1 child process!")

    # YOUR MAIN CODE GOES HERE
    street_db = StreetDB()
    graph = Graph()
    # count = 1

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    while True:
        line = sys.stdin.readline().strip()
        # line = input("input a command:\n")
        # # for test only
        # print("read a line:", line)
        if line == "":
            break
        cmd, val = parse_line(line, street_db)
        if cmd is None:
            continue
        if cmd == 'add':
            street_db.add(val)
        elif cmd == 'mod':
            street_db.modify(val)
        elif cmd == 'rm':
            street_db.remove(val[0])    # val is [] containing a single element
        else:  # gg
            count = 1
            # prev_vertices = graph.gen_output_vertices_dict()
            graph, count = gen_graph(street_db, count)
            # for test only
            # graph.output_street_vertices()
            graph.output()

        # for test only
        # print(street_db.streets)

    # print("Finished reading input")
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()

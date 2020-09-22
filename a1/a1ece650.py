#!/usr/bin/env python3
import sys
from command_parser import parse_line
from street_db import StreetDB
from graph_generator import gen_graph

# street_dict = {}


# YOUR CODE GOES HERE

def main():
    # YOUR MAIN CODE GOES HERE
    street_db = StreetDB()

    # sample code to read from stdin.
    # make sure to remove all spurious print statements as required
    # by the assignment
    while True:
        # line = sys.stdin.readline()
        line = input("input a command:")
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
            street_db.remove(val)
        else:   # gg
            graph = gen_graph()
            graph.output()
            # print("gg to be completed!")

        print(street_db.streets)

    # print("Finished reading input")
    # return exit code 0 on successful termination
    sys.exit(0)


if __name__ == "__main__":
    main()

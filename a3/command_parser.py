import re
import sys
from intersect import Point
from street_db import StreetDB

command_list = ['add', 'mod', 'rm', 'gg']


# Reference: https://stackoverflow.com/questions/5574702/how-to-print-to-stderr-in-python
def error_print(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr, flush=True)


def parse_line(line, street_db):
    """
    Parse an input line and return command and arguments.
    Print a message to stderr on error.

    :returns cmd: str
             args: list. [str, [(), (), ...]]. a list containing street name(str) and a list of point tuples
             returns None, None when input line is invalid.
    """
    cmd = None
    args = None
    # # already striped in main
    # striped = line.strip()
    split = line.split()
    # if len(split) == 0:
    #     error_print("Error: invalid input. should at least provide a command name.")
    #     return None, None

    # get the command
    cmd = split[0]
    if cmd not in command_list:
        error_print("Error: specified a command that does not exist.")
        return None, None

    # get the arguments
    arg_str = line[len(split[0]):].strip()
    if cmd == "gg":
        arg_str = line[len(split[0]):]
        if len(arg_str) != 0:
            error_print("Error: 'gg' should not be invoked with any argument.")
            return None, None
    else:       # get the street name
        if len(arg_str) == 0:
            error_print("Error: 'add'/'mod'/'rm' did not specify any arguments.")
            return None, None
        quote_pos = arg_str.find('"', 1)    # 1 - the second '"', idx start from 0
        if arg_str[0] != '"' or quote_pos == -1:    # first condition: add d" (1,2) (3,4)
            error_print("Error: 'add'/'mod'/'rm' did not specify a double-quoted street name.")
            return None, None
        else:
            street_name = arg_str[0: quote_pos + 1]  # include "". do not strip()
            # do not need the follwing for a3: can name the street whatever you want
            # street_pattern = r"(\"[a-zA-Z\s]*\")"
            # valid = re.match(street_pattern, street_name)
            # if valid is None:
            #     error_print("Error: street name should not contain characters other than letters and whitespaces.")
            #     return None, None
            street_name = street_name[1:-1].lower()
            # for test only
            # print("street name is: " + street_name)

            line_segments = arg_str[quote_pos + 1:].strip()
            if cmd == 'rm':
                # if street_name not in street_dict:
                if not street_db.contains(street_name):
                    error_print("Error: 'rm' specified a street that does not exist.")
                    return None, None
                if len(line_segments) != 0:
                    error_print("Error: 'rm' specified more than 1 argument.")
                    return None, None
                return cmd, [street_name]
            else:
                # if quote_pos == len(arg_str) - 1:
                #     error_print("Error: 'add'/'mod' did not specify line segments.")
                #     return None, None
                if len(line_segments) == 0:
                    error_print("Error: 'add'/'mod' did not specify line segments.")
                    return None, None

                # check: at least 1 space between street_name and line segments
                space_pattern = r"(\s)"
                valid = re.match(space_pattern, arg_str[quote_pos + 1])
                if valid is None:
                    error_print("Error: there should be at least 1 space between street_name and line segments.")
                    return None, None

                if cmd == 'add':
                    # if street_name in street_dict:
                    if street_db.contains(street_name):
                        error_print("Error: 'add' specified a street that already existed.")
                        return None, None
                    if street_name == "":
                        error_print("Error: 'add' specified an empty street name.")
                        return None, None
                else:   # 'mod'
                    # if street_name not in street_dict:
                    if not street_db.contains(street_name):
                        error_print("Error: 'mod' specified a street that does not exist.")
                        return None, None

                args = [street_name, []]
                # parse line segments
                points = line_segments.split(')')
                if points[-1] != "":    # example: (1,1) (2,2) asd
                    error_print("Error: 'add' or 'mod' specified an invalid line segment.")
                    return None, None
                # # for test only
                # print(points[-1])
                # print(points)
                points.pop()    # the last one is ''
                if len(points) == 0:
                    error_print("Error: 'add' or 'mod' specified an invalid line segment.")
                    return None, None
                # construct a pattern for (x,y) coordinates
                # pattern = r"(\(\s*-?\d+\s*,\s*-?\d+\s*\))"
                pattern = r"(\(\s*-?\d+\s*,\s*-?\d+\s*)"
                for point in points:
                    point_striped = point.strip()
                    valid = re.match(pattern, point_striped)
                    if not valid:
                        error_print("Error: 'add' or 'mod' specified an invalid line segment.")
                        return None, None
                    else:
                        point_str = valid.group()
                        if len(point_str) != len(point_striped):    # example: '(1, 2' and '(1, 2a'
                            error_print("Error: 'add' or 'mod' specified an invalid line segment.")
                            return None, None
                        # point_str = point_str.replace(' ', '')    # int() could handle whitespaces
                        # point_str = point_str.replace('\t', '')
                        result = get_coordinates(point_str + ')')
                        if len(args[1]) > 0 and result == args[1][-1]:  # two consecutive points are the same, skip it
                            continue
                        else:
                            args[1].append(result)
                if len(args[1]) == 1:
                    error_print("Error: 'add' or 'mod' specified only 1 point. at least 2 points.")
                    return None, None

    return cmd, args


def get_coordinates(point_str) -> Point:
    point_str = point_str[1:-1]     # remove '(' and ')'
    coordinates = point_str.split(',')
    point = Point(int(coordinates[0]), int(coordinates[1]))
    return point


# # for test_only
# while True:
#     my_line = input("input a command:\n")
#     my_cmd, my_val = parse_line(my_line)
#     if my_cmd:
#         print("cmd: " + my_cmd)
#     if my_val:
#         print("street name: " + my_val[0])
#         if len(my_val) > 1:
#             print("line segments:")
#             for lst in my_val[1]:
#                 print(' (' + str(lst[0]) + ',' + str(lst[1]) + ') ')

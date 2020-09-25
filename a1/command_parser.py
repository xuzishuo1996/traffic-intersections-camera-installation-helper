import re
from intersect import Point
from street_db import StreetDB
# from a1ece650 import street_dict

command_list = ['add', 'mod', 'rm', 'gg']

# # for test only
# street_dict = {"a", "b", "c"}


def parse_line(line, street_db):
    """
    Parse an input line and return command and arguments.
    Print a message on error.

    :returns cmd: str
             args: list. [str, [(), (), ...]]. a list containing street name(str) and a list of point tuples
             returns None, None when input line is invalid.
    """
    cmd = None
    args = None
    striped = line.strip()
    split = striped.split()
    if len(split) == 0:
        print("Error: invalid input. should at least provide a command name.")
        return None, None

    # get the command
    cmd = split[0]
    if cmd not in command_list:
        print("Error: specified a command that does not exist.")
        return None, None

    # get the arguments
    arg_str = striped[len(split[0]):].strip()
    if cmd == "gg":
        arg_str = striped[len(split[0]):]
        if not len(arg_str) == 0:
            print("Error: 'gg' should not be invoked with any argument.")
            return None, None
    else:       # get the street name
        quote_pos = arg_str.find('"', 1)    # 1 - the second '"', idx start from 0
        if arg_str[0] != '"' or quote_pos == -1:    # first condition: add d" (1,2) (3,4)
            print("Error: 'add'/'mod'/'rm' did not specify a double-quoted street name.")
            return None, None
        else:
            street_name = arg_str[0:quote_pos + 1]      # include "". do not strip()
            street_pattern = r"(\"[a-zA-Z\s]*\")"
            valid = re.match(street_pattern, street_name)
            if valid is None:
                print("Error: street name should not contain characters other than letters and whitespaces.")
            street_name = street_name[1:-1].lower()
            print("street name is: " + street_name)

            line_segments = arg_str[quote_pos + 1:].strip()
            if cmd == 'rm':
                # if street_name not in street_dict:
                if not street_db.contains(street_name):
                    print("Error: 'rm' specified a street that does not exist.")
                    return None, None
                if len(line_segments) != 0:
                    print("Error: 'rm' specified more than 1 argument.")
                    return None, None
                return cmd, [street_name]
            else:
                if cmd == 'add':
                    # if street_name in street_dict:
                    if street_db.contains(street_name):
                        print("Error: 'add' specified a street that already existed.")
                        return None, None
                    if street_name == "":
                        print("Error: 'add' specified an empty street name.")
                        return None, None
                else:   # 'mod'
                    # if street_name not in street_dict:
                    if not street_db.contains(street_name):
                        print("Error: 'mod' specified a street that does not exist.")
                        return None, None

                if len(line_segments) == 0:
                    print("Error: 'add'/'mod' did not specify line segments.")
                    return None, None

                args = [street_name, []]
                # parse line segments
                points = line_segments.split(')')
                points.pop()    # the last one is ''
                # construct a pattern for (x,y) coordinates
                # pattern = r"(\(\s*-?\d+\s*,\s*-?\d+\s*\))"
                pattern = r"(\(\s*-?\d+\s*,\s*-?\d+\s*)"
                for point in points:
                    point_striped = point.strip()
                    valid = re.match(pattern, point_striped)
                    if not valid:
                        print("Error: 'add' or 'mod' specified an invalid line segment.")
                        return None, None
                    else:
                        point_str = valid.group()
                        # point_str = point_str.replace(' ', '')    # int() could handle whitespaces
                        # point_str = point_str.replace('\t', '')
                        result = get_coordinates(point_str + ')')
                        if len(args[1]) > 0 and result == args[1][-1]:  # two consecutive points are the same, skip it
                            continue
                        else:
                            args[1].append(result)
                if len(args[1]) == 1:
                    print("Error: 'add' or 'mod' specified only 1 point. at least 2 points.")
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

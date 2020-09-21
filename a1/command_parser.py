import re
from a1ece650 import street_dict

command_list = ['add', 'mod', 'rm', 'gg']


def parse_line(line):
    """
    Parse an input line and return command and arguments.
    Print a message on error.

    :returns cmd, val. set cmd = None when input line is invalid.
    """
    sp = line.strip().split()
    arg_len = len(sp)
    val = None
    if arg_len == 0:
        print("Error: no input command. Please provide one.")
        cmd = None
        return cmd, val
    if sp[0] not in command_list:
        print("Error: Your input command is invalid. Valid commands include: 'add', 'mod', 'rm', 'gg'.")
        cmd = None
        return cmd, val
    # valid command name
    cmd = sp[0]
    if cmd == 'gg':
        if arg_len > 1:
            print("Error: 'gg' should not be invoked with any arg.")
            cmd = None
    elif cmd == 'rm':
        if arg_len != 1:    # invalid arg num
            print("Error: invalid arg number for 'rm'. should be 1: street name.")
            cmd = None
        else:   # valid arg num
            if sp[1] not in street_dict:
                print("Error: 'rm' specified a street name that does not exist.")
                cmd = None
            else:
                val = sp[1]
    else:   # 'add' and 'mod'
        if arg_len < 3:
            print("Error: invalid arg num for 'add' or 'mod'. should be 2: street name and line segments.")
            cmd = None
        else:
            if cmd == 'add' and sp[1] in street_dict:
                print("Error: 'add' specified a street name that already exists.")
                cmd = None
            elif cmd == 'mod' and sp[1] not in street_dict:
                print("Error: 'mod' specified a street name that does not exist.")
                cmd = None
            else:
                val = []
                val[0] = sp[1]
                val[1] = []
                # deal with street line segments
                # construct a pattern for (x,y) coordinates
                pattern = r"(\(\s?-?\d+\s?,\s?-?\d+\s?\))"
                for point in sp[2:]:
                    valid = re.match(pattern, point)
                    if not valid:
                        print("Error: 'add' or 'mod' specified an invalid line segment.")
                        cmd = None
                        return cmd, val
                    else:
                        point_str = valid.group().replace(" ", "")
                        point = get_coordinates(point_str)
                        val[1].append(point)

    return cmd, val


def get_coordinates(point_str):
    point_str = point_str[1:-1]     # remove '(' and ')'
    coordinates = point_str.split(',')
    point = (int(coordinates[0]), int(coordinates[1]))
    return point

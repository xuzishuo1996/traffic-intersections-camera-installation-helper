import re
# from a1ece650 import street_dict

command_list = ['add', 'mod', 'rm', 'gg']
street_dict = {"a", "b", "c"}


def parse_line(line):
    """
    Parse an input line and return command and arguments.
    Print a message on error.

    :returns cmd, val. return None, None when input line is invalid.
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
        quote_pos = arg_str.find('"', 1)
        if quote_pos == -1:
            print("Error: 'add'/'mod'/'rm' did not specify a double-quoted street name.")
            return None, None
        else:
            street_name = arg_str[1:quote_pos].strip()
            line_segments = arg_str[quote_pos + 1:].strip()
            if cmd == 'rm':
                if street_name not in street_dict:
                    print("Error: 'rm' specified a street that does not exist.")
                    return None, None
                if len(line_segments) != 0:
                    print("Error: 'rm' specified more than 1 argument.")
                    return None, None
                return cmd, [street_name]
            else:
                if cmd == 'add':
                    if street_name in street_dict:
                        print("Error: 'add' specified a street that already existed.")
                        return None, None
                    if street_name == "":
                        print("Error: 'add' specified an empty street name.")
                        return None, None
                else:   # 'mod'
                    if street_name not in street_dict:
                        print("Error: 'mod' specified a street that does not exist.")
                        return None, None

                if len(line_segments) == 0:
                    print("Error: 'add'/'mod' did not specify line segments.")
                    return None, None

                args = [street_name, []]
                # parse line segments
                points = line_segments.split()
                # construct a pattern for (x,y) coordinates
                pattern = r"(\(\s?-?\d+\s?,\s?-?\d+\s?\))"
                for point in points:
                    valid = re.match(pattern, point)
                    if not valid:
                        print("Error: 'add' or 'mod' specified an invalid line segment.")
                        return None, None
                    else:
                        point_str = valid.group().replace(" ", "")
                        result = get_coordinates(point_str)
                        args[1].append(result)

    return cmd, args


def get_coordinates(point_str):
    point_str = point_str[1:-1]     # remove '(' and ')'
    coordinates = point_str.split(',')
    point = (int(coordinates[0]), int(coordinates[1]))
    return point


# for test_only
while True:
    my_line = input("input a command:\n")
    my_cmd, my_val = parse_line(my_line)
    if my_cmd:
        print("cmd: " + my_cmd)
    if my_val:
        print("street name: " + my_val[0])
        if len(my_val) > 1:
            print("line segments:\n")
            for list in my_val[1]:
                print(' (' + str(list[0]) + ',' + str(list[1]) + ') ')


# deprecated
def parse_line_old(line):
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


import sys

class Matrix:
    def __init__(self, row=0, col=0, fill=0):
        """
        for question a)
        row: row number
        col: column number
        values is a 2-D list
        """
        self.row = row
        self.col = col
        self.values = [[fill] * self.col for i in range(row)]

    def get_row_number(self):
        """
        for question a)
        """
        return self.row

    def get_col_number(self):
        """
        for question a)
        """
        return self.col

    def get_item(self, row, col):
        """
        for question a)
        """
        if row < 0 or row >= self.row:
            raise Exception("row number is out of range!")
        if col < 0 or col >= self.col:
            raise Exception("row number is out of range!")
        return self.values[row][col]


def print_matrix(matrix):
    """
    for question b)
    """
    print(matrix.row)
    for row_list in matrix.values:
        s = "| "
        for value in row_list[:-1]:
            s = s + str(value) + ', '
        s = s + str(row_list[-1]) + " |"
        print(s)


def parse_matrix():
    """
    for question c)
    """
    row = int(sys.stdin.readline().strip())
    # input_values = sys.stdin.readline().strip('|').strip().split(',')
    input_values = sys.stdin.readline().strip().strip('|').strip().split(',')
    col = len(input_values)
    res = Matrix(row, col)
    res.values[0] = [int(value) for value in input_values]
    for i in range(1, row):
        input_values = sys.stdin.readline().strip().strip('|').strip().split(',')
        res.values[i] = [int(value) for value in input_values]
    return res


def matrix_mul(A, B):
    """
    for question d)
    """
    if A.col != B.row:
        raise Exception("multiplication between A and B is not possible")
    res = Matrix(A.row, B.col)
    for i in range(0, res.row):
        for j in range(0, res.col):
            res.values[i][j] = 0
            for idx in range(0, A.col):
                res.values[i][j] += A.values[i][idx] * B.values[idx][j]
    return res




# matrix = Matrix(2, 3)
# print_matrix(matrix)
parse_matrix()
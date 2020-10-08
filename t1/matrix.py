class Matrix:
    def __init__(self, row=0, col=0, fill=0.0):
        """
        for question a)
        """
        self.row = row
        self.column = col
        self.matrix = [[fill] * self.column for i in range(row)]

    def get_row_number(self):
        """
        for question a)
        """
        return self.row

    def get_column_number(self):
        """
        for question a)
        """
        return self.column

    def get_item(self, row, column):
        """
        for question a)
        """
        if row < 0 or row >= self.row:
            raise Exception("row number is out of range!")
        if column < 0 or column >= self.column:
            raise Exception("row number is out of range!")
        return self.matrix[row][column]

    

matrix = Matrix(2, 3)
print(matrix)
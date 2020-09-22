class StreetDB:
    def __init__(self):
        self.streets = {}

    def add(self, street):
        if street[0] not in self.streets.keys():
            self.streets[street[0]] = street[1]

    def modify(self, street):
        if street[0] in self.streets.keys():
            self.streets[street[0]] = street[1]

    def remove(self, street):
        if street[0] in self.streets.keys():
            del self.streets[street[0]]

    def contains(self, street) -> bool:
        return street[0] in self.streets.keys()

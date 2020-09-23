from graph import Graph
from intersect import Segment


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

    def convert_to_segs(self) -> dict:
        """
        :return: dict. (key: str - street name, val: [] - segments)
        """
        street_segs = {}
        for item in self.streets.items():
            segs = []
            for idx, point in enumerate(item[1][:-1]):
                segs.append(Segment(point, item[1][idx + 1]))
            street_segs[item[0]] = segs
        return street_segs

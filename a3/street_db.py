from graph import Graph
from intersect import Segment, Point


class StreetDB:
    def __init__(self):
        self.streets = {}   # dict. (key: str - street name, val: [] - Points)

    def add(self, street):
        if street[0] not in self.streets.keys():
            self.streets[street[0]] = street[1]

    def modify(self, street):
        if street[0] in self.streets.keys():
            self.streets[street[0]] = street[1]

    def remove(self, street_name):
        if street_name in self.streets.keys():
            del self.streets[street_name]

    def contains(self, street_name) -> bool:
        return street_name in self.streets.keys()

    def convert_to_segs(self) -> dict:
        """
        :return: dict. {key: str - street name, val: set() of Segment}
        """
        street_segs = {}
        for item in self.streets.items():
            # segs = []
            segs = set()
            for idx, point in enumerate(item[1][:-1]):
                # segs.append(Segment(point, item[1][idx + 1]))
                seg = Segment(point, item[1][idx + 1])
                if seg not in segs:
                    segs.add(seg)
            street_segs[item[0]] = segs
        return street_segs


if __name__ == '__main__':
    street_db = StreetDB()
    street_db.streets["King Street S"] = [Point(1, 2), Point(3, 4)]
    print(street_db.streets["King Street S"])
    street_db.remove("King Street S")

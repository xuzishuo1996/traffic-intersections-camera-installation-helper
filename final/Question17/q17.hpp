class Point
{
    int x;
    int y;
    bool inf;
    bool nac;

    Point(int _x, int _y) : x(_x), y(_y), inf(false), nac(false) {}
    Point(bool _inf, bool _pos) : inf(_inf), nac(!_inf), y(0) { x = _pos ? 1 : -1; }

public:
    bool isInf() const { return inf; }
    bool isNaC() const { return nac; }
    bool isPos() const { return x >= 1; }
    bool getX() const { return x; }
    bool getY() const { return y; }

    // Signature for equality for part (b)
    bool eq(const Point &other) const;
    // Signature for weak order for part (d)
    bool lt(const Point &other) const;
    // Signature for hash function for part (e)
    unsigned hash() const;
};
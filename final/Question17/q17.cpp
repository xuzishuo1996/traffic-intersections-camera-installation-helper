#include "q17.hpp"
#include <climits>

// Signature for equality for part (b)
bool Point::eq(const Point &other) const
{
    if (isInf())
    {
        if (!other.isInf())
        {
            return false;
        }
        else
        {
            return (isPos() == other.isPos());
        }
    }
    if (isNaC())
    {
        return other.isNaC();
    }
    return ((x == other.getX()) && (y == other.getY()));
}

// Signature for weak order for part (d)
bool Point::lt(const Point &other) const
{
    // Nac is the biggest
    if (isNaC())
    {
        if (other.isNaC())
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    if (other.isNaC())
    {
        return true;
    }

    if (isInf())
    {
        if (other.isInf())
        {
            return (!isPos() && other.isPos()); // this is negative inf, the other is positive inf
        }
        else
        {
            return !isPos(); // this is negative inf
        }
    }

    if (x < other.getX())
    {
        return true;
    }
    else if (x > other.getX())
    {
        return false;
    }
    else
    {
        return y < other.getY();
    }
}

// Signature for hash function for part (e)
// Please also see the picture for explanation
unsigned Point::hash() const
{
    if (isInf())
    {
        return UINT_MAX;
    }
    if (isNaC())
    {
        return 0;
    }
    return static_cast<unsigned int>(x) +
           429496751 * static_cast<unsigned int>(y); // arithmetic operations wrap on overflow
}
#include <iostream>
#include <sstream>
#include <vector>
// #include <utility>

#include <cmath>

// typedef std::pair<unsigned, unsigned> Factor; // (factor, its num)
typedef struct
{
    unsigned first;  // factor
    unsigned second; // num
} Factor;

std::string
prime_factor(unsigned x)
{
    std::stringstream out;

    // YOUR CODE GOES HERE

    // deal with x = 0, x = 1, and x is prime?

    std::vector<Factor> res;

    if (x % 2 == 0)
    {
        Factor fac;
        fac.first = 2;
        fac.second = 1;
        x /= 2;
        while (x % 2 == 0)
        {
            ++fac.second;
            x /= 2;
        }
        res.push_back(fac);
    }

    if (x > 1)
    {
        for (unsigned i = 3; i <= sqrt(x); i += 2)
        {
            if (x % i != 0)
            {
                continue;
            }
            Factor fac;
            fac.first = i;
            fac.second = 1;
            x /= i;
            while (x % i == 0)
            {
                ++fac.second;
                x /= i;
            }
            res.push_back(fac);
            if (x == 1) // break early
            {
                break;
            }
        }
        if (x > 1)
        {
            Factor fac;
            fac.first = x;
            fac.second = 1;
            res.push_back(fac);
        }
    }

    // input is prime
    if (res.size() == 0)
    {
        return "";
    }

    out << res[0].first;
    if (res[0].second > 1)
    {
        out << "^" << res[0].second;
    }

    for (auto iter = res.begin() + 1; iter != res.end(); ++iter)
    {
        out << " x " << (*iter).first;
        if ((*iter).second > 1)
        {
            out << "^" << (*iter).second;
        }
    }

    return out.str();
}

int main()
{

    auto input = 9;
    auto output = prime_factor(input);

    std::cout << "Prime factorization of " << input << " is " << output
              << std::endl;
    // expected output: 3^2

    input = 240;
    output = prime_factor(input);

    std::cout << "Prime factorization of " << input << " is " << output
              << std::endl;
    // expected output: 2^4 x 3 x 5
    return 0;
}

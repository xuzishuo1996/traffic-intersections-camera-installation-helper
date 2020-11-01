#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "Point.hpp"
#include "Segment.hpp"

int main(int argc, char **argv)
{
    // // for test only: driver-to-rgen argument passing
    // std::cout << "I am rgen child process!" << std::endl;
    // std::cout << "argc: " << argc << std::endl;
    // for (int i = 1; i < argc; ++i)
    // {
    //     std::cout << argv[i] << std::endl;
    // }
    // std::cout << std::endl;

    // set default args
    unsigned max_num_of_streets = 10;
    unsigned max_num_of_segs = 5;
    unsigned max_interval = 5;
    int max_coordinate_abs = 20;

    // parse args: expected options are '-s', '-n', '-l' and '-c' and corresponding values
    int option;
    while ((option = getopt(argc, argv, "s:n:l:c:")) != -1)
    {
        // for test only
        std::cout << "option is: " << (char)option << std::endl;

        std::string tmp_val;
        tmp_val = optarg;
        int opt_val = atoi(tmp_val.c_str());

        switch (option)
        {
        case 's':
            max_num_of_streets = opt_val;
            break;
        case 'n':
            max_num_of_segs = opt_val;
            break;
        case 'l':
            max_interval = opt_val;
            break;
        case 'c':
            max_coordinate_abs = opt_val;
            break;
        default:
            break;
        }
    }
    // // for test only
    // std::cout << "[-s] max_num_of_streets: " << max_num_of_streets << std::endl;
    // std::cout << "[-n] max_num_of_segs: " << max_num_of_segs << std::endl;
    // std::cout << "[-l] max_interval: " << max_interval << std::endl;
    // std::cout << "[-c] max_coordinate_abs: " << max_coordinate_abs << std::endl;

    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");
    // check that it did not fail
    if (urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }

    int prev_size = 0; // the num of streets in the previous graph, for rm cmd
    while (true)
    {
        // define random numbers
        unsigned num_of_streets;
        unsigned num_of_segs;
        unsigned interval;
        int x, y;

        // gen num_of_streets: [2, max_num_of_streets]
        urandom.read((char *)&num_of_streets, sizeof(unsigned));
        num_of_streets = num_of_streets % (max_num_of_streets - 1) + 2;
        // std::cout << "Random num_of_streets: " << num_of_streets << "\n";

        // gen interval: [5, max_interval]
        urandom.read((char *)&interval, sizeof(unsigned));
        interval = interval % (max_interval - 4) + 5;
        // std::cout << "Random interval: " << interval << "\n";

        std::vector<std::vector<Point>> streets;

        // gen streets
        for (int i = 0; i < num_of_streets; ++i)
        {
            // gen num_of_segs: [1, max_num_of_segs]
            urandom.read((char *)&num_of_segs, sizeof(unsigned));
            num_of_segs = num_of_segs % max_num_of_segs + 1;
            // std::cout << "Random num_of_segs: " << num_of_segs << "\n";

            std::vector<Point> street;

            for (int j = 0; j <= num_of_segs; ++i) // note: coorindates number = num_of_segs + 1
            {

                // gen coordinates: [-max_coordinate_abs, max_coordinate_abs]
                urandom.read((char *)&x, sizeof(int));
                x = x % (max_coordinate_abs + 1);
                // std::cout << "Random x: " << x << "\n";

                urandom.read((char *)&y, sizeof(int));
                y = y % (max_coordinate_abs + 1);
                // std::cout << "Random y: " << y << "\n";

                // check streets
                street.push_back(Point(x, y));
            }
            streets.push_back(street);
        }

        // set street name = count number, do not need to store it, just store previous size
        // issue rm
        for (int i = 0; i < prev_size; ++i)
        {
            std::cout << "rm \"" << i << "\"" << std::endl;
        }
        // issue add
        for (int i = 0; i < streets.size(); ++i)
        {
            std::cout << "add \"" << i << "\" ";
            // TODO: output the points
        }
        // issue gg
        std::cout << "gg" << std::endl;

        prev_size = streets.size();

        // wait for a few seconds
        sleep(interval);
    }

    // for test only
    // std::cout << "add \"Weber Street\" (2,-1) (2,2) (5,5) (5,6) (3,8)" << std::endl;
    // std::cout << "add \"King Street S\" (4, 2)(4, 8)" << std::endl;
    // std::cout << "add \"Davenport Road\" (1,4) (5,8)" << std::endl;
    // std::cout << "gg" << std::endl;

    // close random stream
    urandom.close();
    return 0;
}
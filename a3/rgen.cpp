#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "Point.hpp"
#include "Segment.hpp"
#include "SegUtility.hpp"

// Reference: ece650 cpp demo repo - https://git.uwaterloo.ca/ece650-1209/cpp

int main(int argc, char **argv)
{
    // // for test only: driver-to-rgen argument passing
    // std::cerr << "I am rgen child process!" << std::endl;
    // std::cerr << "argc: " << argc << std::endl;
    // for (int i = 0; i < argc; ++i)
    // {
    //     std::cerr << argv[i] << std::endl;
    // }
    // std::cerr << std::endl;

    // set default args
    unsigned max_num_of_streets = 10;
    unsigned max_num_of_segs = 5;
    unsigned max_interval = 5;
    int max_coordinate_abs = 20;

    int max_num_of_streets_int;
    int max_num_of_segs_int;
    int max_interval_int;

    // parse args: expected options are '-s', '-n', '-l' and '-c' and corresponding values
    int option;
    opterr = 0;
    while ((option = getopt(argc, argv, "s:n:l:c:")) != -1)
    {
        // // for test only
        // std::cerr << "enter parsing option: " << (char)option << std::endl;

        std::string tmp_val;
        switch (option)
        {
        case 's':
            tmp_val = optarg;
            max_num_of_streets_int = atoi(tmp_val.c_str());
            if (max_num_of_streets_int < 0)
            {
                std::cerr << "Error: the value of the -s must >= 2." << std::endl;
                exit(1);
            }
            max_num_of_streets = max_num_of_streets_int;
            break;
        case 'n':
            tmp_val = optarg;
            max_num_of_segs_int = atoi(tmp_val.c_str());
            if (max_num_of_segs_int < 0)
            {
                std::cerr << "Error: the value of the -n must >= 1." << std::endl;
                exit(1);
            }
            max_num_of_segs = max_num_of_segs_int;
            break;
        case 'l':
            tmp_val = optarg;
            max_interval_int = atoi(tmp_val.c_str());
            if (max_interval_int < 0)
            {
                std::cerr << "Error: the value of the -l must >= 5." << std::endl;
                exit(1);
            }
            max_interval = max_interval_int;
            break;
        case 'c':
            tmp_val = optarg;
            max_coordinate_abs = atoi(tmp_val.c_str());
            if (max_coordinate_abs < 0)
            {
                std::cerr << "Error: the value of the -c must >= 0." << std::endl;
                exit(1);
            }
            break;
        case '?':
            if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
                std::cerr << "Error: option -" << (char)optopt << " requires an argument." << std::endl;
            else
                std::cerr << "Error: unknown option: " << (char)optopt << std::endl;
            exit(1);
        default:
            std::cerr << "Error: invalid option." << std::endl;
            exit(1);
        }
    }

    // for test only
    std::cerr << "[-s] max_num_of_streets: " << max_num_of_streets << std::endl;
    std::cerr << "[-n] max_num_of_segs: " << max_num_of_segs << std::endl;
    std::cerr << "[-l] max_interval: " << max_interval << std::endl;
    std::cerr << "[-c] max_coordinate_abs: " << max_coordinate_abs << std::endl;

    if (max_num_of_streets < 2)
    {
        std::cerr << "I am here: max_num_of_streets" << std::endl;
        std::cerr << "Error: the value of the '-s' option must >= 2." << std::endl;
        exit(1);
    }
    if (max_num_of_segs < 1)
    {
        std::cerr << "Error: the value of the '-n' option must >= 1." << std::endl;
        exit(1);
    }
    if (max_interval < 5)
    {
        std::cerr << "Error: the value of the '-l' option must >= 5." << std::endl;
        exit(1);
    }

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
        std::vector<std::vector<Segment>> streets_in_segs;

        // gen streets
        for (unsigned i = 0; i < num_of_streets; ++i)
        {
            // gen num_of_segs: [1, max_num_of_segs]
            urandom.read((char *)&num_of_segs, sizeof(unsigned));
            num_of_segs = num_of_segs % max_num_of_segs + 1;
            // std::cout << "Random num_of_segs: " << num_of_segs << "\n";

            std::vector<Point> street;
            std::vector<Segment> segs;

            for (unsigned j = 0; j <= num_of_segs; ++j) // note: coorindates number = num_of_segs + 1
            {
                bool succeed;
                for (int k = 0; k < 25; ++k)
                {
                    succeed = true;
                    // gen coordinates: [-max_coordinate_abs, max_coordinate_abs]
                    urandom.read((char *)&x, sizeof(int));
                    x = x % (max_coordinate_abs + 1);
                    // std::cout << "Random x: " << x << "\n";
                    urandom.read((char *)&y, sizeof(int));
                    y = y % (max_coordinate_abs + 1);
                    // std::cout << "Random y: " << y << "\n";

                    Point p = Point(x, y);
                    Segment seg;
                    // check validity
                    if (street.size() > 0)
                    {
                        // check that 2 adjacent points are not the same
                        if (p == street[street.size() - 1])
                        {
                            succeed = false;
                        }
                        // check that the new segment does not intersect with previous segs in the same street
                        if (succeed)
                        {
                            seg = Segment(street[street.size() - 1], p);
                            // the previous (adjacent) seg in the same street
                            if (segs.size() > 0 && !adj_segs_valid(segs[segs.size() - 1], seg))
                            {
                                succeed = false;
                            }
                            // other (non - adjacent) segs in the same street
                            for (int t = 0; t < (int)segs.size() - 1; ++t)
                            {
                                if (!non_adj_segs_valid(segs[t], seg))
                                {
                                    succeed = false;
                                    break;
                                }
                            }
                        }
                        // check that the new segment does not overlap with segs in other (previous) streets
                        if (succeed)
                        {
                            for (std::vector<Segment> &prev_street : streets_in_segs)
                            {
                                for (Segment &prev_seg : prev_street)
                                {
                                    if (!diff_streets_segs_valid(prev_seg, seg))
                                    {
                                        succeed = false;
                                        break;
                                    }
                                }
                                if (!succeed)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    // if the new segment is valid, add it
                    if (succeed)
                    {
                        street.push_back(p);
                        if (street.size() > 0)
                        {
                            segs.push_back(seg);
                        }
                        break;
                    }
                }

                if (!succeed)
                {
                    std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << std::endl;
                    exit(1);
                }
            }
            streets.push_back(street);
            streets_in_segs.push_back(segs);
        }

        // set street name = count number, do not need to store it, just store previous size
        // issue rm
        for (int i = 0; i < prev_size; ++i)
        {
            std::cout << "rm \"" << i << "\"" << std::endl;
        }
        // issue add
        for (unsigned i = 0; i < streets.size(); ++i)
        {
            std::cout << "add \"" << i << "\" "; // street name is "i"
            // output the points in a street
            std::vector<Point> &street = streets[i];
            std::vector<Point>::const_iterator iter;
            for (iter = street.begin(); iter != street.end() - 1; ++iter)
            {
                std::cout << *iter << " ";
            }
            std::cout << *iter << std::endl;
        }
        // issue gg
        std::cout << "gg" << std::endl;

        prev_size = streets.size();

        // wait for a few seconds
        sleep(interval);
    }

    // // for test only
    // std::cout << "add \"Weber Street\" (2,-1) (2,2) (5,5) (5,6) (3,8)" << std::endl;
    // std::cout << "add \"King Street S\" (4, 2)(4, 8)" << std::endl;
    // std::cout << "add \"Davenport Road\" (1,4) (5,8)" << std::endl;
    // std::cout << "gg" << std::endl;

    // close random stream
    urandom.close();
    return 0;
}
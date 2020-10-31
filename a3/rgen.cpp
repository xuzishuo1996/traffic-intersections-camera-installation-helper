#include <iostream>
#include <unistd.h>

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
    // for test only
    std::cout << "[-s] max_num_of_streets: " << max_num_of_streets << std::endl;
    std::cout << "[-n] max_num_of_segs: " << max_num_of_segs << std::endl;
    std::cout << "[-l] max_interval: " << max_interval << std::endl;
    std::cout << "[-c] max_coordinate_abs: " << max_coordinate_abs << std::endl;

    // while (true)
    // {
    //     // gen random numbers
    //     // gen streets
    //     // check streets
    //     // issue rm
    //     // issue add
    //     // issue gg
    //     std::cout << "gg" << std::endl;
    //     // wait for a few seconds
    // }

    // for test only
    // std::cout << "add \"Weber Street\" (2,-1) (2,2) (5,5) (5,6) (3,8)" << std::endl;
    // std::cout << "add \"King Street S\" (4, 2)(4, 8)" << std::endl;
    // std::cout << "add \"Davenport Road\" (1,4) (5,8)" << std::endl;
    // std::cout << "gg" << std::endl;

    return 0;
}
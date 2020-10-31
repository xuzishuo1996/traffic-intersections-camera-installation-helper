#include <iostream>

int main(int argc, char **argv)
{
    // for test: driver-to-rgen argument passing
    std::cout << "I am rgen child process!" << std::endl;
    std::cout << "argc: " << argc << std::endl;
    for (int i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
    std::cout << std::endl;

    // while (true)
    // {
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
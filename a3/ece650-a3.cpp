#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_OF_CHILD_PROC 3

// driver: exec a2 locally
int main(int argc, char *argv[])
{
    pid_t kid[NUM_OF_CHILD_PROC]; // for rgen, a1 and a2

    int RgenToA1[2]; // one-way pipe from rgen to a1
    pipe(RgenToA1);

    int InputToA2[2]; // one-way pipe from a1 and user-input to a2
    pipe(InputToA2);

    /* a2 */
    // char *a2Args[0];
    // kid[2] = fork();

    // if (kid[2] == 0) // child process: a2
    // {
    //     dup2(A1ToA2[0], STDIN_FILENO);
    //     close(A1ToA2[0]);
    //     close(A1ToA2[1]);

    // dup2(DriverToA2[0], STDIN_FILENO);
    // close(DriverToA2[0]);
    // close(DriverToA2[1]);

    // execv("./build/ece650-a2", a2Args);
    //     execv("./ece650-a2", a2Args);
    //     return 0;
    // }
    // else if (kid[2] < 0) // fail to fork
    // {
    //     std::cerr << "Error: could not fork\n";
    //     return 1;
    // }

    /* a1 */
    char *a1Args[3];
    a1Args[0] = (char *)"python3";
    a1Args[1] = (char *)"../ece650-a1.py"; // curr path is in build/, so use ../
    a1Args[2] = nullptr;                   // have to include ending nullptr

    kid[1] = fork();
    if (kid[1] == 0) // child process: a1
    {
        std::cout << "forked a1" << std::endl;

        dup2(RgenToA1[0], STDIN_FILENO);
        close(RgenToA1[0]);
        close(RgenToA1[1]);

        // dup2(A1ToA2[1], STDOUT_FILENO);
        // close(A1ToA2[0]);
        // close(A1ToA2[1]);

        execv("/usr/bin/python3", a1Args);
        return 0;
    }
    else if (kid[1] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    /* rgen */
    kid[2] = fork();
    if (kid[2] == 0) // child process: rgen
    {
        std::cout << "forked rgen" << std::endl;

        dup2(RgenToA1[1], STDOUT_FILENO);
        close(RgenToA1[0]);
        close(RgenToA1[1]);

        execv("./rgen", argv); // ./ is build/
        return 0;
    }
    else if (kid[2] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    // int DriverToA2[2]; // one-way pipe from driver to a2: for s (shortest-path) command
    // pipe(DriverToA2);

    // dup2(DriverToA2[1], STDOUT_FILENO);
    // close(DriverToA2[0]);
    // close(DriverToA2[1]);

    // // accept s commands
    // while (!std::cin.eof())
    // {
    //     std::string line;
    //     std::getline(std::cin, line);
    //     std::cout << line << std::endl; // redirect s commands to a2
    // }

    // sleep(10);
    std::cout << "I am the parent proc!" << std::endl;

    int res[NUM_OF_CHILD_PROC];
    for (int i = 1; i < 3; ++i)
    {
        waitpid(kid[i], &res[i], 0);
    }

    return 0;
}

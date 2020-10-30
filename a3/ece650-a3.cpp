#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_OF_CHILD_PROC 3

// driver: exec a2 locally
int main(int argc, char *argv[])
{
    pid_t kid[NUM_OF_CHILD_PROC]; // for rgen, a1 and a2

    int RgenToA1[2]; // one-way pipe from rgen to a1
    pipe(RgenToA1);

    int A1ToA2[2]; // one-way pipe from a1 to a2 (curr proc)
    pipe(A1ToA2);

    int DriverToA2[2]; // one-way pipe from driver to a2: for s (shortest-path) command
    pipe(DriverToA2);

    kid[0] = fork();
    if (kid[0] == 0) // child process: rgen
    {
        // dup2(RgenToA1[1], STDOUT_FILENO);
        close(RgenToA1[0]);
        close(RgenToA1[1]);

        execv("./build/run/bin/rgen", argv);
        return 0;
    }
    else if (kid[0] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    char *a1Args[1];
    a1Args[1] = (char *)"./ece650-a1.py";

    kid[1] = fork();
    if (kid[1] == 0) // child process: a1
    {
        dup2(RgenToA1[0], STDIN_FILENO);
        close(RgenToA1[0]);
        close(RgenToA1[1]);

        dup2(A1ToA2[1], STDOUT_FILENO);
        close(A1ToA2[0]);
        close(A1ToA2[1]);

        execv("python3", a1Args);
        return 0;
    }
    else if (kid[1] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    char *a2Args[0];
    kid[2] = fork();
    if (kid[2] == 0) // child process: a1
    {
        dup2(A1ToA2[0], STDIN_FILENO);
        close(A1ToA2[0]);
        close(A1ToA2[1]);

        // dup2(DriverToA2[0], STDIN_FILENO);
        // close(DriverToA2[0]);
        // close(DriverToA2[1]);

        execv("./build/run/bin/ece650-a2", a2Args);
        return 0;
    }
    else if (kid[2] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

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

    int res[NUM_OF_CHILD_PROC];
    for (int i = 0; i < NUM_OF_CHILD_PROC; ++i)
    {
        waitpid(kid[i], &res[i], 0);
    }
    return 0;
}

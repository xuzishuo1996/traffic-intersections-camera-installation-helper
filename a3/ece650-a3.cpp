#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_OF_CHILD_PROC 3

// Reference: ece650 cpp demo repo - https://git.uwaterloo.ca/ece650-1209/cpp

pid_t kid[NUM_OF_CHILD_PROC]; // for rgen, a1 and a2

// rgen exit signal handler
void rgen_exit_handler(int arg)
{
    // std::cerr << "the signal is: " << arg << std::endl;  // 17: SIGCHLD
    for (int i = 0; i < NUM_OF_CHILD_PROC - 1; ++i)
    {
        kill(kid[i], SIGTERM);
    }

    int res[NUM_OF_CHILD_PROC - 1];
    for (int i = 0; i < NUM_OF_CHILD_PROC - 1; ++i)
    {
        waitpid(kid[i], &res[i], 0);
    }
    exit(0);
}

int main(int argc, char **argv)
{
    int RgenToA1[2]; // one-way pipe from rgen to a1
    pipe(RgenToA1);

    int InputToA2[2]; // one-way pipe from a1 and user-input to a2
    pipe(InputToA2);

    /* a2 */
    char *a2Args[1];
    a2Args[0] = nullptr;
    kid[0] = fork();
    if (kid[0] == 0) // child process: a2
    {
        // // for test only
        // std::cout << "forked a2" << std::endl;

        dup2(InputToA2[0], STDIN_FILENO);
        close(InputToA2[0]);
        close(InputToA2[1]);

        execv("./ece650-a2", a2Args);
        return 0;
    }
    else if (kid[0] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    /* a1 */
    /* It works, but it's better to make .py executable */
    char *a1Args[3];
    a1Args[0] = (char *)"python3";
    a1Args[1] = (char *)"./ece650-a1.py";
    a1Args[2] = nullptr; // have to include ending nullptr

    // char *a1Args[1];
    // a1Args[0] = nullptr;

    kid[1] = fork();
    if (kid[1] == 0) // child process: a1
    {
        // // for test only
        // std::cout << "forked a1" << std::endl;

        dup2(RgenToA1[0], STDIN_FILENO);
        close(RgenToA1[0]);
        close(RgenToA1[1]);

        dup2(InputToA2[1], STDOUT_FILENO);
        close(InputToA2[0]);
        close(InputToA2[1]);

        execv("/usr/bin/python3", a1Args);
        // execv("../ece650-a1.py", a1Args);    // does not work
        return 0;
    }
    else if (kid[1] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    /* rgen */
    signal(SIGCHLD, rgen_exit_handler); // register signal callback for rgen exit on 25 attempts

    // char *rgenArgs[9];
    // int i;
    // for (i = 1; i < argc; ++i)
    // {
    //     rgenArgs[i - 1] = argv[i];
    // }
    // rgenArgs[i - 1] = nullptr;

    kid[2] = fork();
    if (kid[2] == 0) // child process: rgen
    {
        // // for test only
        // std::cout << "forked rgen" << std::endl;

        dup2(RgenToA1[1], STDOUT_FILENO);
        close(RgenToA1[0]);
        close(RgenToA1[1]);

        // execv("./rgen", rgenArgs); // ./ is build/
        execv("./rgen", argv); // ./ is build/
        return 0;
    }
    else if (kid[2] < 0) // fail to fork
    {
        std::cerr << "Error: could not fork\n";
        return 1;
    }

    dup2(InputToA2[1], STDOUT_FILENO);
    close(InputToA2[0]);
    close(InputToA2[1]);

    // accept s commands
    while (!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);
        std::cout << line << std::endl; // redirect s commands to a2
    }

    // kill
    for (int i = 0; i < NUM_OF_CHILD_PROC; ++i)
    {
        kill(kid[i], SIGTERM);
    }

    int res[NUM_OF_CHILD_PROC];
    for (int i = 0; i < NUM_OF_CHILD_PROC; ++i)
    {
        waitpid(kid[i], &res[i], 0);
    }

    return 0;
}

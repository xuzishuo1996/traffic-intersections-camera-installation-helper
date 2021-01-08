// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
// defined std::unique_ptr
#include <memory>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "vertex_cover.hpp"
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// Reference: https://git.uwaterloo.ca/ece650-1209/minisat

pthread_t thr[3];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
    // // -- allocate on the heap so that we can reset later if needed
    // std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    // Graph *graph;

    // read from stdin until EOF
    while (!std::cin.eof())
    {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);

        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);

        // while there are characters in the input line
        while (!input.eof())
        {
            char cmd;
            // parse the command name
            input >> cmd;     // ignore leading whitespaces
            if (input.fail()) // input contains only whitespaces
            {
                break;
            }

            arg_for_thread_struct arg_for_thread;
            if (cmd == 'V')
            {
                input >> arg_for_thread.vertex_num;
                // graph = new Graph(vertex_num);
                break;
            }
            else if (cmd == 'E')
            {
                std::string remaining;
                input >> remaining;

                set_edges(remaining, arg_for_thread);
                if (arg_for_thread.edges.empty())
                {
                    std::cout << "CNF-SAT-VC:\nAPPROX-VC-1:\nAPPROX-VC-2:" << std::endl;
                }
                else
                {
                    // vertex-cover code comes in here
                    // create a thread for each algorithm
                    // method 1: CNF-SAT-VC Vertex-Cover
                    int retcode = 0;
                    retcode = pthread_create(&(thr[0]), nullptr, &cnf_sat_vc, (arg_for_thread_struct *)&arg_for_thread);
                    if (retcode)
                    {
                        std::cerr << "pthread_create cnf_sat_vc: " << strerror(retcode) << std::endl;
                        exit(1);
                    }
                    // method 2: Vertex-Cover Approximation 1
                    retcode = pthread_create(&(thr[1]), nullptr, &approx_vc_1, (arg_for_thread_struct *)&arg_for_thread);
                    if (retcode)
                    {
                        std::cerr << "pthread_create approx_vc_1: " << strerror(retcode) << std::endl;
                        exit(1);
                    }
                    // method 3: Vertex-Cover Approximation 2
                    retcode = pthread_create(&(thr[2]), nullptr, &approx_vc_2, (arg_for_thread_struct *)&arg_for_thread);
                    if (retcode)
                    {
                        std::cerr << "pthread_create approx_vc_2: " << strerror(retcode) << std::endl;
                        exit(1);
                    }

                    // wait the 3 threads to complete
                    retcode = pthread_join(thr[0], nullptr);
                    if (retcode)
                    {
                        std::cerr << "pthread_join cnf_sat_vc: " << strerror(retcode) << std::endl;
                        exit(1);
                    }
                    retcode = pthread_join(thr[1], nullptr);
                    if (retcode)
                    {
                        std::cerr << "pthread_join approx_vc_1: " << strerror(retcode) << std::endl;
                        exit(1);
                    }
                    retcode = pthread_join(thr[2], nullptr);
                    if (retcode)
                    {
                        std::cerr << "pthread_join approx_vc_2: " << strerror(retcode) << std::endl;
                        exit(1);
                    }
                }
                break;
            }
        }
    }

    int retcode = pthread_mutex_destroy(&lock);
    if (retcode)
    {
        std::cerr << "mutex_destroy, " << strerror(retcode) << std::endl;
        exit(1);
    }

    return 0;
}
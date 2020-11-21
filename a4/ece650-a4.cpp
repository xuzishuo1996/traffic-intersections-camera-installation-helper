// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> // std::find_if
#include <regex>
// defined std::unique_ptr
#include <memory>
#include "Graph.hpp"
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// Reference: https://git.uwaterloo.ca/ece650-1209/minisat

int main(int argc, char **argv)
{
    // // -- allocate on the heap so that we can reset later if needed
    // std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    Graph *graph;

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

            if (cmd == 'V')
            {
                unsigned vertex_num;
                input >> vertex_num;
                graph = new Graph(vertex_num);
                break;
            }
            else if (cmd == 'E')
            {
                std::vector<Edge> edges;
                std::string remaining;
                input >> remaining;
                graph->set_edges(remaining);
                if (graph->edges_is_empty())
                {
                    std::cout << std::endl;
                }
                else
                {
                    graph->get_vertex_cover();
                    // graph->get_vertex_cover(solver);

                    // // the next line de-allocates existing solver and allocates a new
                    // // one in its place.
                    // solver.reset(new Minisat::Solver());
                }
                break;
            }
        }
    }

    return 0;
}
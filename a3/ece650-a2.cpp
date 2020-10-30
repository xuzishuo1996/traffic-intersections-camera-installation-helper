// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> // std::find_if
#include <regex>
#include "Graph.hpp"

std::vector<Edge> gen_edges_from_input(std::string input, unsigned idx_limit)
{
    std::vector<Edge> edges; // return value

    // input from a1 is always legal: do not need it
    // // check '-': ensure there are no vertices with a negative reference number
    // std::string::iterator pos = std::find_if(input.begin(), input.end(), [](char ch) {
    //     return ch == '-';
    // });
    // if (pos != input.end())
    // {
    //     std::cerr << "Error: edges include a vertex does not exist." << std::endl;
    //     edges = {};
    //     return edges;
    // }

    std::regex pat{R"((\d+)\s*,\s*(\d+))"};
    std::sregex_iterator iter(input.begin(), input.end(), pat);

    for (; iter != std::sregex_iterator{}; ++iter)
    {
        Edge edge;
        // below 5 lines are adapted from: https://stackoverflow.com/questions/236129/how-do-i-iterate-over-the-words-of-a-string
        std::istringstream iss((*iter)[0]);
        std::string item;
        for (unsigned i = 0; i < 2; ++i)
        {
            if (std::getline(iss, item, ','))
            {
                unsigned v = (unsigned)stoi(item);
                if (v > idx_limit || v <= 0)
                {
                    std::cerr << "Error: edges include a vertex does not exist." << std::endl;
                    edges = {};
                    return edges;
                }
                if (i == 0)
                {
                    edge.first = v;
                }
                else
                {
                    edge.second = v;
                }
            }
            else
            {
                std::cerr << "Error: the command is invalid." << std::endl;
                edges = {};
                return edges;
            }
        }

        if (edge.first != edge.second)
        {
            edges.push_back(edge);
        }
    }

    // // for test only
    // std::cout << "output edges: " << '\n';
    // std::cout << "edges size(): " << edges.size() << '\n';
    // for (std::vector<Edge>::const_iterator iter = edges.begin(); iter != edges.end(); ++iter)
    // {
    //     std::cout << '<' << (*iter).first << ',' << (*iter).second << "> ";
    // }
    // std::cout << std::endl;

    return edges;
}

int main(int argc, char **argv)
{
    Graph *graph;

    // read from stdin until EOF
    while (!std::cin.eof())
    {
        // // print a promt
        // std::cout << "Enter a command" << std::endl;

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
                // std::cerr << "Error: the command is invalid 1." << std::endl;
                break;
            }

            if (cmd == 'V')
            {
                unsigned vertex_num;
                input >> vertex_num;
                if (input.fail())
                {
                    std::cerr << "Error: the command is invalid 2." << std::endl;
                    break;
                }
                graph = new Graph(vertex_num);
                break;
            }
            else if (cmd == 'E')
            {
                std::vector<Edge> edges;
                std::string remaining;
                input >> remaining;
                edges = gen_edges_from_input(remaining, graph->get_vertex_num());
                if (!edges.empty())
                {
                    graph->set_edges(edges);
                }
                std::cout << "V " << graph->get_vertex_num() << std::endl;
                std::cout << "E " << remaining << std::endl;
                break;
            }
            else if (cmd == 's')
            {
                unsigned src, dst;
                input >> src;
                if (input.fail())
                {
                    std::cerr << "Error: the command is invalid 3." << std::endl;
                    break;
                }
                input >> dst;
                if (input.fail())
                {
                    std::cerr << "Error: the command is invalid 4." << std::endl;
                    break;
                }
                graph->get_shortest_path(src, dst);
                break;
            }
            else
            {
                std::cerr << "Error: the command is invalid 5" << std::endl;
                break;
            }
        }
    }
}
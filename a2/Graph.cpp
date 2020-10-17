#include "Graph.hpp"
#include <iostream>
#include <queue>
#include <climits>
// #include <array>

Graph::Graph(){};

// set the capacity of adj_list to vertex_num + 1, index 0 is not used for convenience
Graph::Graph(unsigned vertex_num) : vertex_num(vertex_num), adj_list({vertex_num + 1})
{
    // adj_list.reserve(vertex_num);
}

Graph::~Graph()
{
}

// store edges from the input to the Graph data structure
void Graph::set_edges(const std::vector<Edge> &edges)
{
    // iter has to be const because 'edges' is const
    for (std::vector<Edge>::const_iterator iter = edges.begin(); iter != edges.end(); ++iter)
    {
        Edge curr = *iter;
        adj_list[curr.first].push_back(curr.second);
        adj_list[curr.second].push_back(curr.first);
    }
}

// use breath-first-search because the graph is unweighted
void Graph::get_shortest_path(unsigned src, unsigned dst)
{
    unsigned num = adj_list.size() - 1;
    if (src > num || dst > num)
    {
        std::cerr << "Error: the input source or destination does not exist." << std::endl;
        return;
    }

    if (src == dst)
    {
        std::cout << src << '-' << src << std::endl;
        return;
    }

    // unsigned path_table[num + 1];
    // std::array<unsigned, num> path_table{10, 20, 30}; // the second para in the template must have a constant value

    // store the previous vertex of the find shortest path algorithm.
    std::vector<unsigned> path_table(num + 1, UINT_MAX);
    path_table[src] = src; // mark source vertex's previous vertex as itself

    std::queue<unsigned> q;
    q.push(src);

    while (!q.empty())
    {
        unsigned curr = q.front();
        q.pop();

        unsigned neighbor;
        // No need to add a set to record visited vertices
        for (std::vector<unsigned>::const_iterator iter = adj_list[curr].begin(); iter != adj_list[curr].end(); ++iter)
        {
            neighbor = *iter;
            if (path_table[neighbor] == UINT_MAX) // has not been visited
            {
                path_table[neighbor] = curr;
                if (neighbor == dst)
                {
                    break;
                }
                q.push(neighbor);
            }
        }
        if (neighbor == dst)
        {
            break;
        }
    }

    // generate the path sequence from the path table
    std::vector<unsigned> path_seq;
    if (path_table[dst] == UINT_MAX)
    {
        std::cerr << "Error: there is no path between the source and the destination." << std::endl;
        return;
    }
    path_seq.push_back(dst);
    unsigned curr_v = dst;
    while (curr_v != src)
    {
        curr_v = path_table[curr_v];
        path_seq.push_back(curr_v);
    }

    // print the result
    std::vector<unsigned>::const_reverse_iterator iter;
    for (iter = path_seq.rbegin(); iter != path_seq.rend() - 1; ++iter)
    {
        std::cout << *iter << '-';
    }
    std::cout << *iter << std::endl;
}

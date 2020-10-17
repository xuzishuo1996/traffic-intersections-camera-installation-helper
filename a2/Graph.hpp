#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility> // pair is in it

typedef std::pair<unsigned, unsigned> Edge;

class Graph
{
private:
    unsigned vertex_num;
    std::vector<std::vector<unsigned>> adj_list; // reference or direct

public:
    Graph(); // default constructor
    Graph(unsigned vertex_num);
    virtual ~Graph(); // virtual destructor

    unsigned inline get_vertex_num() const
    {
        return vertex_num;
    }
    void set_edges(const std::vector<Edge> &edges); // store edges from the input to the Graph data structure
    void get_shortest_path(unsigned src, unsigned dst);
};

#endif
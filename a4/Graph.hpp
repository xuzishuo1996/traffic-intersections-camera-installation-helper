#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <utility> // pair is in it
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

typedef std::pair<unsigned, unsigned> Edge;

class Graph
{
private:
    unsigned vertex_num;
    std::vector<Edge> edges;
    // std::vector<std::vector<unsigned>> adj_list; // reference or direct

public:
    Graph(); // default constructor
    Graph(unsigned vertex_num);
    virtual ~Graph(); // virtual destructor

    inline unsigned get_vertex_num() const
    {
        return vertex_num;
    }
    inline bool edges_is_empty() const
    {
        return edges.empty();
    }
    void set_edges(std::string input); // store edges from the input to the Graph data structure
    void get_vertex_cover();
};

#endif
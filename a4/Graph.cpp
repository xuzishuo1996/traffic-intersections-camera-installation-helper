#include "Graph.hpp"
#include <iostream>
#include <queue>
#include <climits>
#include <memory>
#include <algorithm>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include "minisat/mtl/Vec.h"

Graph::Graph(){};

// set the capacity of adj_list to vertex_num + 1, index 0 is not used for convenience
Graph::Graph(unsigned vertex_num) : vertex_num(vertex_num)
{
}

Graph::~Graph()
{
}

// store edges from the input to the Graph data structure
void Graph::set_edges(const std::vector<Edge> &input_edges)
{
    edges = input_edges;

    // // for test only
    // for (unsigned i = 0; i < edges.size(); ++i)
    // {
    //     std::cout << "(" << edges[i].first << "," << edges[i].second << ")" << std::endl;
    // }
}

// // store edges from the input to the Graph data structure
// void Graph::set_edges(const std::vector<Edge> &edges)
// {
//     // iter has to be const because 'edges' is const
//     for (std::vector<Edge>::const_iterator iter = edges.begin(); iter != edges.end(); ++iter)
//     {
//         Edge curr = *iter;
//         adj_list[curr.first].push_back(curr.second);
//         adj_list[curr.second].push_back(curr.first);
//     }
// }

// reduction from Vertex-Cover to CNF-SAT and use minisat to solve it
// use n * k literals
void Graph::get_vertex_cover()
{
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    unsigned n = vertex_num;
    // k - number of vertex-cover
    for (unsigned k = 1; k <= n; ++k)
    {
        // create literals
        Minisat::vec<Minisat::Lit> lits; // n * k
        for (unsigned i = 0; i < n * k; ++i)
        {
            lits.push(Minisat::mkLit(solver->newVar()));
        }

        // create clauses
        // 1. each pos in vertex cover should refer to one of the vertex: x1 ∨ y1 ∨ z1
        // k    n-clause(clause with n literals)
        for (unsigned j = 0; j < k; ++j)
        {
            Minisat::vec<Minisat::Lit> v;
            for (unsigned i = 0; i < n; ++i)
            {
                v.push(lits[i * k + j]);
            }
            solver->addClause(v);
        }

        // 2. No one vertex can appear twice in a vertex cover
        // n * k*(k-1)/2    2-clause
        for (unsigned i = 0; i < n; ++i)
        {
            for (unsigned p = 0; p < k - 1; ++p)
            {
                for (unsigned q = p + 1; q < k; ++q)
                {
                    solver->addClause(~lits[i * k + p], ~lits[i * k + q]);
                }
            }
        }

        // 3. No more than one vertex appears in the same position of the vertex cover
        // n*(n-1)/2 * k    2-clause
        for (unsigned p = 0; p < n - 1; ++p)
        {
            for (unsigned q = p + 1; q < n; ++q)
            {
                for (unsigned j = 0; j < k; ++j)
                {
                    solver->addClause(~lits[p * k + j], ~lits[q * k + j]);
                }
            }
        }

        // 4. each edge should contain at least one vertex that is included in the vertex-cover
        // |E|  2*k clause
        for (Edge &edge : edges)
        {
            Minisat::vec<Minisat::Lit> v;
            for (unsigned p = 0; p < k; ++p)
            {
                v.push(lits[(edge.first - 1) * k + p]);
                v.push(lits[(edge.second - 1) * k + p]);
            }
            solver->addClause(v);
        }

        bool res = solver->solve();
        if (res) // satisfiable
        {
            // check model (literal values)
            std::vector<int> cover;
            for (unsigned i = 0; i < n; ++i)
            {
                for (unsigned j = 0; j < k; ++j)
                {
                    if (solver->modelValue(lits[i * k + j]) == Minisat::l_True)
                    {
                        cover.push_back(i + 1);
                        break;
                    }
                }
                if (cover.size() == k)
                {
                    break;
                }
            }

            // sort in ascending order
            std::sort(cover.begin(), cover.end());
            // output the result
            unsigned i;
            for (i = 0; i < cover.size() - 1; ++i)
            {
                std::cout << cover[i] << " ";
            }
            std::cout << cover[i] << std::endl;

            break;
        }

        // the next line de-allocates existing solver and allocates a new
        // one in its place.
        solver.reset(new Minisat::Solver());
    }
}
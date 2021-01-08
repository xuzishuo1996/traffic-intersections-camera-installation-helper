#ifndef VERTEX_COVER_HPP
#define VERTEX_COVER_HPP

#include <vector>
#include <unordered_set>
#include <utility> // pair is in it
#include <memory>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

typedef std::pair<unsigned, unsigned> Edge;

typedef struct
{
    unsigned vertex_num;
    std::vector<Edge> edges;
} arg_for_thread_struct;

// signal handler for CNF-SAT vertex cover timeout
void timeout_handler(int signum);

// set edges in arg_for_thread from the input
void set_edges(const std::string &input, arg_for_thread_struct &arg_for_thread);

// reduction from Vertex-Cover to CNF-SAT and use minisat to solve it
// use n * k literals
void *cnf_sat_vc(void *arg);
// approximation vertex cover: pick an edge in the remaining adjacent list
void *approx_vc_1(void *arg);
// approximation vertex cover: pick the vertex with the highest degree
void *approx_vc_2(void *arg);

#endif
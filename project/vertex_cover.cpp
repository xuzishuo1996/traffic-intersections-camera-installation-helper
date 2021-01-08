#include "vertex_cover.hpp"
#include <iostream>
#include <queue>
#include <climits>
#include <memory>
#include <algorithm>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <regex>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include "minisat/mtl/Vec.h"

#define TIMEOUT 5 // timeout second for CNF-SAT-VC

extern pthread_t thr[3];
extern pthread_mutex_t lock;

// signal handler reference: https://linuxhint.com/sigalarm_alarm_c_language/
void timeout_handler(int signum)
{
    // cancel current active alarm
    alarm(0);

    // get mutex
    int retcode = pthread_mutex_lock(&lock);
    // if (retcode)
    // {
    //     std::cerr << "timeout handler mutex lock"
    //               << ", " << strerror(retcode) << std::endl;
    //     exit(1);
    // }

    // if not terminated, ...
    retcode = pthread_kill(thr[0], 0);
    if (retcode == 0) // thr[0] still exists
    {
        retcode = pthread_cancel(thr[0]);
        if (retcode != 0)
        {
            std::cerr << "fail to cancel timeout CNF-SAT-VC: " << strerror(retcode) << std::endl;
            exit(1);
        }
        std::cout << "CNF-SAT-VC: timeout" << std::endl;
    }

    retcode = pthread_mutex_unlock(&lock);
    // if (retcode)
    // {
    //     std::cerr << "timeout handler mutex unlock"
    //               << ", " << strerror(retcode) << std::endl;
    //     exit(1);
    // }
}

// set edges in arg_for_thread from the input
void set_edges(const std::string &input, arg_for_thread_struct &arg_for_thread)
{
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
                arg_for_thread.edges = {};
            }
        }

        if (edge.first != edge.second)
        {
            arg_for_thread.edges.push_back(edge);
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
}

// approximation vertex cover: pick the vertex with the highest degree
void *approx_vc_1(void *arg)
{
    arg_for_thread_struct *arg_for_thread = (arg_for_thread_struct *)arg;
    const std::vector<Edge> &edges = arg_for_thread->edges;                             // const reference
    std::vector<std::unordered_set<unsigned>> adj_list{arg_for_thread->vertex_num + 1}; // adj_list[0] is not used for convenience. vertex idx start from 1

    // fill in the adjacent list for approx_1
    for (const Edge &edge : edges)
    {
        adj_list[edge.first].insert(edge.second);
        adj_list[edge.second].insert(edge.first);
    }
    // // for test only
    // for (unsigned i = 1; i < adj_list.size(); ++i)
    // {
    //     if (!adj_list[i].empty())
    //     {
    //         std::cout << i << ": ";
    //         for (auto iter = adj_list[i].begin(); iter != adj_list[i].end(); ++iter)
    //         {
    //             std::cout << *iter << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }

    std::vector<unsigned> cover;

    while (true)
    {
        // find the vertex with max degree
        unsigned max_degree = 0;
        unsigned max_vertex = 0;
        for (unsigned i = 1; i < adj_list.size(); ++i)
        {
            unsigned degree = adj_list[i].size();
            // std::cout << i << " size: " << degree << std::endl;
            if (degree > max_degree)
            {
                max_degree = degree;
                max_vertex = i;
            }
        }
        if (max_degree == 0)
        {
            break;
        }
        // std::cout << " max_vertex: " << max_vertex << std::endl;
        cover.push_back(max_vertex);

        // remove edges incident to max_vertex
        for (unsigned i : adj_list[max_vertex])
        {
            auto iter = adj_list[i].find(max_vertex);
            adj_list[i].erase(iter);
        }
        adj_list[max_vertex].clear();
    }

    std::sort(cover.begin(), cover.end());

    // output the result
    int retcode = pthread_mutex_lock(&lock);
    if (retcode)
    {
        std::cerr << "approx_vc_1 mutex lock"
                  << ", " << strerror(retcode) << std::endl;
        exit(1);
    }
    std::cout << "APPROX-VC-1: ";
    unsigned i = 0;
    for (i = 0; i < cover.size() - 1; ++i)
    {
        std::cout << cover[i] << ",";
    }
    std::cout << cover[i] << std::endl;
    retcode = pthread_mutex_unlock(&lock);
    if (retcode)
    {
        std::cerr << "approx_vc_1 mutex unlock"
                  << ", " << strerror(retcode) << std::endl;
        exit(1);
    }

    return nullptr;
}

// approximation vertex cover: pick an edge in the remaining adjacent list
void *approx_vc_2(void *arg)
{
    arg_for_thread_struct *arg_for_thread = (arg_for_thread_struct *)arg;
    const std::vector<Edge> &edges = arg_for_thread->edges;                                          // const reference
    std::vector<std::unordered_set<unsigned>> adj_list_for_approx_2{arg_for_thread->vertex_num + 1}; // adj_list_for_approx_2[0] is not used for convenience. vertex idx start from 1
    std::unordered_set<unsigned> idx_not_empty;

    // fill in the adjacent list and not_empty_index_set_of_adjacent_list for approx_2
    for (const Edge &edge : edges)
    {
        adj_list_for_approx_2[edge.first].insert(edge.second);
        idx_not_empty.insert(edge.first);
    }
    // // for test only
    // for (unsigned i = 1; i < adj_list_for_approx_2.size(); ++i)
    // {
    //     if (!adj_list_for_approx_2[i].empty())
    //     {
    //         std::cout << i << ": ";
    //         for (auto iter = adj_list_for_approx_2[i].begin(); iter != adj_list_for_approx_2[i].end(); ++iter)
    //         {
    //             std::cout << *iter << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // }

    std::vector<unsigned> cover; // result

    // pick an edge(v1, v2): pick the first in the remaining adj_list
    while (!idx_not_empty.empty())
    {
        // std::cout << "size: " << idx_not_empty.size() << std::endl;
        // auto iter1 = idx_not_empty.begin();
        // std::cout << *iter1 << std::endl;
        // // no need to do that, already checked
        // while (adj_list_for_approx_2[*iter1].empty())
        // {
        //     ++iter1;
        // }
        unsigned v1 = *(idx_not_empty.begin());
        unsigned v2 = *(adj_list_for_approx_2[v1].begin());
        // add v1 and v2 to the cover result
        cover.push_back(v1);
        cover.push_back(v2);

        // remove v1 and v2 in the adj_list_for_approx_2
        adj_list_for_approx_2[v1].clear();
        adj_list_for_approx_2[v2].clear();
        // remove v1 and v2 in the idx_not_empty set
        idx_not_empty.erase(v1); // remove v1
        idx_not_empty.erase(v2); // remove v2

        // remove edges attached to v1 and v2 (that are not stored in [v1] and [v2])
        std::vector<unsigned> empty_idx_to_rm;
        for (unsigned v : idx_not_empty)
        {
            // it's ok to erase an non-existed element or erase from an empty set
            adj_list_for_approx_2[v].erase(v1);
            adj_list_for_approx_2[v].erase(v2);
            if (adj_list_for_approx_2[v].empty())
            {
                empty_idx_to_rm.push_back(v);
            }
        }
        for (unsigned v : empty_idx_to_rm)
        {
            idx_not_empty.erase(v);
        }
    }

    std::sort(cover.begin(), cover.end());

    // output the result
    int retcode = pthread_mutex_lock(&lock);
    if (retcode)
    {
        std::cerr << "approx_vc_2 mutex lock"
                  << ", " << strerror(retcode) << std::endl;
        exit(1);
    }
    std::cout << "APPROX-VC-2: ";
    unsigned i = 0;
    for (i = 0; i < cover.size() - 1; ++i)
    {
        std::cout << cover[i] << ",";
    }
    std::cout << cover[i] << std::endl;
    retcode = pthread_mutex_unlock(&lock);
    if (retcode)
    {
        std::cerr << "approx_vc_2 mutex unlock"
                  << ", " << strerror(retcode) << std::endl;
        exit(1);
    }

    return nullptr;
}

// reduction from Vertex-Cover to CNF-SAT and use minisat to solve it
// use n * k literals
void *cnf_sat_vc(void *arg)
{
    // for timeout
    // default state is cancel enabled
    // int ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    // if (ret != 0)
    // {
    //     std::cerr << "fail to enable cancellation of CNF-SAT-VC: " << strerror(ret) << std::endl;
    //     exit(1);
    // }
    int ret = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if (ret != 0)
    {
        std::cerr << "fail to enable cancellation of CNF-SAT-VC: " << strerror(ret) << std::endl;
        exit(1);
    }

    // set timeout for CNF-SAT-VC
    signal(SIGALRM, timeout_handler);
    alarm(TIMEOUT); // one off timer

    arg_for_thread_struct *arg_for_thread = (arg_for_thread_struct *)arg;
    unsigned n = arg_for_thread->vertex_num;
    const std::vector<Edge> &edges = arg_for_thread->edges;
    // // for test only
    // for (unsigned i = 0; i < edges.size(); ++i)
    // {
    //     std::cout << "(" << edges[i].first << "," << edges[i].second << ")" << std::endl;
    // }

    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

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
        for (const Edge &edge : edges)
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
            // cancel current active alarm
            alarm(0);

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
            int retcode = pthread_mutex_lock(&lock);
            if (retcode)
            {
                std::cerr << "cnf-sat-vc mutex lock"
                          << ", " << strerror(retcode) << std::endl;
                exit(1);
            }
            std::cout << "CNF-SAT-VC: ";
            unsigned i;
            for (i = 0; i < cover.size() - 1; ++i)
            {
                std::cout << cover[i] << ",";
            }
            std::cout << cover[i] << std::endl;
            retcode = pthread_mutex_unlock(&lock);
            if (retcode)
            {
                std::cerr << "cnf-sat-vc mutex unlock"
                          << ", " << strerror(retcode) << std::endl;
                exit(1);
            }

            solver.reset(); // delete the managed Solver
            break;
        }

        // the next line de-allocates existing solver and allocates a new
        // one in its place.
        solver.reset(new Minisat::Solver());
    }

    return nullptr;
}

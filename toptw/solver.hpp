#pragma once

#include "problem.hpp"
#include "evaluate.hpp"
#include "construct.hpp"
#include "local_search.hpp"
#include <iostream>
#include <iomanip>
#include <set>
#include <algorithm>

inline void _report(const std::vector<std::vector<int>>& solution,
                    const Problem& prob, double score, bool verbose) {
    if (!verbose) return;

    int start = prob.depot_start;
    int end = prob.depot_end;

    std::vector<int> all_visited;
    for (const auto& r : solution) {
        for (int node : r) all_visited.push_back(node);
    }

    std::set<int> dupes;
    std::set<int> seen;
    for (int v : all_visited) {
        if (v == start || v == end) continue;
        if (seen.count(v)) dupes.insert(v);
        seen.insert(v);
    }

    double total = 0.0;
    for (int i = 1; i < prob.n - 1; i++) total += prob.scores[i];

    std::cout << "\n==================================================" << std::endl;
    std::cout << "  Best score: " << score << " / " << total
              << " (" << std::fixed << std::setprecision(1)
              << (score / total * 100.0) << "%)" << std::endl;
    if (dupes.empty()) {
        std::cout << "  ✓ No duplicates" << std::endl;
    } else {
        std::cout << "  ⚠ DUPLICATES: {";
        bool first = true;
        for (int d : dupes) {
            if (!first) std::cout << ", ";
            std::cout << d;
            first = false;
        }
        std::cout << "}" << std::endl;
    }
    std::cout << "==================================================" << std::endl;

    for (int t = 0; t < static_cast<int>(solution.size()); t++) {
        const auto& r = solution[t];
        std::cout << "  Team " << (t + 1) << " (" << (r.size() - 2) << " locs, "
                  << "score " << route_score(r, prob) << "): ";
        for (size_t i = 0; i < r.size(); i++) {
            if (i > 0) std::cout << " -> ";
            std::cout << r[i];
        }
        std::cout << std::endl;
    }
}

inline std::vector<std::vector<int>> solve(const Problem& prob,
                                            int n_restarts = 10,
                                            int max_ls_iter = 50,
                                            bool verbose = true) {
    std::vector<std::vector<int>> best_solution;
    double best_score = -1.0;

    if (verbose) {
        std::cout << "==================================================" << std::endl;
        std::cout << "  TOPTW Solver — m=" << prob.m << ", n=" << (prob.n - 2)
                  << ", Tmax=" << static_cast<int>(prob.Tmax) << std::endl;
        std::cout << "==================================================" << std::endl;
    }

    for (int restart = 0; restart < n_restarts; restart++) {
        std::mt19937_64 rng(restart * 137 + 1);
        double alpha = std::min(0.3, restart * 0.03);

        auto sol = construct_greedy(prob, rng, alpha);
        double score = solution_score(sol, prob);

        for (int iter = 0; iter < max_ls_iter; iter++) {
            double prev = score;

            for (int t = 0; t < prob.m; t++) {
                sol[t] = two_opt_route(sol[t], prob);
            }

            sol = relocate_move(sol, prob);
            sol = exchange_move(sol, prob);

            score = solution_score(sol, prob);
            if (score <= prev) break;
        }

        if (score > best_score) {
            best_score = score;
            best_solution = sol;
        }

        if (verbose) {
            std::cout << "  restart " << std::setw(2) << (restart + 1)
                      << ": score = " << score << std::endl;
        }
    }

    _report(best_solution, prob, best_score, verbose);

    return best_solution;
}

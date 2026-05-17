#pragma once

#include "problem.hpp"
#include "evaluate.hpp"
#include <random>
#include <set>
#include <algorithm>
#include <limits>

inline std::vector<std::vector<int>> construct_greedy(const Problem& prob,
                                                       std::mt19937_64& rng,
                                                       double alpha = 0.0) {
    int start = prob.depot_start;
    int end = prob.depot_end;

    std::vector<std::vector<int>> solution(prob.m);
    for (int t = 0; t < prob.m; t++) {
        solution[t] = {start, end};
    }

    std::set<int> unvisited;
    for (int i = 1; i < prob.n - 1; i++) {
        unvisited.insert(i);
    }

    while (!unvisited.empty()) {
        struct Candidate {
            double ratio;
            int team;
            int node;
            std::vector<int> new_route;
        };
        std::vector<Candidate> candidates;

        for (int node : unvisited) {
            for (int t = 0; t < prob.m; t++) {
                const auto& route = solution[t];
                for (size_t pos = 0; pos + 1 < route.size(); pos++) {
                    double cost = compute_insertion_cost(route, node,
                                                          static_cast<int>(pos), prob);
                    if (cost < std::numeric_limits<double>::infinity()) {
                        double ratio = prob.scores[node] / std::max(cost, 0.01);
                        std::vector<int> new_route = route;
                        new_route.insert(new_route.begin() + pos + 1, node);
                        candidates.push_back({ratio, t, node, new_route});
                    }
                }
            }
        }

        if (candidates.empty()) break;

        std::sort(candidates.begin(), candidates.end(),
                  [](const Candidate& a, const Candidate& b) {
                      return a.ratio > b.ratio;
                  });

        Candidate best;
        if (alpha == 0.0) {
            best = candidates[0];
        } else {
            int rcl_size = std::max(1, static_cast<int>(candidates.size() * alpha));
            std::uniform_int_distribution<int> idx_dist(0, rcl_size - 1);
            best = candidates[idx_dist(rng)];
        }

        solution[best.team] = best.new_route;
        unvisited.erase(best.node);
    }

    return solution;
}

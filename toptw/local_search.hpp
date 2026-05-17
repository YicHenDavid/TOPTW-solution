#pragma once

#include "problem.hpp"
#include "evaluate.hpp"
#include <algorithm>

inline std::vector<int> two_opt_route(const std::vector<int>& route,
                                       const Problem& prob) {
    std::vector<int> best = route;
    auto best_arr = evaluate_route(route, prob);
    if (!best_arr.has_value()) return best;
    double best_end = best_arr.value().back();

    int n = static_cast<int>(route.size());
    if (n <= 3) return best;

    for (int i = 1; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            std::vector<int> new_route = route;
            std::reverse(new_route.begin() + i, new_route.begin() + j + 1);

            auto arr = evaluate_route(new_route, prob);
            if (arr.has_value() && arr.value().back() < best_end) {
                best = new_route;
                best_end = arr.value().back();
            }
        }
    }

    return best;
}

inline std::vector<std::vector<int>> relocate_move(
    const std::vector<std::vector<int>>& solution, const Problem& prob) {
    auto best_sol = solution;
    double best_score = solution_score(solution, prob);

    for (int t1 = 0; t1 < prob.m; t1++) {
        const auto& r1 = solution[t1];
        if (r1.size() <= 3) continue;

        for (size_t pos1 = 1; pos1 + 1 < r1.size(); pos1++) {
            int node = r1[pos1];
            std::vector<int> new_r1 = r1;
            new_r1.erase(new_r1.begin() + static_cast<long>(pos1));
            if (!evaluate_route(new_r1, prob).has_value()) continue;

            for (int t2 = 0; t2 < prob.m; t2++) {
                if (t1 == t2) continue;
                const auto& r2 = solution[t2];
                for (size_t pos2 = 0; pos2 + 1 < r2.size(); pos2++) {
                    auto new_r2 = try_insert(r2, node, static_cast<int>(pos2), prob);
                    if (new_r2.has_value()) {
                        auto new_sol = solution;
                        new_sol[t1] = new_r1;
                        new_sol[t2] = new_r2.value();
                        double sc = solution_score(new_sol, prob);
                        if (sc > best_score) {
                            best_sol = new_sol;
                            best_score = sc;
                        }
                    }
                }
            }
        }
    }

    return best_sol;
}

inline std::vector<std::vector<int>> exchange_move(
    const std::vector<std::vector<int>>& solution, const Problem& prob) {
    auto best_sol = solution;
    double best_score = solution_score(solution, prob);

    for (int t1 = 0; t1 < prob.m - 1; t1++) {
        for (int t2 = t1 + 1; t2 < prob.m; t2++) {
            const auto& r1 = solution[t1];
            const auto& r2 = solution[t2];

            for (size_t pos1 = 1; pos1 + 1 < r1.size(); pos1++) {
                for (size_t pos2 = 1; pos2 + 1 < r2.size(); pos2++) {
                    int node1 = r1[pos1];
                    int node2 = r2[pos2];

                    auto new_r1 = r1;
                    auto new_r2 = r2;
                    new_r1[pos1] = node2;
                    new_r2[pos2] = node1;

                    if (evaluate_route(new_r1, prob).has_value() &&
                        evaluate_route(new_r2, prob).has_value()) {
                        auto new_sol = solution;
                        new_sol[t1] = new_r1;
                        new_sol[t2] = new_r2;
                        double sc = solution_score(new_sol, prob);
                        if (sc > best_score) {
                            best_sol = new_sol;
                            best_score = sc;
                        }
                    }
                }
            }
        }
    }

    return best_sol;
}

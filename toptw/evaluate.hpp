#pragma once

#include "problem.hpp"
#include <optional>
#include <vector>
#include <limits>

inline std::optional<std::vector<double>> evaluate_route(
    const std::vector<int>& route, const Problem& prob) {
    if (route.size() < 2) return std::nullopt;

    std::vector<double> arrive(route.size(), 0.0);

    for (size_t k = 1; k < route.size(); k++) {
        int i = route[k - 1];
        int j = route[k];
        double candidate = arrive[k - 1] + prob.service[i] + prob.dist[i][j];
        arrive[k] = std::max(candidate, prob.tw_open[j]);

        if (arrive[k] > prob.tw_close[j]) return std::nullopt;

        double slack = arrive[k] + prob.service[j] + prob.dist[j][prob.depot_end];
        if (slack > prob.Tmax) return std::nullopt;
    }

    return arrive;
}

inline double route_score(const std::vector<int>& route, const Problem& prob) {
    double total = 0.0;
    for (int node : route) total += static_cast<double>(prob.scores[node]);
    return total;
}

inline double solution_score(const std::vector<std::vector<int>>& solution,
                             const Problem& prob) {
    double total = 0.0;
    for (const auto& r : solution) total += route_score(r, prob);
    return total;
}

inline std::optional<std::vector<int>> try_insert(const std::vector<int>& route,
                                                   int node, int pos,
                                                   const Problem& prob) {
    std::vector<int> new_route;
    new_route.reserve(route.size() + 1);
    for (int i = 0; i <= pos; i++) new_route.push_back(route[i]);
    new_route.push_back(node);
    for (size_t i = pos + 1; i < route.size(); i++)
        new_route.push_back(route[i]);

    if (evaluate_route(new_route, prob).has_value()) return new_route;
    return std::nullopt;
}

inline double compute_insertion_cost(const std::vector<int>& route, int node,
                                      int pos, const Problem& prob) {
    auto old_arr = evaluate_route(route, prob);
    auto new_route = try_insert(route, node, pos, prob);
    if (!new_route.has_value())
        return std::numeric_limits<double>::infinity();
    auto new_arr = evaluate_route(new_route.value(), prob);
    return new_arr.value().back() - old_arr.value().back();
}

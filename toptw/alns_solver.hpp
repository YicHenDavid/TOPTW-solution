#pragma once

#include "problem.hpp"
#include "evaluate.hpp"
#include "local_search.hpp"
#include <random>
#include <set>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>

// ── Destroy operators ──────────────────────────────────────────

/// Random removal: pick `q` visited customers at random.
inline std::vector<int> destroy_random(const std::vector<std::vector<int>>& sol,
                                        int q, std::mt19937_64& rng,
                                        const Problem& prob) {
    std::vector<int> all;
    for (const auto& r : sol)
        for (int v : r)
            if (v != prob.depot_start && v != prob.depot_end)
                all.push_back(v);
    std::shuffle(all.begin(), all.end(), rng);
    if ((int)all.size() < q) q = (int)all.size();
    return std::vector<int>(all.begin(), all.begin() + q);
}

/// Related removal: pick a random seed, then remove its `q` nearest neighbours (by distance).
inline std::vector<int> destroy_related(const std::vector<std::vector<int>>& sol,
                                         int q, std::mt19937_64& rng,
                                         const Problem& prob) {
    std::vector<int> all;
    for (const auto& r : sol)
        for (int v : r)
            if (v != prob.depot_start && v != prob.depot_end)
                all.push_back(v);
    if (all.empty()) return {};

    std::uniform_int_distribution<int> idx(0, (int)all.size() - 1);
    int seed = all[idx(rng)];

    std::sort(all.begin(), all.end(), [&](int a, int b) {
        return prob.dist[seed][a] < prob.dist[seed][b];
    });

    if ((int)all.size() <= q) return all;
    return std::vector<int>(all.begin(), all.begin() + q);
}

/// Worst removal: remove nodes with the worst score / insertion-cost ratio.
inline std::vector<int> destroy_worst(const std::vector<std::vector<int>>& sol,
                                       int q, std::mt19937_64& rng,
                                       const Problem& prob) {
    std::vector<std::pair<double, int>> ratios;
    for (const auto& r : sol) {
        for (size_t k = 1; k + 1 < r.size(); k++) {
            int v = r[k];
            if (v == prob.depot_start || v == prob.depot_end) continue;
            // insertion cost = removing it saves dist(prev,v)+dist(v,next)-dist(prev,next)
            int prev = r[k - 1], next = r[k + 1];
            double saving = prob.dist[prev][v] + prob.dist[v][next] - prob.dist[prev][next];
            double cost = std::max(saving, 0.01);
            ratios.emplace_back(prob.scores[v] / cost, v);
        }
    }
    std::sort(ratios.begin(), ratios.end()); // ascending = worst first
    if ((int)ratios.size() < q) q = (int)ratios.size();
    std::vector<int> removed;
    for (int i = 0; i < q; i++) removed.push_back(ratios[i].second);
    return removed;
}

// ── Repair operators ───────────────────────────────────────────

/// Greedy insertion: repeatedly insert the unvisited node with the best score/cost ratio.
inline std::vector<std::vector<int>>
repair_greedy(const std::vector<std::vector<int>>& partial,
              const std::vector<int>& unvisited,
              const Problem& prob,
              std::mt19937_64& rng) {
    auto sol = partial;
    std::set<int> uv(unvisited.begin(), unvisited.end());

    while (!uv.empty()) {
        struct Cand { double ratio; int team; int node; std::vector<int> new_route; };
        std::vector<Cand> candidates;

        for (int node : uv) {
            for (int t = 0; t < prob.m; t++) {
                const auto& route = sol[t];
                for (size_t pos = 0; pos + 1 < route.size(); pos++) {
                    double cost = compute_insertion_cost(route, node, (int)pos, prob);
                    if (cost < std::numeric_limits<double>::infinity()) {
                        double r = prob.scores[node] / std::max(cost, 0.01);
                        auto nr = route;
                        nr.insert(nr.begin() + pos + 1, node);
                        candidates.push_back({r, t, node, nr});
                    }
                }
            }
        }
        if (candidates.empty()) break;

        std::sort(candidates.begin(), candidates.end(),
                  [](const Cand& a, const Cand& b) { return a.ratio > b.ratio; });
        auto& best = candidates[0];
        sol[best.team] = best.new_route;
        uv.erase(best.node);
    }
    return sol;
}

/// Regret-2 insertion: at each step, insert the node whose best and 2nd-best
/// insertion costs differ the most (largest regret).
inline std::vector<std::vector<int>>
repair_regret2(const std::vector<std::vector<int>>& partial,
               const std::vector<int>& unvisited,
               const Problem& prob,
               std::mt19937_64& rng) {
    auto sol = partial;
    std::set<int> uv(unvisited.begin(), unvisited.end());

    while (!uv.empty()) {
        struct Entry { double regret; int team; int node; int best_pos;
                       std::vector<int> best_route; };
        std::vector<Entry> entries;

        for (int node : uv) {
            double best_cost = std::numeric_limits<double>::infinity();
            double second_cost = std::numeric_limits<double>::infinity();
            int best_team = -1, best_pos = -1;
            std::vector<int> best_route;

            for (int t = 0; t < prob.m; t++) {
                const auto& route = sol[t];
                for (size_t pos = 0; pos + 1 < route.size(); pos++) {
                    double cost = compute_insertion_cost(route, node, (int)pos, prob);
                    if (cost < best_cost) {
                        second_cost = best_cost;
                        best_cost = cost;
                        best_team = t;
                        best_pos = (int)pos;
                        auto nr = route;
                        nr.insert(nr.begin() + pos + 1, node);
                        best_route = nr;
                    } else if (cost < second_cost) {
                        second_cost = cost;
                    }
                }
            }

            if (best_cost < std::numeric_limits<double>::infinity()) {
                double regret = (second_cost < std::numeric_limits<double>::infinity())
                                  ? (second_cost - best_cost)
                                  : 1e9; // only one feasible position → huge regret
                entries.push_back({regret, best_team, node, best_pos, best_route});
            }
        }
        if (entries.empty()) break;

        std::sort(entries.begin(), entries.end(),
                  [](const Entry& a, const Entry& b) { return a.regret > b.regret; });
        auto& pick = entries[0];
        sol[pick.team] = pick.best_route;
        uv.erase(pick.node);
    }
    return sol;
}

// ── Helper ──────────────────────────────────────────────────────

inline void apply_local_search(std::vector<std::vector<int>>& sol,
                                const Problem& prob,
                                int max_iter = 20) {
    for (int iter = 0; iter < max_iter; iter++) {
        double prev = solution_score(sol, prob);
        for (int t = 0; t < prob.m; t++)
            sol[t] = two_opt_route(sol[t], prob);
        sol = relocate_move(sol, prob);
        sol = exchange_move(sol, prob);
        if (solution_score(sol, prob) <= prev) break;
    }
}

// ── ALNS Main Solver ───────────────────────────────────────────

struct ALNSResult {
    std::vector<std::vector<int>> solution;
    double score;
    double elapsed_seconds;
};

inline ALNSResult solve_alns(const Problem& prob,
                              int max_iterations = 500,
                              int destroy_pct = 15,
                              double T_init = 50.0,
                              double alpha_cool = 0.995,
                              bool verbose = true) {
    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();

    // ── Initial solution via original multi-start solver ──
    std::vector<std::vector<int>> best_sol;
    double best_score = -1.0;
    {
        for (int restart = 0; restart < 10; restart++) {
            std::mt19937_64 init_rng(restart * 137 + 1);
            double alpha = std::min(0.3, restart * 0.03);
            auto sol = construct_greedy(prob, init_rng, alpha);
            apply_local_search(sol, prob, 50);
            double sc = solution_score(sol, prob);
            if (sc > best_score) { best_score = sc; best_sol = sol; }
        }
    }

    auto current_sol = best_sol;
    double current_score = best_score;

    // ── Destroy/repair operator pools ──
    enum { D_RANDOM = 0, D_RELATED = 1, D_WORST = 2, D_COUNT = 3 };
    enum { R_GREEDY = 0, R_REGRET2 = 1, R_COUNT = 2 };

    std::vector<double> w_destroy(D_COUNT, 1.0);
    std::vector<double> w_repair(R_COUNT, 1.0);
    double lambda = 0.9;
    // score rewards: global-best, better, accepted, rejected
    double psi_vals[4] = {30.0, 10.0, 3.0, 0.0};

    int total_visited = 0;
    for (const auto& r : best_sol)
        total_visited += (int)r.size() - 2;
    int destroy_q = std::max(2, total_visited * destroy_pct / 100);

    std::mt19937_64 rng(137);
    std::uniform_real_distribution<double> u01(0.0, 1.0);
    double T = T_init;

    if (verbose) {
        std::cout << "\n==================================================" << std::endl;
        std::cout << "  ALNS Solver — m=" << prob.m << ", n=" << (prob.n - 2)
                  << ", Tmax=" << (int)prob.Tmax << std::endl;
        std::cout << "  max_iter=" << max_iterations << ", destroy=" << destroy_pct
                  << "%, T0=" << T_init << ", α=" << std::setprecision(4) << alpha_cool << std::endl;
        std::cout << "==================================================" << std::endl;
    }

    int accepted = 0, improved = 0, global_improved = 0;
    int iter_since_best = 0;

    for (int iter = 0; iter < max_iterations; iter++) {
        // ── Roulette-wheel select destroy & repair ──
        auto select_op = [&](const std::vector<double>& w) -> int {
            double total = 0;
            for (double x : w) total += x;
            double r = u01(rng) * total;
            double cum = 0;
            for (int i = 0; i < (int)w.size(); i++) {
                cum += w[i];
                if (r <= cum) return i;
            }
            return (int)w.size() - 1;
        };
        int d_idx = select_op(w_destroy);
        int r_idx = select_op(w_repair);

        // ── Destroy ──
        std::vector<int> removed;
        switch (d_idx) {
            case D_RANDOM:  removed = destroy_random(current_sol, destroy_q, rng, prob); break;
            case D_RELATED: removed = destroy_related(current_sol, destroy_q, rng, prob); break;
            case D_WORST:   removed = destroy_worst(current_sol, destroy_q, rng, prob); break;
        }

        // Remove nodes from current solution
        std::set<int> removed_set(removed.begin(), removed.end());
        auto partial = current_sol;
        for (auto& route : partial) {
            route.erase(std::remove_if(route.begin(), route.end(),
                [&](int v) { return removed_set.count(v); }), route.end());
        }

        // ── Repair ──
        auto repaired = (r_idx == R_GREEDY)
            ? repair_greedy(partial, removed, prob, rng)
            : repair_regret2(partial, removed, prob, rng);

        // ── Local search ──
        apply_local_search(repaired, prob, 10);
        double new_score = solution_score(repaired, prob);

        // ── Acceptance (SA-style) ──
        int psi_idx;
        if (new_score > best_score) {
            psi_idx = 0;  // global best
            best_score = new_score;
            best_sol = repaired;
            global_improved++;
            iter_since_best = 0;
        } else if (new_score > current_score) {
            psi_idx = 1;  // better than current
            improved++;
            iter_since_best = 0;
        } else {
            double delta = new_score - current_score; // negative
            double prob_accept = std::exp(delta / T);
            if (u01(rng) < prob_accept) {
                psi_idx = 2; // accepted
                accepted++;
            } else {
                psi_idx = 3; // rejected
            }
        }

        if (new_score > current_score || psi_idx != 3) {
            current_sol = repaired;
            current_score = new_score;
        }

        iter_since_best++;

        // ── Update weights ──
        double psi = psi_vals[psi_idx];
        w_destroy[d_idx] = lambda * w_destroy[d_idx] + (1.0 - lambda) * psi;
        w_repair[r_idx]  = lambda * w_repair[r_idx]  + (1.0 - lambda) * psi;

        // ── Cooling ──
        T *= alpha_cool;

        // ── Periodic report ──
        if (verbose && (iter + 1) % 100 == 0) {
            std::cout << "  iter " << std::setw(4) << (iter + 1)
                      << ": best=" << best_score
                      << "  cur=" << current_score
                      << "  T=" << std::fixed << std::setprecision(1) << T
                      << "  w_d=(" << std::setprecision(2) << w_destroy[0]
                      << "," << w_destroy[1] << "," << w_destroy[2] << ")"
                      << "  w_r=(" << w_repair[0] << "," << w_repair[1] << ")"
                      << std::endl;
        }

        // Early stop if no global improvement for a long time
        if (iter_since_best > 200) break;
    }

    auto t1 = high_resolution_clock::now();
    double elapsed = duration<double>(t1 - t0).count();

    if (verbose) {
        double total = 0;
        for (int i = 1; i < prob.n - 1; i++) total += prob.scores[i];
        std::cout << "\n==================================================" << std::endl;
        std::cout << "  ALNS Final — best score: " << best_score
                  << " / " << total << " (" << (best_score / total * 100.0) << "%)" << std::endl;
        std::cout << "  global_improved: " << global_improved
                  << "  improved: " << improved
                  << "  accepted: " << accepted << std::endl;
        std::cout << "  Wall time: " << elapsed << "s" << std::endl;
        std::cout << "==================================================" << std::endl;
        for (int t = 0; t < (int)best_sol.size(); t++) {
            const auto& r = best_sol[t];
            std::cout << "  Team " << (t + 1) << " (" << (r.size() - 2) << " locs, score "
                      << route_score(r, prob) << "): ";
            for (size_t i = 0; i < r.size(); i++) {
                if (i > 0) std::cout << " -> ";
                std::cout << r[i];
            }
            std::cout << std::endl;
        }

        // Final operator weights
        std::cout << "\n  Final weights: destroy=("
                  << std::fixed << std::setprecision(2)
                  << w_destroy[0] << "," << w_destroy[1] << "," << w_destroy[2]
                  << ")  repair=(" << w_repair[0] << "," << w_repair[1] << ")" << std::endl;
    }

    return {best_sol, best_score, elapsed};
}

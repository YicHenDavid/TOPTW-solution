#pragma once

#include "problem.hpp"
#include "evaluate.hpp"
#include "local_search.hpp"
#include <random>
#include <set>
#include <map>
#include <deque>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>

// ═══════════════════════════════════════════════════════════════
//  DESTROY OPERATORS
// ═══════════════════════════════════════════════════════════════

/// ① Random Removal — pick q visited customers uniformly at random.
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

/// ② Shaw Removal — remove nodes that are "related" to a random seed.
/// Relatedness = weighted sum of distance similarity and time-window overlap.
inline std::vector<int> destroy_shaw(const std::vector<std::vector<int>>& sol,
                                      int q, std::mt19937_64& rng,
                                      const Problem& prob) {
    std::vector<int> all;
    for (const auto& r : sol)
        for (int v : r)
            if (v != prob.depot_start && v != prob.depot_end)
                all.push_back(v);
    if (all.empty()) return {};

    // Pick a random seed
    std::uniform_int_distribution<int> idx(0, (int)all.size() - 1);
    int seed = all[idx(rng)];

    // Rank by relatedness: distance (normalised) + time-window overlap
    double max_dist = 0;
    for (int v : all) max_dist = std::max(max_dist, prob.dist[seed][v]);

    std::vector<std::pair<double, int>> relatedness;
    for (int v : all) {
        double dist_norm = (max_dist > 0) ? prob.dist[seed][v] / max_dist : 0;
        // time-window overlap: 1 - normalised gap between windows
        double tw_overlap = 0;
        double gap = std::max(0.0, prob.tw_open[v] - prob.tw_close[seed])
                   + std::max(0.0, prob.tw_open[seed] - prob.tw_close[v]);
        double max_tw = std::max(prob.tw_close[seed] - prob.tw_open[seed],
                                 prob.tw_close[v] - prob.tw_open[v]);
        if (max_tw > 0) tw_overlap = 1.0 - std::min(1.0, gap / max_tw);

        double related = 0.6 * dist_norm + 0.4 * (1.0 - tw_overlap);
        relatedness.emplace_back(related, v);
    }

    // Sort by relatedness (smaller = more related), plus some randomness
    std::uniform_real_distribution<double> noise(0.0, 0.1);
    for (auto& p : relatedness) p.first += noise(rng);

    std::sort(relatedness.begin(), relatedness.end());

    if ((int)relatedness.size() <= q) return all;
    std::vector<int> removed;
    for (int i = 0; i < q; i++) removed.push_back(relatedness[i].second);
    return removed;
}

/// ③ Worst Removal — remove nodes with the worst score / insertion-cost ratio.
inline std::vector<int> destroy_worst(const std::vector<std::vector<int>>& sol,
                                       int q, std::mt19937_64& rng,
                                       const Problem& prob) {
    std::vector<std::pair<double, int>> ratios;
    for (const auto& r : sol) {
        for (size_t k = 1; k + 1 < r.size(); k++) {
            int v = r[k];
            if (v == prob.depot_start || v == prob.depot_end) continue;
            int prev = r[k - 1], next = r[k + 1];
            double saving = prob.dist[prev][v] + prob.dist[v][next] - prob.dist[prev][next];
            double cost = std::max(saving, 0.01);
            ratios.emplace_back(prob.scores[v] / cost, v);
        }
    }
    // Add deterministic noise for tie-breaking
    std::uniform_real_distribution<double> jitter(0.0, 0.01);
    for (auto& p : ratios) p.first += jitter(rng);
    std::sort(ratios.begin(), ratios.end()); // ascending = worst first
    if ((int)ratios.size() < q) q = (int)ratios.size();
    std::vector<int> removed;
    for (int i = 0; i < q; i++) removed.push_back(ratios[i].second);
    return removed;
}

// ═══════════════════════════════════════════════════════════════
//  REPAIR OPERATORS
// ═══════════════════════════════════════════════════════════════

/// ① Greedy Insertion — repeatedly insert the unvisited node with best score/cost ratio.
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

/// ② Regret-k Insertion — insert the node with highest regret first.
/// Regret = sum of (k-th best cost - best cost) across k best positions.
inline std::vector<std::vector<int>>
repair_regret(const std::vector<std::vector<int>>& partial,
              const std::vector<int>& unvisited,
              const Problem& prob,
              std::mt19937_64& rng,
              int k_regret = 3) {
    auto sol = partial;
    std::set<int> uv(unvisited.begin(), unvisited.end());

    while (!uv.empty()) {
        struct Entry { double regret; int team; int node;
                       std::vector<int> best_route; };
        std::vector<Entry> entries;

        for (int node : uv) {
            std::vector<double> costs;
            int best_team = -1;
            std::vector<int> best_route;
            double best_cost = std::numeric_limits<double>::infinity();

            for (int t = 0; t < prob.m; t++) {
                const auto& route = sol[t];
                for (size_t pos = 0; pos + 1 < route.size(); pos++) {
                    double cost = compute_insertion_cost(route, node, (int)pos, prob);
                    if (cost < std::numeric_limits<double>::infinity()) {
                        costs.push_back(cost);
                        if (cost < best_cost) {
                            best_cost = cost;
                            best_team = t;
                            auto nr = route;
                            nr.insert(nr.begin() + pos + 1, node);
                            best_route = nr;
                        }
                    }
                }
            }

            if (!costs.empty()) {
                std::sort(costs.begin(), costs.end());
                double regret = 0;
                for (int r = 0; r < std::min(k_regret, (int)costs.size()); r++)
                    regret += (costs[r] - best_cost);
                entries.push_back({regret, best_team, node, best_route});
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

/// ③ Noise Insertion — greedy insertion with perturbation on the cost.
/// cost_noisy = cost × (1 + noise), where noise ~ uniform(-noise_level, +noise_level)
inline std::vector<std::vector<int>>
repair_noise(const std::vector<std::vector<int>>& partial,
             const std::vector<int>& unvisited,
             const Problem& prob,
             std::mt19937_64& rng,
             double noise_level = 0.15) {
    auto sol = partial;
    std::set<int> uv(unvisited.begin(), unvisited.end());
    std::uniform_real_distribution<double> noise_dist(-noise_level, noise_level);

    while (!uv.empty()) {
        struct Cand { double ratio; int team; int node; std::vector<int> new_route; };
        std::vector<Cand> candidates;

        for (int node : uv) {
            for (int t = 0; t < prob.m; t++) {
                const auto& route = sol[t];
                for (size_t pos = 0; pos + 1 < route.size(); pos++) {
                    double cost = compute_insertion_cost(route, node, (int)pos, prob);
                    if (cost < std::numeric_limits<double>::infinity()) {
                        double noisy_cost = cost * (1.0 + noise_dist(rng));
                        double r = prob.scores[node] / std::max(noisy_cost, 0.01);
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

// ═══════════════════════════════════════════════════════════════
//  LOCAL SEARCH HELPER
// ═══════════════════════════════════════════════════════════════

inline void apply_local_search(std::vector<std::vector<int>>& sol,
                                const Problem& prob,
                                int max_iter = 20,
                                bool debug = false) {
    if (debug) {
        double init = solution_score(sol, prob);
        std::cout << "  [LS] start score=" << init << std::endl;
    }
    for (int iter = 0; iter < max_iter; iter++) {
        double prev = solution_score(sol, prob);
        for (int t = 0; t < prob.m; t++)
            sol[t] = two_opt_route(sol[t], prob);
        double after_2opt = solution_score(sol, prob);
        sol = relocate_move(sol, prob);
        double after_relocate = solution_score(sol, prob);
        sol = exchange_move(sol, prob);
        double after_exchange = solution_score(sol, prob);
        if (debug) {
            std::cout << "  [LS] iter " << iter
                      << "  2opt:" << (after_2opt - prev)
                      << "  reloc:" << (after_relocate - after_2opt)
                      << "  exch:" << (after_exchange - after_relocate)
                      << "  total:" << (after_exchange - prev)
                      << std::endl;
        }
        if (after_exchange <= prev) break;
    }
}

// ═══════════════════════════════════════════════════════════════
//  ALNS MAIN SOLVER — three metaheuristic layers
// ═══════════════════════════════════════════════════════════════
//
//  Layer 1 — Simulated Annealing (acceptance)
//     Accept better solutions always; accept worse with probability exp(Δ/T).
//     Temperature cools geometrically: T ← α × T.
//
//  Layer 2 — Tabu Search (memory)
//     Recently removed nodes are "tabu" for re-insertion for `tabu_tenure` iterations.
//     Prevents the search from immediately reinserting what was just removed.
//
//  Layer 3 — Adaptive Weights (operator selection)
//     Roulette-wheel selection of destroy/repair operators based on past performance.
//     Weights updated via: ρ ← λ·ρ + (1−λ)·ψ  (ψ depends on solution quality).

struct ALNSResult {
    std::vector<std::vector<int>> solution;
    double score;
    double elapsed_seconds;
};

struct ALNSParams {
    int max_iterations   = 500;
    int destroy_pct      = 15;    // % of visited nodes to remove
    double T_init        = 50.0;  // SA initial temperature
    double alpha_cool    = 0.995; // SA cooling rate
    int tabu_tenure      = 12;    // Tabu: iterations a removed node stays forbidden
    double lambda_weight = 0.9;   // ALNS weight decay factor
    int regret_k         = 3;     // k for regret insertion
    double noise_level   = 0.15;  // noise level for noise insertion
    int ls_max_iter      = 20;    // local search iterations per ALNS cycle
    bool verbose         = true;
    bool debug           = false; // per-iteration detail output
};

inline ALNSResult solve_alns(const Problem& prob, const ALNSParams& p = ALNSParams{}) {
    using namespace std::chrono;
    auto t0 = high_resolution_clock::now();

    // ── Initial solution via GRASP multi-start ──
    std::vector<std::vector<int>> best_sol;
    double best_score = -1.0;
    {
        for (int restart = 0; restart < 10; restart++) {
            std::mt19937_64 init_rng(restart * 137 + 1);
            double alpha = std::min(0.3, restart * 0.03);
            auto sol = construct_greedy(prob, init_rng, alpha);
            apply_local_search(sol, prob, 50, false);
            double sc = solution_score(sol, prob);
            if (sc > best_score) { best_score = sc; best_sol = sol; }
        }
    }

    auto current_sol = best_sol;
    double current_score = best_score;

    // ── Operator pools ──
    enum { D_RANDOM = 0, D_SHAW = 1, D_WORST = 2, D_COUNT = 3 };
    enum { R_GREEDY = 0, R_REGRET = 1, R_NOISE = 2, R_COUNT = 3 };

    std::vector<std::string> d_names = {"Random", "Shaw", "Worst"};
    std::vector<std::string> r_names = {"Greedy", "Regret", "Noise"};

    std::vector<double> w_destroy(D_COUNT, 1.0);
    std::vector<double> w_repair(R_COUNT, 1.0);
    double psi_vals[4] = {30.0, 10.0, 3.0, 0.0}; // global-best, better, accepted, rejected

    // ── Count visited nodes for destroy amount ──
    int total_visited = 0;
    for (const auto& r : best_sol) total_visited += (int)r.size() - 2;
    int destroy_q = std::max(2, total_visited * p.destroy_pct / 100);

    // ── Random engine ──
    std::mt19937_64 rng(137);
    std::uniform_real_distribution<double> u01(0.0, 1.0);

    // ── Tabu list: node → iteration when it can be used again ──
    std::map<int, int> tabu_until; // node → iteration index when tabu expires

    double T = p.T_init;
    int accepted = 0, improved = 0, global_improved = 0;
    int iter_since_best = 0;

    if (p.verbose) {
        std::cout << "\n==================================================" << std::endl;
        std::cout << "  ALNS Solver" << std::endl;
        std::cout << "  m=" << prob.m << "  n=" << (prob.n - 1)
                  << "  Tmax=" << (int)prob.Tmax << std::endl;
        std::cout << "  Layers: SA(α=" << p.alpha_cool << " T0=" << p.T_init
                  << ") + Tabu(tenure=" << p.tabu_tenure
                  << ") + AdaptiveWeights(λ=" << p.lambda_weight << ")" << std::endl;
        std::cout << "  max_iter=" << p.max_iterations
                  << "  destroy=" << p.destroy_pct << "%"
                  << "  regret_k=" << p.regret_k
                  << "  noise=" << p.noise_level << std::endl;
        std::cout << "==================================================" << std::endl;
    }

    for (int iter = 0; iter < p.max_iterations; iter++) {
        // ── Roulette-wheel select operators (Layer 3: Adaptive Weights) ──
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
            case D_RANDOM: removed = destroy_random(current_sol, destroy_q, rng, prob); break;
            case D_SHAW:   removed = destroy_shaw(current_sol, destroy_q, rng, prob);   break;
            case D_WORST:  removed = destroy_worst(current_sol, destroy_q, rng, prob);  break;
        }

        // ── Apply Tabu (Layer 2): filter out tabu nodes from removed set ──
        // Tabu prevents recently-removed nodes from being reinserted immediately.
        std::vector<int> tabu_removed, allowed_removed;
        for (int v : removed) {
            auto it = tabu_until.find(v);
            if (it != tabu_until.end() && it->second > iter)
                tabu_removed.push_back(v);
            else
                allowed_removed.push_back(v);
        }
        // If all removed nodes are tabu, ignore tabu this round
        if (allowed_removed.empty()) allowed_removed = removed;

        // Remove nodes from solution
        std::set<int> removed_set(allowed_removed.begin(), allowed_removed.end());
        auto partial = current_sol;
        for (auto& route : partial) {
            route.erase(std::remove_if(route.begin(), route.end(),
                [&](int v) { return removed_set.count(v); }), route.end());
        }

        // ── Repair ──
        auto repaired = [&]() {
            switch (r_idx) {
                case R_GREEDY: return repair_greedy(partial, allowed_removed, prob, rng);
                case R_REGRET: return repair_regret(partial, allowed_removed, prob, rng, p.regret_k);
                case R_NOISE:  return repair_noise(partial, allowed_removed, prob, rng, p.noise_level);
                default:       return repair_greedy(partial, allowed_removed, prob, rng);
            }
        }();

        // ── Mark removed nodes as tabu (Layer 2) ──
        for (int v : allowed_removed)
            tabu_until[v] = iter + p.tabu_tenure;

        // ── Local search ──
        double score_before_ls = solution_score(repaired, prob);
        apply_local_search(repaired, prob, p.ls_max_iter, p.debug);
        double new_score = solution_score(repaired, prob);
        double ls_delta = new_score - score_before_ls;

        // ── Debug output ──
        if (p.debug) {
            std::cout << "\n── iter " << iter
                      << " ──────────────────────────────" << std::endl;
            std::cout << "  selected: D[" << d_names[d_idx] << "] + R["
                      << r_names[r_idx] << "]" << std::endl;
            std::cout << "  removed " << allowed_removed.size() << " nodes: {";
            for (size_t i = 0; i < allowed_removed.size() && i < 10; i++)
                std::cout << (i ? "," : "") << allowed_removed[i];
            if (allowed_removed.size() > 10) std::cout << ",...";
            std::cout << "}" << std::endl;
            if (!tabu_removed.empty()) {
                std::cout << "  tabu blocked " << tabu_removed.size() << " nodes: {";
                for (size_t i = 0; i < tabu_removed.size() && i < 10; i++)
                    std::cout << (i ? "," : "") << tabu_removed[i];
                if (tabu_removed.size() > 10) std::cout << ",...";
                std::cout << "}" << std::endl;
            }
            std::cout << "  tabu list size: " << tabu_until.size() << std::endl;
            std::cout << "  repair score: " << score_before_ls
                      << "  after LS: " << new_score
                      << "  LS delta: " << (ls_delta >= 0 ? "+" : "") << ls_delta
                      << std::endl;
            std::cout << "  current: " << current_score
                      << "  best: " << best_score
                      << "  T: " << std::fixed << std::setprecision(2) << T
                      << "  accepted? ";
        }

        // ── SA Acceptance (Layer 1) ──
        int psi_idx;
        if (new_score > best_score) {
            psi_idx = 0; // global best
            best_score = new_score;
            best_sol = repaired;
            global_improved++;
            iter_since_best = 0;
        } else if (new_score > current_score) {
            psi_idx = 1; // better than current
            improved++;
            iter_since_best = 0;
        } else {
            double delta = new_score - current_score; // ≤ 0
            double prob_accept = std::exp(delta / T);
            if (u01(rng) < prob_accept) {
                psi_idx = 2; // accepted (worse)
                accepted++;
            } else {
                psi_idx = 3; // rejected
            }
        }

        if (new_score > current_score || psi_idx != 3) {
            current_sol = repaired;
            current_score = new_score;
        }

        // Debug: print acceptance result
        if (p.debug) {
            const char* labels[] = {"GLOBAL_BEST ★", "improved ↑", "accepted_worse ~", "REJECTED ✗"};
            std::cout << labels[psi_idx] << std::endl;
        }

        iter_since_best++;

        // ── Update weights (Layer 3) ──
        double psi = psi_vals[psi_idx];
        w_destroy[d_idx] = p.lambda_weight * w_destroy[d_idx] + (1.0 - p.lambda_weight) * psi;
        w_repair[r_idx]   = p.lambda_weight * w_repair[r_idx]  + (1.0 - p.lambda_weight) * psi;

        // ── Cool (Layer 1) ──
        T *= p.alpha_cool;

        // ── Periodic report ──
        if (p.verbose && (iter + 1) % 100 == 0) {
            std::cout << "  iter " << std::setw(4) << (iter + 1)
                      << "  best=" << std::fixed << std::setprecision(0) << best_score
                      << "  cur=" << current_score
                      << "  T=" << std::setprecision(1) << T
                      << "  | D[" << d_names[d_idx] << "]+R[" << r_names[r_idx] << "]"
                      << "  w_d(" << std::setprecision(2)
                      << w_destroy[0] << "," << w_destroy[1] << "," << w_destroy[2] << ")"
                      << "  w_r(" << w_repair[0] << "," << w_repair[1] << "," << w_repair[2] << ")"
                      << "  tabu:" << tabu_until.size()
                      << std::endl;
        }

        if (iter_since_best > 200) break;
    }

    auto t1 = high_resolution_clock::now();
    double elapsed = duration<double>(t1 - t0).count();

    if (p.verbose) {
        double total = 0;
        for (int i = 1; i < prob.n; i++) total += prob.scores[i];
        std::cout << "\n==================================================" << std::endl;
        std::cout << "  ALNS Final — best: " << best_score
                  << " / " << total << " (" << (best_score / total * 100.0) << "%)" << std::endl;
        std::cout << "  global_improved:" << global_improved
                  << "  improved:" << improved
                  << "  accepted_worse:" << accepted << std::endl;
        std::cout << "  time: " << elapsed << "s" << std::endl;
        std::cout << "  Final weights:" << std::endl;
        for (int i = 0; i < D_COUNT; i++)
            std::cout << "    D[" << d_names[i] << "]=" << std::fixed << std::setprecision(2) << w_destroy[i];
        std::cout << std::endl;
        for (int i = 0; i < R_COUNT; i++)
            std::cout << "    R[" << r_names[i] << "]=" << std::setprecision(2) << w_repair[i];
        std::cout << std::endl;
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
    }

    return {best_sol, best_score, elapsed};
}

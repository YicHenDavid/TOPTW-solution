#include "toptw/instance_loader.hpp"
#include "toptw/problem.hpp"
#include "toptw/solver.hpp"
#include "toptw/alns_solver.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    std::string bench_dir = "benchmarks/c_r_rc_100_100";

    std::cout << "\n====================================================" << std::endl;
    std::cout << "  TOPTW Solver — Benchmark Instances" << std::endl;
    std::cout << "  Directory: " << bench_dir << std::endl;
    std::cout << "====================================================\n" << std::endl;

    double total_original = 0.0, total_alns = 0.0;
    double total_time_original = 0.0, total_time_alns = 0.0;
    int count = 0;

    for (const auto& entry : fs::directory_iterator(bench_dir)) {
        if (!entry.is_regular_file()) continue;
        auto path = entry.path().string();
        if (path.find("_tour") != std::string::npos) continue;

        std::string name = entry.path().filename().string();
        std::cout << "\n── " << name << " ──" << std::endl;

        auto prob = load_instance(path);

        // ── Original solver ──
        auto t0 = std::chrono::high_resolution_clock::now();
        auto sol_orig = solve(prob, 5, 50, false);
        auto t1 = std::chrono::high_resolution_clock::now();
        double score_orig = solution_score(sol_orig, prob);
        double time_orig = std::chrono::duration<double>(t1 - t0).count();

        // ── ALNS solver ──
        ALNSParams alns_params;
        alns_params.max_iterations = 300;
        alns_params.verbose = false;
        auto t2 = std::chrono::high_resolution_clock::now();
        auto result = solve_alns(prob, alns_params);
        auto t3 = std::chrono::high_resolution_clock::now();
        double time_alns = std::chrono::duration<double>(t3 - t2).count();

        double total_possible = 0;
        for (int i = 1; i < prob.n; i++) total_possible += prob.scores[i];

        std::cout << "  " << std::left << std::setw(10) << name
                  << " m=" << prob.m << " n=" << (prob.n - 1)
                  << " Tmax=" << (int)prob.Tmax
                  << " | total_score=" << (int)total_possible
                  << " | original: " << std::fixed << std::setprecision(0) << score_orig
                  << " (" << time_orig << "s)"
                  << " | ALNS: " << result.score
                  << " (" << time_alns << "s)"
                  << std::endl;

        total_original += score_orig;
        total_alns += result.score;
        total_time_original += time_orig;
        total_time_alns += time_alns;
        count++;
    }

    std::cout << "\n====================================================" << std::endl;
    std::cout << "  Summary (" << count << " instances)" << std::endl;
    std::cout << "  Original total: " << std::fixed << std::setprecision(0) << total_original
              << "  (avg " << (total_original / count) << ")"
              << "  time: " << std::setprecision(1) << total_time_original << "s" << std::endl;
    std::cout << "  ALNS total:     " << std::setprecision(0) << total_alns
              << "  (avg " << (total_alns / count) << ")"
              << "  time: " << std::setprecision(1) << total_time_alns << "s" << std::endl;
    std::cout << "====================================================" << std::endl;

    return 0;
}

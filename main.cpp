#include "toptw/instance_loader.hpp"
#include "toptw/problem.hpp"
#include "toptw/solver.hpp"
#include "toptw/alns_solver.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    bool debug = false;
    std::string single_instance;
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--debug") debug = true;
        else if (arg == "--instance" && i + 1 < argc) single_instance = argv[++i];
    }

    std::string bench_dir = "benchmarks/c_r_rc_100_100";

    if (!single_instance.empty()) {
        // Single instance debug/quick mode
        auto prob = load_instance(bench_dir + "/" + single_instance);
        double tp = 0;
        for (int j = 1; j < prob.n; j++) tp += prob.scores[j];

        std::cout << "\n=== " << single_instance << " — m=" << prob.m
                  << " n=" << (prob.n-1) << " Tmax=" << (int)prob.Tmax
                  << "  total=" << (int)tp << " ===" << std::endl;

        auto sol_orig = solve(prob, 5, 50, false);
        std::cout << "Original: " << solution_score(sol_orig, prob) << std::endl;

        ALNSParams p;
        p.max_iterations = debug ? 5 : 300;
        p.verbose = true;
        p.debug = debug;
        solve_alns(prob, p);
        return 0;
    }

    // Full benchmark mode
    std::cout << "\n====================================================" << std::endl;
    std::cout << "  TOPTW Solver — Benchmark Instances (29 total)" << std::endl;
    std::cout << "====================================================\n" << std::endl;

    double total_orig = 0, total_alns = 0, time_orig = 0, time_alns = 0;
    int count = 0;

    for (const auto& entry : fs::directory_iterator(bench_dir)) {
        if (!entry.is_regular_file()) continue;
        auto path = entry.path().string();
        if (path.find("_tour") != std::string::npos) continue;

        std::string name = entry.path().filename().string();
        auto prob = load_instance(path);

        auto t0 = std::chrono::high_resolution_clock::now();
        auto sol_orig = solve(prob, 5, 50, false);
        auto t1 = std::chrono::high_resolution_clock::now();

        ALNSParams alns_p;
        alns_p.max_iterations = 300;
        alns_p.verbose = false;
        alns_p.debug = debug;
        auto t2 = std::chrono::high_resolution_clock::now();
        auto result = solve_alns(prob, alns_p);
        auto t3 = std::chrono::high_resolution_clock::now();

        double score_orig = solution_score(sol_orig, prob);
        double tp = 0;
        for (int j = 1; j < prob.n; j++) tp += prob.scores[j];

        std::cout << "  " << std::left << std::setw(10) << name
                  << " m=" << prob.m << " n=" << (prob.n-1)
                  << " Tmax=" << (int)prob.Tmax
                  << " | orig: " << std::fixed << std::setprecision(0) << score_orig
                  << " | ALNS: " << result.score
                  << " | " << std::setprecision(1)
                  << std::chrono::duration<double>(t1-t0).count() << "s / "
                  << std::chrono::duration<double>(t3-t2).count() << "s"
                  << std::endl;

        total_orig += score_orig;
        total_alns += result.score;
        time_orig += std::chrono::duration<double>(t1-t0).count();
        time_alns += std::chrono::duration<double>(t3-t2).count();
        count++;
    }

    std::cout << "\n====================================================" << std::endl;
    std::cout << "  Summary (" << count << " instances)" << std::endl;
    std::cout << "  Original: " << total_orig << " (avg " << (total_orig/count)
              << ")  " << time_orig << "s" << std::endl;
    std::cout << "  ALNS:     " << total_alns << " (avg " << (total_alns/count)
              << ")  " << time_alns << "s" << std::endl;
    std::cout << "====================================================" << std::endl;

    return 0;
}

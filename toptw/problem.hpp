#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

struct Problem {
    int n;
    int m;
    double Tmax;
    int depot_start;
    int depot_end;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<std::vector<double>> dist;
    std::vector<int> scores;
    std::vector<double> service;
    std::vector<double> tw_open;
    std::vector<double> tw_close;
};

inline Problem generate_problem(int n_customers = 30, int m = 3,
                                double Tmax = 200.0, int seed = 42) {
    std::mt19937_64 rng(seed);

    int n = n_customers + 2;

    Problem prob;
    prob.n = n;
    prob.m = m;
    prob.Tmax = Tmax;
    prob.depot_start = 0;
    prob.depot_end = n - 1;

    // Coordinates in [0, 100] x [0, 100]
    std::uniform_real_distribution<double> coord_dist(0.0, 100.0);
    prob.x.resize(n);
    prob.y.resize(n);
    for (int i = 0; i < n; i++) {
        prob.x[i] = coord_dist(rng);
        prob.y[i] = coord_dist(rng);
    }
    prob.x[0] = 0.0;
    prob.y[0] = 50.0;
    prob.x[n - 1] = 100.0;
    prob.y[n - 1] = 50.0;

    // Distance matrix (Euclidean)
    prob.dist.resize(n, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dx = prob.x[i] - prob.x[j];
            double dy = prob.y[i] - prob.y[j];
            prob.dist[i][j] = std::sqrt(dx * dx + dy * dy);
        }
    }

    // Score: higher near center
    prob.scores.resize(n);
    for (int i = 0; i < n; i++) {
        double dx = prob.x[i] - 50.0;
        double dy = prob.y[i] - 50.0;
        double dist_to_center = std::sqrt(dx * dx + dy * dy);
        prob.scores[i] = std::max(1, (int)std::round(100.0 - dist_to_center * 1.5));
    }
    prob.scores[0] = 0;
    prob.scores[n - 1] = 0;

    // Service time: random 1..10
    prob.service.resize(n, 0.0);
    std::uniform_int_distribution<int> service_dist(1, 10);
    for (int i = 1; i < n - 1; i++) {
        prob.service[i] = static_cast<double>(service_dist(rng));
    }

    // Time windows
    prob.tw_open.resize(n, 0.0);
    prob.tw_close.resize(n, 0.0);
    prob.tw_open[0] = 0.0;
    prob.tw_close[0] = Tmax;
    prob.tw_open[n - 1] = 0.0;
    prob.tw_close[n - 1] = Tmax;

    std::uniform_real_distribution<double> width_dist(20.0, 50.0);
    for (int i = 1; i < n - 1; i++) {
        double width = width_dist(rng);
        std::uniform_real_distribution<double> open_dist(0.0, Tmax - width);
        double open_t = open_dist(rng);
        prob.tw_open[i] = open_t;
        prob.tw_close[i] = open_t + width;
    }

    return prob;
}

#include <iostream>
#include <iomanip>
#include <fstream>

inline void print_problem(const Problem& prob) {
    std::cout << "\n========== Generated Problem Instance ==========" << std::endl;
    std::cout << "  members (m): " << prob.m
              << "  |  time budget (Tmax): " << prob.Tmax
              << "  |  nodes (n): " << prob.n
              << "  (depot_start=0, depot_end=" << prob.depot_end << ")"
              << std::endl;

    double total_score = 0.0;
    for (int i = 1; i < prob.n - 1; i++) total_score += prob.scores[i];
    std::cout << "  total possible score: " << total_score << std::endl;
    std::cout << "================================================" << std::endl;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n  " << std::setw(5)  << "node"
              << std::setw(10) << "x"
              << std::setw(10) << "y"
              << std::setw(10) << "score"
              << std::setw(10) << "service"
              << std::setw(12) << "tw_open"
              << std::setw(12) << "tw_close"
              << std::endl;
    std::cout << "  " << std::string(69, '-') << std::endl;

    for (int i = 0; i < prob.n; i++) {
        const char* tag = "";
        if (i == prob.depot_start) tag = "  [START]";
        if (i == prob.depot_end)   tag = "  [END]";
        std::cout << "  " << std::setw(5) << i
                  << std::setw(10) << prob.x[i]
                  << std::setw(10) << prob.y[i]
                  << std::setw(10) << prob.scores[i]
                  << std::setw(10) << prob.service[i]
                  << std::setw(12) << prob.tw_open[i]
                  << std::setw(12) << prob.tw_close[i]
                  << tag << std::endl;
    }
    std::cout << std::endl;
}

inline void save_problem_csv(const Problem& prob, const std::string& path) {
    std::ofstream f(path);
    f << "node,x,y,score,service,tw_open,tw_close\n";
    f << std::fixed << std::setprecision(4);
    for (int i = 0; i < prob.n; i++) {
        f << i << "," << prob.x[i] << "," << prob.y[i] << ","
          << prob.scores[i] << "," << prob.service[i] << ","
          << prob.tw_open[i] << "," << prob.tw_close[i] << "\n";
    }
    std::cout << "Problem saved to " << path << std::endl;
}

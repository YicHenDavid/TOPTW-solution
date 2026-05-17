#pragma once

#include "problem.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>

/// Load a TOPTW benchmark instance (Righini & Salani 2006 format).
///
/// File format:
///   Line 1:  m  ?  n_customers  flag
///   Line 2:  ?  ?
///   Lines 3..n+3: node_id  x  y  score  service_time  ?  ?  ?  tw_open  tw_close
///   (depot is always node 0, Tmax = depot's tw_close)
inline Problem load_instance(const std::string& filepath) {
    std::ifstream f(filepath);
    if (!f) {
        std::cerr << "Error: cannot open " << filepath << std::endl;
        std::exit(1);
    }

    std::string line;

    // ── Line 1: m ? n ? ──
    std::getline(f, line);
    std::istringstream hdr(line);
    int m, dummy1, n_customers, dummy2;
    hdr >> m >> dummy1 >> n_customers >> dummy2;

    // ── Line 2: skip ──
    std::getline(f, line);

    int n = n_customers + 1; // + depot

    Problem prob;
    prob.n = n;
    prob.m = m;
    prob.depot_start = 0;
    prob.depot_end = 0; // TOPTW: start == end depot

    prob.x.resize(n);
    prob.y.resize(n);
    prob.scores.resize(n);
    prob.service.resize(n);
    prob.tw_open.resize(n);
    prob.tw_close.resize(n);

    // ── Read node lines ──
    // All numeric fields in the file use ".00" format (including ints like score).
    // Must read as double to avoid stream parsing errors, then cast to int.
    for (int i = 0; i < n; i++) {
        std::getline(f, line);
        std::istringstream ss(line);
        double id, x, y, score_d, svc, a, b, c, tw_o = 0.0, tw_c = 0.0;

        if (i == 0) {
            // depot: 9 fields (no tw_open — always 0)
            ss >> id >> x >> y >> score_d >> svc >> a >> b >> c >> tw_c;
        } else {
            // customer: 10 fields
            ss >> id >> x >> y >> score_d >> svc >> a >> b >> c >> tw_o >> tw_c;
        }

        int node_id = (int)id;
        prob.x[node_id] = x;
        prob.y[node_id] = y;
        prob.scores[node_id] = (int)score_d;
        prob.service[node_id] = svc;
        prob.tw_open[node_id] = tw_o;
        prob.tw_close[node_id] = tw_c;
    }

    // Tmax = depot's tw_close
    prob.Tmax = prob.tw_close[0];

    // ── Build distance matrix ──
    prob.dist.resize(n, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double dx = prob.x[i] - prob.x[j];
            double dy = prob.y[i] - prob.y[j];
            prob.dist[i][j] = std::sqrt(dx * dx + dy * dy);
        }
    }

    return prob;
}

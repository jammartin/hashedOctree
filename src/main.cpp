#include <iostream>
#include <map>
#include <random>
#include <fstream>

#include <cxxopts.hpp>

#include "matplotlibcpp.h"
#include <cmath>

#include "../include/particle.h"

namespace plt = matplotlibcpp;

const int L{Particle::MASK_COORD2KEY};

int main(int argc, char *argv[]) {
    // initialize command line arguments parser
    cxxopts::Options options("hashedOctree", "A demonstrator for hashing an octree by z-Ordering");
    options.add_options()
        ("o,output-csv", "Write results to a csv-file")
        ("p,plot", "Plot the results and show")
        ("s,save-plot", "Plot the result and save")
        ("K,K-domains", "Number of domains", cxxopts::value<int>()->default_value("4"))
        ("N,N-particles", "Number of particles", cxxopts::value<int>()->default_value("20"))
        ("f,file-prefix", "Filename w/o extension", cxxopts::value<std::string>()->default_value("hot_result"))
        ("h,help", "Show this help");

    // read command line arguments
    auto opts = options.parse(argc, argv);

    // store options needed
    int K {opts["K-domains"].as<int>()};
    int N {opts["N-particles"].as<int>()};
    std::string filePrefix {opts["file-prefix"].as<std::string>()};

    // print help on usage and exit
    if (opts.count("help") || argc < 2)
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    // main program

    // initialize random generator
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

    // container for particles
    std::map <uint64_t, Particle*> particles;
    // generate particles uniformly distributed in a given range
    std::uniform_int_distribution <std::uint_fast32_t> dist(0, L); // define the range, cubic with side length L
    Particle *p[N];
    for (int i = 0; i < N; ++i) {
        // create a particle in a random place in 3D-space
        p[i] = new Particle(dist(gen), dist(gen), dist(gen));
        particles[p[i]->getKey()] = p[i]; // store particle
    }

    // calculate particles per domain: N-particles/K-domains if (N % K == 0)
    // note that length of last domain is less than of the others if (N % K != 0)
    int ppd = (N % K != 0) ? N/K+1 : N/K;

    // command line option -p or -s
    if (opts.count("plot") || opts.count("save-plot")) {
        // prepare data
        std::vector<int> i_x[K], i_y[K], i_z[K];
        int i = 0;
        for (auto const&[key, particle] : particles){
            i_x[i / ppd].push_back(particle->getX());
            i_y[i / ppd].push_back(particle->getY());
            i_z[i / ppd].push_back(particle->getZ());
            ++i;
        }

        // prepare plot
        auto plot3Obj = plt::build_plot3_obj();
        for (int k = 0; k < K; ++k){
            plt::add_plot3(plot3Obj, i_x[k], i_y[k], i_z[k], {{"marker", "x"}});
        }
        plt::cleanup_plot3(plot3Obj);

        // command line option -s
        if (opts.count("save-plot")){
            // save plot to file
            plt::tight_layout();
            plt::save(filePrefix + ".png");
        }
        if (opts.count("plot")){
            // show plot
            plt::show();
        }
    }

    // command line option -c
    // writing results to a csv file
    if (opts.count("output-csv")) {
        // write uniform distribution to file
        std::ofstream outf{filePrefix + ".csv"};
        if (!outf) {
            std::cerr << "An error occurred while opening a 'uniform3d.csv'. - Aborting." << std::endl;
            return 1;
        }

        outf << "key, i_x, i_y, i_z" << '\n'; // csv-header
        for (auto const&[key, particle] : particles) {
            outf << std::bitset<64>(key) << ","
                 << particle->getX() << "," << particle->getY() << "," << particle->getZ() << '\n';
        }
    }
    return 0;
}

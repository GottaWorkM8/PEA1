#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

std::vector<std::string> sources;
std::vector<int> repetitions;
std::string fileName;

void readConfig() {

    std::ifstream file("config.ini");
    std::string line;

    while (std::getline(file, line)) {

        std::istringstream ss(line);
        std::string str;
        int num;

        if (ss >> str >> num) {

            sources.push_back(str);
            repetitions.push_back(num);
        }

        else
            fileName = line;
    }
}

void writeSrc(std::string src) {

    std::ofstream file(fileName, std::ios::app);
    file << src << ", ";
}

void writeSolution(int reps, int cost, std::vector<int> path) {

    std::ofstream file(fileName, std::ios::app);
    file << std::to_string(reps) << ", " << std::to_string(cost) << ", " << "[ ";

    for (int i = 0; i < path.size(); i++)
        file << std::to_string(path.at(i)) << " ";

    file << "]\n";
}

void writeTime(int time) {

    std::ofstream file(fileName, std::ios::app);
    file << std::to_string(time) << "\n";
}

void tsp(std::vector<std::vector<int>> graph, int reps) {

    for (int k = 0; k < reps; k++) {

        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

        // store all vertices in order except for the first
        std::vector<int> vertices;
        for (int i = 1; i < graph.size(); i++)
            vertices.push_back(i);

        // store optimal path
        std::vector<int> bestPath;

        // store minimum cost of paths
        int bestPathCost = INT_MAX;

        do {

            // store current path cost
            int pathCost = 0;

            // count current path cost
            int j = 0;

            for (int i = 0; i < vertices.size(); i++) {

                pathCost += graph[j][vertices[i]];
                j = vertices[i];
            }

            pathCost += graph[j][0];

            // update the best possible path
            if (pathCost < bestPathCost) {

                bestPath = vertices;
                bestPath.insert(bestPath.begin(), 0);
                bestPathCost = pathCost;
            }

        } while (next_permutation(vertices.begin(), vertices.end()));

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        if (k == 0)
            writeSolution(reps, bestPathCost, bestPath);

        writeTime(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
    }
}

void solve(std::string src, int reps) {

    std::ifstream file(src);
    std::vector<std::vector<int>> graph;
    std::string line;

    while (std::getline(file, line)) {

        std::istringstream ss(line);
        std::vector<int> temp;
        int num;

        while (ss >> num)
            temp.push_back(num);

        graph.push_back(temp);
    }

    writeSrc(src);
    tsp(graph, reps);
}

int main() {

    readConfig();

    for (int i = 0; i < sources.size(); i++) {

        solve(sources.at(i), repetitions.at(i));
        std::cout << sources.at(i) << " completed..." << std::endl;
    }

    return 0;
}
#include <map>
#include <fstream>
#include <iostream>

#include "TMatrix.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    std::string files[] = {
        "./inputs/test.json",
        "./inputs/test2.json"
    };

    std::map<std::string, std::vector<double>> map = {};
    std::string expNames[sizeof(files) / sizeof(files[0])] = {};

    TMatrix covSyst = TMatrix(sizeof(files) / sizeof(files[0]), sizeof(files) / sizeof(files[0]));
    covSyst.Zero();
    TMatrix covStat = TMatrix(sizeof(files) / sizeof(files[0]), sizeof(files) / sizeof(files[0]));
    covStat.Zero();

    int idx = 0;
    for (auto file: files) {
        std::fstream f(file);
        json data = json::parse(f);

        expNames[idx] = data["expName"];
        covSyst(idx, idx) = data["IBDUncertainty"]["syst"];
        covStat(idx, idx) = data["IBDUncertainty"]["stat"];
        for (auto& el: data.at("fuels").items()) {
            std::cout << "Key: " << el.key() << ", item: " << el.value() << std::endl;

            if (map.find(el.key()) != map.end()) {
                map[el.key()][idx] = el.value();
            } else {
                map[el.key()] = std::vector<double>(sizeof(files) / sizeof(files[0]), 0.0);
                map[el.key()][idx] = el.value();
            }
        }
        ++idx;
    }

    for (auto item: map) {
        std::cout << item.first << " (";

        for (auto item: item.second) {
            std::cout << item << ", ";
        }

        std::cout << ")" << std::endl;
    }
}
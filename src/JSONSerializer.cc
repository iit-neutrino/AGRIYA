#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <format>

#include "TMatrix.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    std::string files[] = {
        "../inputs/test.json",
        "../inputs/test2.json",
        "../inputs/test3.json"
    };

    std::map<std::string, std::vector<double>> map = {};
    std::vector<std::string> expNames(sizeof(files) / sizeof(files[0]));

    TMatrix covSyst = TMatrix(sizeof(files) / sizeof(files[0]), sizeof(files) / sizeof(files[0]));
    TMatrix mask = TMatrix(sizeof(files) / sizeof(files[0]), sizeof(files) / sizeof(files[0])); //If the value has already been modified
    covSyst.Zero();
    mask.Zero();
    
    TMatrix covStat = TMatrix(sizeof(files) / sizeof(files[0]), sizeof(files) / sizeof(files[0]));
    covStat.Zero();

    int idx = 0;
    int otherIdx, smallerIdx, largerIdx;


    for (auto file: files) {
        std::fstream f(file);
        json data = json::parse(f);

        expNames[idx] = data["expName"];
        ++idx;
    }

    idx = 0;

    for (auto file: files) {
        std::fstream f(file);
        json data = json::parse(f);

        covSyst(idx, idx) = data["IBDUncertainty"]["syst"];
        covStat(idx, idx) = data["IBDUncertainty"]["stat"];
        mask(idx, idx) = 1;

        for (auto& dep: data.at("Dependencies").items()) {
            otherIdx = std::find(expNames.begin(), expNames.end(), dep.key()) - expNames.begin(); //Getting the index of the other experiment
            if (otherIdx != (expNames.end() - expNames.begin())) {
                smallerIdx = std::min(idx, otherIdx);
                largerIdx = std::max(idx, otherIdx);

                if (mask(smallerIdx, largerIdx) > 0) {
                    std::string errorVal = "Tried to set already set value in the covariance matricies. Please check files: ";
                    errorVal += files[smallerIdx] + ", and " + files[largerIdx];
                    errorVal += " and make the dependencies unique.";
                    throw std::runtime_error(errorVal);
                }

                mask(smallerIdx, largerIdx) = 1;
                covSyst(smallerIdx, largerIdx) = dep.value().value("syst", 0.0);
                covStat(smallerIdx, largerIdx) = dep.value().value("stat", 0.0);
            }
        }

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

    covSyst.Print();
    covStat.Print();
}
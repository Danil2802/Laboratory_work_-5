#include "plane.h"
#include "enum.h"
#include <iostream>
#include <sstream>

int main() {
    Plane planeSystem;
    std::string command;

    for (; std::getline(std::cin, command); ) {
        std::stringstream ss(command);
        std::string operation;
        ss >> operation;

        if (operation == "CREATE_PLANE") {
            std::string planeName;
            ss >> planeName;
            if (planeName.empty()) {
                std::cout << "Incorrect command format." << std::endl;
                continue;
            }
            std::vector<std::string> towns;
            std::string town;
            for (; ss >> town; ) {
                towns.push_back(town);
            }
            if (towns.empty()) {
                std::cout << "Incorrect command format." << std::endl;
                continue;
            }
            planeSystem.createPlane(planeName, towns);
        }
        else if (operation == "PLANES_FOR_TOWN") {
            std::string town;
            ss >> town;
            if (town.empty() || ss >> town) { // Проверка на наличие только одного слова
                std::cout << "Incorrect command format." << std::endl;
                continue;
            }
            planeSystem.planesForTown(town);
        }
        else if (operation == "TOWNS_FOR_PLANE") {
            std::string planeName;
            ss >> planeName;
            if (planeName.empty() || ss >> planeName) { // Проверка на наличие только одного слова
                std::cout << "Incorrect command format." << std::endl;
                continue;
            }
            planeSystem.townsForPlane(planeName);
        }
        else if (operation == "PLANES") {
            std::string extra;
            if (ss >> extra) { // Проверка на наличие дополнительных слов
                std::cout << "Incorrect command format." << std::endl;
                continue;
            }
            if (planeSystem.hasPlanes()) {
                planeSystem.displayPlanes();
            }
            else {
                std::cout << "No planes." << std::endl;
            }
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }
    }

    return 0;
}

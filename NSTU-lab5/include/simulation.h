#ifndef SIMULATION_H
#define SIMULATION_H

#include "animal.h"
#include <vector>
#include <memory>
#include <map>

class Simulation {
private:
    int width;
    int height;
    int turns;
    std::vector<std::unique_ptr<Animal>> animals;
    
    void movePhase();
    void feedingPhase();
    void agingPhase();
    void reproductionPhase();
    void extinctionPhase();
    
    std::map<std::pair<int, int>, std::vector<Animal*>> groupByPosition();
    
public:
    Simulation(int width, int height, int turns);
    
    void addAnimal(std::unique_ptr<Animal> animal);
    void run(bool visualize = false);
    void printFinalState() const;
    void visualizeStep(int step) const;
};

#endif
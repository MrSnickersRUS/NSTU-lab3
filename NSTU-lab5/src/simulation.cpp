#include "simulation.h"
#include "prey.h"
#include "predator.h"
#include <iostream>
#include <algorithm>

Simulation::Simulation(int width, int height, int turns)
    : width(width), height(height), turns(turns) {}

void Simulation::addAnimal(std::unique_ptr<Animal> animal) {
    animals.push_back(std::move(animal));
}

std::map<std::pair<int, int>, std::vector<Animal*>> Simulation::groupByPosition() {
    std::map<std::pair<int, int>, std::vector<Animal*>> groups;
    
    for (auto& animal : animals) {
        auto pos = std::make_pair(animal->getX(), animal->getY());
        groups[pos].push_back(animal.get());
    }
    
    return groups;
}

void Simulation::movePhase() {
    for (auto& animal : animals) {
        animal->move(width, height);
    }
}

void Simulation::feedingPhase() {
    auto groups = groupByPosition();
    
    for (auto& [pos, group] : groups) {
        // Найдем хищников и жертв в этой клетке
        std::vector<Predator*> predators;
        std::vector<Prey*> preys;
        
        for (auto* animal : group) {
            if (animal->getType() == AnimalType::PREDATOR) {
                predators.push_back(static_cast<Predator*>(animal));
            } else {
                preys.push_back(static_cast<Prey*>(animal));
            }
        }
        
        if (!predators.empty() && !preys.empty()) {
            // Первый хищник съедает всех жертв
            for (size_t i = 0; i < preys.size(); i++) {
                predators[0]->eatPrey();
            }
            
            // Удаляем съеденных жертв
            animals.erase(
                std::remove_if(animals.begin(), animals.end(),
                    [&preys](const std::unique_ptr<Animal>& animal) {
                        return std::find(preys.begin(), preys.end(), 
                                       dynamic_cast<Prey*>(animal.get())) != preys.end();
                    }),
                animals.end()
            );
        }
    }
}

void Simulation::agingPhase() {
    for (auto& animal : animals) {
        animal->incrementAge();
    }
}

void Simulation::reproductionPhase() {
    std::vector<std::unique_ptr<Animal>> newborns;
    
    for (auto& animal : animals) {
        if (animal->canReproduce()) {
            newborns.push_back(animal->reproduce());
            
            // Сбрасываем счетчик для хищников
            if (animal->getType() == AnimalType::PREDATOR) {
                static_cast<Predator*>(animal.get())->resetSatiety();
            }
            // Отмечаем размножение для жертв
            else {
                static_cast<Prey*>(animal.get())->markReproduced();
            }
        }
    }
    
    for (auto& newborn : newborns) {
        animals.push_back(std::move(newborn));
    }
}

void Simulation::extinctionPhase() {
    animals.erase(
        std::remove_if(animals.begin(), animals.end(),
            [](const std::unique_ptr<Animal>& animal) {
                return animal->isDead();
            }),
        animals.end()
    );
}

void Simulation::run(bool visualize) {
    if (visualize) {
        std::cout << "Начальное состояние\n";
        visualizeStep(0);
    }
    
    for (int turn = 1; turn <= turns; turn++) {
        movePhase();
        feedingPhase();
        agingPhase();
        reproductionPhase();
        extinctionPhase();
        
        if (visualize) {
            std::cout << "\nХод " << turn << "\n";
            visualizeStep(turn);
        }
    }
}

void Simulation::visualizeStep(int step) const {
    auto groups = const_cast<Simulation*>(this)->groupByPosition();
    
    std::cout << "Шаг: " << step << " | Животных: " << animals.size() << "\n";
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto it = groups.find({x, y});
            if (it == groups.end()) {
                std::cout << "· ";
            } else {
                int predators = 0, preys = 0;
                for (auto* animal : it->second) {
                    if (animal->getType() == AnimalType::PREDATOR) {
                        predators++;
                    } else {
                        preys++;
                    }
                }
                
                if (predators > 0 && preys > 0) {
                    std::cout << "X "; 
                } else if (predators > 0) {
                    std::cout << "W ";  
                } else {
                    std::cout << "R ";
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Simulation::printFinalState() const {
    auto groups = const_cast<Simulation*>(this)->groupByPosition();
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto it = groups.find({x, y});
            if (it == groups.end()) {
                std::cout << "*";
            } else {
                int predators = 0, preys = 0;
                for (auto* animal : it->second) {
                    if (animal->getType() == AnimalType::PREDATOR) {
                        predators++;
                    } else {
                        preys++;
                    }
                }
                
                if (predators > 0 && preys > 0) {
                    std::cout << "+" << preys << "-" << predators;
                } else if (predators > 0) {
                    std::cout << "-" << predators;
                } else {
                    std::cout << "+" << preys;
                }
            }
        }
        std::cout << "\n";
    }
}

#ifndef PREDATOR_H
#define PREDATOR_H

#include "animal.h"

class Predator : public Animal {
private:
    static const int MAX_AGE = 25;
    static const int PREY_TO_REPRODUCE = 2;
    int preyEaten;
    
public:
    Predator(int x, int y, Direction dir, int k);
    
    void move(int fieldWidth, int fieldHeight) override;
    bool canReproduce() const override;
    std::unique_ptr<Animal> reproduce() const override;
    AnimalType getType() const override;
    int getMaxAge() const override;
    int getSpeed() const override;
    
    void eatPrey();
    void resetSatiety();
};

#endif

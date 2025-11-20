#ifndef PREY_H
#define PREY_H

#include "animal.h"

class Prey : public Animal {
private:
    static const int MAX_AGE = 15;
    static const int FIRST_REPRODUCTION_AGE = 5;
    static const int SECOND_REPRODUCTION_AGE = 10;
    int reproductionCount;
    
public:
    Prey(int x, int y, Direction dir, int k);
    
    void move(int fieldWidth, int fieldHeight) override;
    bool canReproduce() const override;
    std::unique_ptr<Animal> reproduce() const override;
    AnimalType getType() const override;
    int getMaxAge() const override;
    int getSpeed() const override;
    
    void markReproduced();
};

#endif
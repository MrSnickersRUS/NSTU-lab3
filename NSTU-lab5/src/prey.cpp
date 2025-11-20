#include "prey.h"

Prey::Prey(int x, int y, Direction dir, int k)
    : Animal(x, y, dir, k), reproductionCount(0) {}

void Prey::move(int fieldWidth, int fieldHeight) {
    // Жертва двигается на 1 клетку
    int dx = 0, dy = 0;
    
    switch (dir) {
        case Direction::UP:    dy = -1; break;
        case Direction::RIGHT: dx = 1;  break;
        case Direction::DOWN:  dy = 1;  break;
        case Direction::LEFT:  dx = -1; break;
    }
    
    x = (x + dx + fieldWidth) % fieldWidth;
    y = (y + dy + fieldHeight) % fieldHeight;
    
    // Проверка смены направления
    stepCounter++;
    if (stepCounter >= k) {
        rotateDirection();
        stepCounter = 0;
    }
}

bool Prey::canReproduce() const {
    return (age == FIRST_REPRODUCTION_AGE && reproductionCount == 0) ||
           (age == SECOND_REPRODUCTION_AGE && reproductionCount == 1);
}

std::unique_ptr<Animal> Prey::reproduce() const {
    return std::make_unique<Prey>(x, y, dir, k);
}

AnimalType Prey::getType() const {
    return AnimalType::PREY;
}

int Prey::getMaxAge() const {
    return MAX_AGE;
}

int Prey::getSpeed() const {
    return 1;
}

void Prey::markReproduced() {
    reproductionCount++;
}

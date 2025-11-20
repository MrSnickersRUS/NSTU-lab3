#include "predator.h"

Predator::Predator(int x, int y, Direction dir, int k)
    : Animal(x, y, dir, k), preyEaten(0) {}

void Predator::move(int fieldWidth, int fieldHeight) {
    int dx = 0, dy = 0;
    
    switch (dir) {
        case Direction::UP:    dy = -2; break;
        case Direction::RIGHT: dx = 2;  break;
        case Direction::DOWN:  dy = 2;  break;
        case Direction::LEFT:  dx = -2; break;
    }
    
    x = (x + dx + fieldWidth * 2) % fieldWidth;
    y = (y + dy + fieldHeight * 2) % fieldHeight;
    
    // Проверка смены направления
    stepCounter++;
    if (stepCounter >= k) {
        rotateDirection();
        stepCounter = 0;
    }
}

bool Predator::canReproduce() const {
    return preyEaten >= PREY_TO_REPRODUCE;
}

std::unique_ptr<Animal> Predator::reproduce() const {
    return std::make_unique<Predator>(x, y, dir, k);
}

AnimalType Predator::getType() const {
    return AnimalType::PREDATOR;
}

int Predator::getMaxAge() const {
    return MAX_AGE;
}

int Predator::getSpeed() const {
    return 2;
}

void Predator::eatPrey() {
    preyEaten++;
}

void Predator::resetSatiety() {
    preyEaten = 0;
}

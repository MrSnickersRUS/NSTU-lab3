#include "animal.h"

Animal::Animal(int x, int y, Direction dir, int k)
    : x(x), y(y), dir(dir), k(k), age(0), stepCounter(0) {}

void Animal::incrementAge() {
    age++;
}

bool Animal::isDead() const {
    return age >= getMaxAge();
}

void Animal::rotateDirection() {
    int dirValue = static_cast<int>(dir);
    dir = static_cast<Direction>((dirValue + 1) % 4);
}

void Animal::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

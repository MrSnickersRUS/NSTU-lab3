#ifndef ANIMAL_H
#define ANIMAL_H

#include <memory>
#include <vector>

enum class Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

enum class AnimalType {
    PREY,
    PREDATOR
};

// Абстрактный базовый класс для всех зверей
class Animal {
protected:
    int x;
    int y;
    Direction dir;
    int k;
    int age;
    int stepCounter;
    
public:
    Animal(int x, int y, Direction dir, int k);
    virtual ~Animal() = default;
    
    virtual void move(int fieldWidth, int fieldHeight) = 0;
    virtual bool canReproduce() const = 0;
    virtual std::unique_ptr<Animal> reproduce() const = 0;
    virtual AnimalType getType() const = 0;
    virtual int getMaxAge() const = 0;
    virtual int getSpeed() const = 0;
    
    void incrementAge();
    bool isDead() const;
    void rotateDirection();

    int getX() const { return x; }
    int getY() const { return y; }
    int getAge() const { return age; }
    Direction getDirection() const { return dir; }
    int getK() const { return k; }
    
    void setPosition(int newX, int newY);
};

#endif
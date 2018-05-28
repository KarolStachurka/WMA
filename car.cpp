#include "car.h"

Car::Car()
{

}
Car::Car(int centerX, int centerY, int radius)
{
    this->centerX = centerX;
    this->centerY = centerY;
    this->radius = radius;
}
void Car::update(int centerX, int centerY, int radius)
{
    this->centerX = centerX;
    this->centerY = centerY;
    this->radius = radius;
    updated = true;
    history++;
}

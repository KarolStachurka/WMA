#ifndef CAR_H
#define CAR_H


class Car
{
public:
    int centerX;
    int centerY;
    int radius;
    int history = 0;
    bool updated = false;
    bool counted = false;
    bool spotted= false;
    Car();
    Car(int centerX, int centerY, int radius);
    void update(int centerX, int centerY, int radius);
};

#endif // CAR_H

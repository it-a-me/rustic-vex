#ifndef MOTOR_INCLUDED
#define MOTOR_INCLUDED
#include "main.h"

class Motor : public pros::Motor
{
  public:
    std::int32_t voltage = 0;
    int rpm = 0;

    explicit Motor(std::int8_t portnumber)
      : pros::Motor(portnumber)
    {
    }
    // move measures in i8 rather than %;
    void spin_voltage()
    {
        this->move(voltage * 127 / 100);
        pros::lcd::print(4, "%d", voltage);
    }
    void spin_voltage(double ratio) { this->move(voltage * 127 / 100 * ratio); }
    void spin_rpm(double ratio) { this->move_velocity(rpm * ratio); }
};

#endif

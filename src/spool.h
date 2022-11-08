#ifndef SPOOL_INCLUDED
#define SPOOL_INCLUDED

#include "drivetrain.h"

class Spool
{
  public:
    enum SpoolState
    {
        Stopped,
        Unwinding,
        Winding,
    };

  private:
    Drivetrain* drive;
    SpoolState state = Stopped;
    Motor spool;

  public:
    Spool(Motor spool_, int velocity_, Drivetrain* drive_)
      : drive(drive_)
      , spool(std::move(spool_))
    {
        spool.rpm = velocity_*600 / 100;
        spool.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        spool.set_gearing(pros::E_MOTOR_GEAR_600);
    }

    void update(bool unwind, bool wind)
    {
        if (unwind) {
            state = Unwinding;
            spool.spin_rpm(1);
        } else if (wind) {
            state = Winding;
            spool.spin_rpm(-0.5);
        } else if (state == Unwinding) {
            spool.spin_rpm(1);
        } else {
            state = Stopped;
            spool.brake();
        }
        drive->unspooling = state == Unwinding;
    }
};
#endif

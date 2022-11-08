#ifndef ROLLERSPINNER_INCLUDED
#define ROLLERSPINNER_INCLUDED
#include "drivetrain.h"

class RollerSpinner
{
    // pointers to drivetrain and roller spinning motor
  private:
    Drivetrain* drive;
    Motor spinner;

    // constructor of roller spinner class
  public:
    RollerSpinner(Motor spinner_, int velocity_, Drivetrain* drive_)
      : spinner(std::move(spinner_))
      , drive(drive_)
    {
        spinner.rpm = velocity_;
        spinner.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    }

    // spins the wheel either forward or reverse.  If the wheel is spining also
    // slightly drive backwards to apply pressure on the wheel.
    void update(bool fwd, bool rev)
    {
        if (fwd || rev) {
            drive->setMotorVelocity(Drivetrain::MotorPosition::front_left, -8);
            drive->setMotorVelocity(Drivetrain::MotorPosition::front_right, -2);
            drive->setMotorVelocity(Drivetrain::MotorPosition::back_right, -2);
            drive->setMotorVelocity(Drivetrain::MotorPosition::back_left, -8);
            // dr->fr = -2;
            // dr->br = -2;
            // dr->fl = -8;
            // dr->bl = -8;
            if (fwd) {
                spinner.spin_rpm(1);
            } else if (rev) {
                spinner.spin_rpm(-1);
            }
        } else {
            spinner.brake();
        }
    }
};
#endif

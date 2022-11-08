#ifndef DRIVETRAIN_INCLUDED
#define DRIVETRAIN_INCLUDED
#include <array>
using std::array;

#include "main.h"

#include "motor.h"
// manual drivetrain implementation
class Drivetrain
{
  public:
    // abstraction towards direction, used as a paramenter when moving
    // horizontally
    enum HorizontalDir
    {
        Left,
        Right,
    };

  protected:
    // array of the 4 motors in the drivetrain
    array<Motor, 4> motors;
    // speed of horizontal movement
    int horizontalVelocity = 70;
    // if we are in precision slower speed or max speed
    bool slowed = false;

    // adds sideways motion to current motor speeds
    void horizontal(HorizontalDir dir)
    {
        if (dir == HorizontalDir::Left) {
            motors[front_left].voltage += -horizontalVelocity;
            motors[front_right].voltage += horizontalVelocity;
            motors[back_left].voltage += horizontalVelocity;
            motors[back_right].voltage += -horizontalVelocity;

        } else if (dir == HorizontalDir::Right) {
            motors[front_left].voltage += horizontalVelocity;
            motors[front_right].voltage += -horizontalVelocity;
            motors[back_left].voltage += -horizontalVelocity;
            motors[back_right].voltage += horizontalVelocity;
        }
    }

    // resets motor velocities to 0 to ensure that they get reset between frames
    void reset_velocities()
    {
        for (Motor& motor : motors) {
            motor.voltage = 0;
        }
    }

  public:
    bool unspooling = false;
    // allows you to reference motors by position on robot rather than index in
    // int array
    enum MotorPosition
    {
        front_left,
        front_right,
        back_left,
        back_right,
    };
    // drivetrain constructor to ensure no null references
    Drivetrain(Motor front_left_port,
               Motor front_right_port,
               Motor back_left_port,
               Motor back_right_port,
               pros::motor_brake_mode_e stoppingMode)
      : motors({ std::move(front_left_port),
                 std::move(front_right_port),
                 std::move(back_left_port),
                 std::move(back_right_port) })
    {
        reset_velocities();
        setStoppingMode(stoppingMode);
    };

    // tell motors to act on current velocities
    void drive()
    {
        for (Motor motor : motors) {
            // reduce speed in slow mode
            if (unspooling) {
                motor.spin_voltage(0.4);
            } else if (slowed) {
                motor.spin_voltage(0.5);
            } else {
                motor.spin_voltage();
            }
        }

        // reset velocities to prevent carry over between frames
        reset_velocities();
    }
    // limits access to private fields
    int getMotorVelocity(MotorPosition pos) { return motors.at(pos).voltage; }

    void setMotorVelocity(MotorPosition pos, int val)
    {
        motors.at(pos).voltage = val;
    }

    // sets stoping mode for all motors at once
    void setStoppingMode(pros::motor_brake_mode_e mode)
    {
        for (const Motor& motor : motors) {
            motor.set_brake_mode(mode);
        }
    }

    // stops all drivetrain motors
    void stop()
    {
        for (const Motor& motor : motors) {
            motor.brake();
        }
    }

    // toggle speed
    void ToggleSlow()
    {
        slowed = !slowed;
        // return slowed;
    }

    void userControl(int LeftStick, int RightStick, bool left, bool right)
    {
        LeftStick = LeftStick * 100 / 127;
        RightStick = RightStick * 100 / 127;
        // mix horizontal and stick movement
        if ((LeftStick != 0 || RightStick != 0) && (left || right)) {
            motors[back_left].voltage = LeftStick;
            motors[front_left].voltage = LeftStick;
            motors[front_right].voltage = RightStick;
            motors[back_right].voltage = RightStick;
            if (left) {
                horizontal(HorizontalDir::Left);
            } else if (right) {
                horizontal(HorizontalDir::Right);
            }
            for (Motor& motor : motors) {
                motor.voltage /= 2;
            }
            // stick movement
        } else if (LeftStick != 0 || RightStick != 0) {
            motors[back_left].voltage = LeftStick;
            motors[front_left].voltage = LeftStick;
            motors[front_right].voltage = RightStick;
            motors[back_right].voltage = RightStick;
            // horiziontal movement
        } else if (left) {
            horizontal(HorizontalDir::Left);
        } else if (right) {
            horizontal(HorizontalDir::Right);
        }
    }
    // drive for a set time
    void AutonomousDrive(int left, int right, double seconds)
    {
        motors[back_left].voltage = left;
        motors[front_left].voltage = left;
        motors[front_right].voltage = right;
        motors[back_right].voltage = right;

        this->drive();
        pros::delay(seconds * float(1000));
        this->stop();
    }
    // drive horizontally for a set time
    void AutonomousHorizontal(HorizontalDir dir, double seconds)
    {
        this->horizontal(dir);
        this->drive();
        pros::delay(seconds * 1000);
        this->stop();
    }
};

#endif

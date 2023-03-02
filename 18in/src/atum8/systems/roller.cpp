#include "roller.hpp"

namespace atum8
{
    Roller::Roller(UPMotor iMotor,
                   UPOptical iOptical,
                   const Color &iColor) : motor{std::move(iMotor)},
                                          optical{std::move(iOptical)},
                                          color{iColor}
    {
        optical->set_led_pwm(100);
        motor->set_brake_mode(pros::motor_brake_mode_e::E_MOTOR_BRAKE_BRAKE);
    }

    void Roller::taskFn()
    {
        while (true)
        {
            if (turningToColor)
            {
                runRoller(80);
                waitFor([this]()
                        {
                    auto rgb = optical->get_rgb();
                    if(color == Color::Red)
                        return rgb.red < rgb.blue;
                    return rgb.red > rgb.blue; },
                        0.75_s);
                runRoller(-40);
                waitFor([this]()
                        {
                    auto rgb = optical->get_rgb();
                    if(color == Color::Red)
                        return rgb.red > rgb.blue;
                    return rgb.red < rgb.blue; },
                        0.75_s);
                turningToColor = false;
                stopRoller();
            }
            pros::delay(stdDelay);
        }
    }

    void Roller::runRoller(int speed)
    {
        motor->move(speed);
    }

    void Roller::stopRoller()
    {
        motor->move(0);
    }

    void Roller::runForAt(double position, int velocity)
    {
        motor->move_relative(position, velocity);
    }

    void Roller::turnToColor()
    {
        turningToColor = true;
    }

    void Roller::setColor(const Color &iColor)
    {
        color = iColor;
    }

    Color Roller::getColor() const
    {
        return color;
    }

    SPRoller SPRollerBuilder::build() const
    {
        return std::make_shared<Roller>(std::make_unique<pros::Motor>(motorPort, gearset),
                                        std::make_unique<pros::Optical>(opticalPort),
                                        color);
    }

    SPRollerBuilder SPRollerBuilder::withMotor(int iPort,
                                               const pros::motor_gearset_e_t &iGearset)
    {
        motorPort = iPort;
        gearset = iGearset;
        return *this;
    }

    SPRollerBuilder SPRollerBuilder::withOptical(int iPort)
    {
        opticalPort = iPort;
        return *this;
    }

    SPRollerBuilder SPRollerBuilder::withColor(const Color &iColor)
    {
        color = iColor;
        return *this;
    }
}
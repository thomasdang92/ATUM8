#include "atum8/systems/catapult.hpp"
#include "main.h"
#include "pros/misc.h"

namespace atum8 {
    void Catapult::taskFn() {
        catapultMotors.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
        while(true) {
            controller();
            std::cout << "rotation: " << rotationSensor.get_position() << std::endl;
            //std::cout << "angle: " << rotationSensor.get_angle() << std::endl;
            std::cout << "very cool: " << 9.5 * rotationSensor.get_position() - 100 << std::endl;

            pros::delay(10);
        }
    }

    void Catapult::controller(){
        if(Chris.get_digital_new_press(DIGITAL_X))
            isManualMode = !isManualMode;

        if(isManualMode == false)
            automaticContolls();
        else
            manualControlls();

    }

    // void Catapult::automaticContolls(){
    //     if(rotationSensor.get_position() > 300) {//420
    //         if(rotationSensor.get_position() > 600)
    //             catapultMotors.move_voltage(12000);
    //         else
    //           catapultMotors.move_voltage(9.5 * rotationSensor.get_position() - 100);

    //     }
    //     else {
    //         if(Chris.get_digital(DIGITAL_R1))
    //             catapultMotors.move_voltage(12000);
    //         else
    //             catapultMotors.move_voltage(0);
    //     }   
    // }

    void Catapult::automaticContolls() {
        if(!catapultStop.get_value()) {
            catapultMotors.move_voltage(12000);   
        } else {
            if(Chris.get_digital(DIGITAL_R1)) {
                catapultMotors.move_voltage(12000);
            } else {
                catapultMotors.move_voltage(0);
            }
        }
    }

    void Catapult::manualControlls(){
        if(Chris.get_digital(DIGITAL_R1))
            catapultMotors.move_voltage(12000);
        else if(Chris.get_digital(DIGITAL_R2))
            catapultMotors.move_voltage(-12000);
        else
            catapultMotors.move_voltage(0);
    }
}
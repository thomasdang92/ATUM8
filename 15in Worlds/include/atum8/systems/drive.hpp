/**
 * @file drive.hpp
 * @author Thomas Tran Dang (thomasdang92@gmail.com)
 * @brief This file provides a class for all of the drive train methods for the 15in. 
          As of the moment the 15in is a tank drive. 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include "atum8/controllers/pid.hpp"
#include "atum8/imus.hpp"
#include "atum8/slewRate.hpp"

namespace atum8 {
    class Drive : Pid, Imus, SlewRate{
        public:
          void controller();
          void move(double inches, double rpm, double acceleration, bool dift, double secThreshold);
          void turn(double angle, double rpm, double acceleration, double secThreshold);


        private:
        void setRightPower(double power);
        void setLeftPower(double power);
        double getRightPower();
        double getLeftPower();
        void setDriveBrakeMode(const std::string brakeMode);
        double getRightEncoderValues();
        double getLeftEncoderValues();
        double getEncoderAverages();
        void resetEncoders();
        void reset();
        double msCounter;
        double power;
    };
}
/**
 * @file globals.hpp
 * @author Thomas Tran Dang (thomasdang92@gmail.com)
 * @brief This file provides the setup for the motors, controller, and sensors. 
 *        It also provides globals variables used throughout the program. 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "main.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"

namespace atum8 {
// Robot Setup
extern pros::Controller Chris;
extern pros::Motor rightFrontDrive;
extern pros::Motor rightMiddleDrive;
extern pros::Motor rightBackDrive;
extern pros::Motor leftFrontDrive;
extern pros::Motor leftMiddleDrive;
extern pros::Motor leftBackDrive;
extern pros::Motor rightCatapultMotor;
extern pros::Motor leftCatapultMotor;
extern pros::Motor leftIntakeMotor;
extern pros::Motor rightIntakeMotor;
extern pros::ADIDigitalIn catapultStop;
extern pros::Imu imuSensorAlpha;
extern pros::Imu imuSensorBeta;
extern pros::Imu imuSensorCharlie;
extern pros::Optical opticalSensor;
extern pros::Vision visionSensor;
extern pros::ADIDigitalOut endGame;
extern pros::Rotation rightEncoder;
extern pros::Rotation leftEncoder;
extern pros::Rotation backEncoder;

//
const float encoderWheelRadius{1.625}; // inches
const double encoderWheelCircumference{200/25.4};

const double driveGearRatio {60.0 / 36.0};
extern int program;
} // namespace atum8
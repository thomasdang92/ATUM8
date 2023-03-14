/**
 * @file imus.hpp
 * @author Thomas Tran Dang (thomasdang92@gmail.com)
 * @brief This file provides a class for the IMU Sensors. The methods simpley combines the IMU Sensors together.
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
 
#pragma once
#include "globals.hpp"
#include "main.h"

namespace atum8{
    class Imus {
        public:
        void resetImuSensors();
        double getImuSensorAverages();
    };
}
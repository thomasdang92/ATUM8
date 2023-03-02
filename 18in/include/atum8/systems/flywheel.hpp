#pragma once

#include "atum8/constants.hpp"
#include "atum8/controllers/controller.hpp"
#include "atum8/settledChecker.hpp"
#include "atum8/task.hpp"
#include "atum8/rollingAverage.hpp"
#include "atum8/slewRate.hpp"
#include "okapi/api/units/QAngularSpeed.hpp"
#include "okapi/api/units/QAngularAcceleration.hpp"
namespace atum8
{
    class Flywheel : public Task
    {
    public:
        Flywheel(UPMotorGroup iMotorGroup,
                 SPController iVelocityController,
                 SPSettledChecker<okapi::QAngularSpeed, okapi::QAngularAcceleration> iVelocitySettledChecker,
                 SPRollingAverage iRollingAverage = nullptr,
                 SPSlewRate iSlewRate = nullptr,
                 double iSpeedMultiplier = 7.0);

        void taskFn();

        void setReferenceSpeed(const okapi::QAngularSpeed speed);

        okapi::QAngularSpeed getReferenceSpeed() const;

        okapi::QAngularSpeed getSpeed() const;

        bool readyToFire(okapi::QAngularSpeed speed);

        bool readyToFire() const;

        SPController getVelocityController() const;

        SPSettledChecker<okapi::QAngularSpeed, okapi::QAngularAcceleration> getVelocitySettledChecker() const;

    private:
        UPMotorGroup motorGroup;
        double speedMultiplier{7.0};
        SPController velocityController;
        SPSlewRate slewRate;
        SPSettledChecker<okapi::QAngularSpeed, okapi::QAngularAcceleration> velocitySettledChecker;
        SPRollingAverage rollingAverage;
        okapi::QAngularSpeed referenceSpeed{0_degps};
    };

    using UPFlywheel = std::unique_ptr<Flywheel>;
    using SPFlywheel = std::shared_ptr<Flywheel>;

    class SPFlywheelBuilder
    {
    public:
        SPFlywheel build() const;

        SPFlywheelBuilder withMotors(const std::vector<std::int8_t> &iPorts);

        SPFlywheelBuilder withSpeedMultiplier(double iSpeedMultiplier);

        SPFlywheelBuilder withController(SPController iVelocityController);

        SPFlywheelBuilder withSettledChecker(const okapi::QAngularSpeed &maxSpeedError,
                                             const okapi::QAngularAcceleration &maxAccelError = 0_rpmps,
                                             const okapi::QTime &minTime = 0_s);

        SPFlywheelBuilder withRollingAverage(int size);

        SPFlywheelBuilder withSlew(double maxNegChange, double maxPosChange);

    private:
        std::vector<std::int8_t> ports;
        double speedMultiplier{15.0};
        SPController velocityController;
        SPSettledChecker<okapi::QAngularSpeed, okapi::QAngularAcceleration> velocitySettledChecker;
        SPRollingAverage rollingAverage;
        SPSlewRate slewRate;
    };
}
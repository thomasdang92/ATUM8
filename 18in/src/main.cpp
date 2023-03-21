#include "main.h"
#include "okapi/api/units/QLength.hpp"

constexpr okapi::QAngularSpeed lowSpeed{2475_rpm};
constexpr okapi::QAngularSpeed highSpeed{2800_rpm};
constexpr okapi::QTime flywheelTimeout{1_s};

atum8::SPOdometry odometry;

void initialize()
{
	initializeLCD();
	autonSelector = std::make_shared<atum8::AutonSelector>();
	debugger = std::make_shared<atum8::Debugger>(atum8::Debugger::LineFns{
		[](int control)
		{ return "HELLO WORLD!"; },
		[](int control)
		{ return "IT'S YA BOY:"; },
		[](int control)
		{ return "ANUBIS"; },
		[](int control)
		{
			return "X: " + std::to_string(odometry->getPosition().x.convert(okapi::inch)) + "in";
		},
		[](int control)
		{
			return "Y: " + std::to_string(odometry->getPosition().y.convert(okapi::inch)) + "in";
		},
		[](int control)
		{
			return "X: " + std::to_string(odometry->getPosition().h.convert(okapi::degree)) + "deg";
		}});
	gui = autonSelector;

	auto forwardPidFF = std::make_shared<atum8::Slider>(std::make_unique<atum8::PidFF>(21, 0.01),
														std::make_unique<atum8::PidFF>(6.5, 0.001),
														4,
														0.9);
	auto turnPidFF = std::make_shared<atum8::Slider>(std::make_unique<atum8::PidFF>(10, 0.01),
													 std::make_unique<atum8::PidFF>(2.75, 0.015),
													 5,
													 0.9);
	drive = atum8::SPDriveBuilder()
				.withLeftPorts({1, 11})
				.withRightPorts({-10, -20})
				.withGearing(1)
				.withImus({14, 15, 16}, 1)
				.withBaseWidth(14.625_in)
				.withWheelCircum(5.4_in)
				.withForwardController(forwardPidFF)
				.withForwardSlew(6)
				.withTurnController(turnPidFF)
				.withTurnSlew(12)
				.withForwardSettledChecker(1_in, 1_inps, 0.375_s)
				.withTurnSettledChecker(1_deg, 500_degps, 0.375_s)
				.withStickFunction([](int input)
								   { return pow(input, 3) / 16129; })
				.build();
	drive->reset();
	gui->view();
	
	odometry = atum8::SPOdometryBuilder()
				   .withLeft('C', 'D')
				   .withRight('E', 'F')
				   .withSide('G', 'H', 0_in)
				   .withEncoderMultiplier(1.0 / 2048 / 4)
				   .withWidth(14.53_in)
				   .withWheelCircum(7.6802125_in)
				   .build();
	odometry->start();

	// auto flywheelVelController = std::make_shared<atum8::PidFF>(30, 0, 0, 4.05);
	// flywheel = atum8::SPFlywheelBuilder()
	// 			   .withMotors({16, -17})
	// 			   .withController(flywheelVelController)
	// 			   .withSettledChecker(20_rpm, 20_rpmps)
	// 			   .withSpeedMultiplier(15.0)
	// 			   .withRollingAverage(15)
	// 			   .withSlew(-100, 100)
	// 			   .build();
	// flywheel->start();
	//
	// intake = atum8::SPIntakeBuilder()
	// 			 .withMotor(13)
	// 			 .withPiston('H')
	// 			 .withFlywheel(flywheel)
	// 			 .withShotDelay(450)
	// 			 .build();
	// intake->start();
	//
	// roller = atum8::SPRollerBuilder()
	// 			 .withMotor(5)
	// 			 .withColor(atum8::Color::Red)
	// 			 .build();
	// roller->start();
	//
	// endGame = std::make_unique<pros::ADIDigitalOut>('G');
}

void disabled()
{
	gui = autonSelector;
	while (true)
	{
		gui->view();
		pros::delay(10);
	}
}

void autonomous()
{
	// flywheel->reset();
	// intake->shoot(0);
	switch (autonSelector->getMatchInfo().routine)
	{
	case atum8::Routine::Skills:
		skills();
		break;
	case atum8::Routine::Match:
		match();
		break;
	case atum8::Routine::Special:
		special();
		break;
	}
}

void opcontrol()
{
	gui = debugger;
	gui->view();
	pros::Controller master{pros::controller_id_e_t::E_CONTROLLER_MASTER};
	// flywheel->reset();
	// flywheel->setReferenceSpeed(lowSpeed + adjustment);
	// master.print(2, 0, "LOW SPEED\n");
	// intake->shoot(0);
	// bool manualRoller{true};
	// bool readyNotified{false};

	while (true)
	{
		gui->view();

		// Drive Controls
		const int forward{master.get_analog(ANALOG_LEFT_Y)};
		const int turn{master.get_analog(ANALOG_RIGHT_X)};
		drive->driver(forward, turn);

		// if (master.get_digital_new_press(DIGITAL_B))
		// {
		// 	if (drive->getDriverSettings()->brakeMode == pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST)
		// 		drive->getDriverSettings()->brakeMode = pros::motor_brake_mode_e::E_MOTOR_BRAKE_HOLD;
		// 	else
		// 		drive->getDriverSettings()->brakeMode = pros::motor_brake_mode_e::E_MOTOR_BRAKE_COAST;
		// }
		//
		// if (master.get_digital_new_press(DIGITAL_A))
		// 	drive->getDriverSettings()->maxPower = drive->getDriverSettings()->maxPower == 1.0 ? 0.5 : 1.0;
		//
		// // Intake Controls
		// if (master.get_digital(DIGITAL_L1))
		// 	intake->runIntake(127);
		// else if (master.get_digital(DIGITAL_L2))
		// 	intake->runIntake(-127);
		// else
		// 	intake->runIntake(0);
		//
		// // Roller Controls
		// if (manualRoller)
		// {
		// 	if (master.get_digital(DIGITAL_R1))
		// 		roller->runRoller();
		// 	else
		// 		roller->stopRoller();
		// }
		// else if (master.get_digital_new_press(DIGITAL_R1))
		// 	roller->turnToColor();
		//
		// if (master.get_digital_new_press(DIGITAL_X))
		// 	manualRoller = !manualRoller;
		//
		// // Shooter Controls
		// if (master.get_digital_new_press(DIGITAL_R2))
		// 	intake->shoot(1);
		//
		// if (intake->isShooting())
		// 	readyNotified = false;
		// else if (flywheel->readyToFire() && !readyNotified)
		// {
		// 	master.rumble("..");
		// 	readyNotified = true;
		// }
		//
		// if (master.get_digital_new_press(DIGITAL_Y))
		// {
		// 	switch ((int)flywheel->getReferenceSpeed().convert(okapi::rpm))
		// 	{
		// 	case 0:
		// 		flywheel->setReferenceSpeed(lowSpeed + adjustment);
		// 		master.print(2, 0, "LOW SPEED                ");
		// 		break;
		// 	case (int)(lowSpeed + adjustment).convert(okapi::rpm):
		// 		flywheel->setReferenceSpeed(highSpeed + adjustment);
		// 		master.print(2, 0, "HIGH SPEED                ");
		// 		break;
		// 	default:
		// 		flywheel->setReferenceSpeed(0_rpm);
		// 		master.print(2, 0, "OFF                ");
		// 		break;
		// 	}
		// 	readyNotified = false;
		// }
		//
		// // End Game Controls
		// if (master.get_digital(DIGITAL_UP) &&
		// 	master.get_digital(DIGITAL_DOWN) &&
		// 	master.get_digital(DIGITAL_LEFT) &&
		// 	master.get_digital(DIGITAL_RIGHT))
		// 	endGame->set_value(1);

		pros::delay(atum8::stdDelay);
	}
}

/* -------------------------------------------------------------------------- */
/*                                   Helpers                                  */
/* -------------------------------------------------------------------------- */

void initializeLCD()
{
	pros::lcd::initialize();
	pros::lcd::set_background_color(lv_color_t{0x000000});
	pros::lcd::set_text_color(255, 255, 255);
	pros::lcd::register_btn0_cb([]()
								{ gui->control(-1); });
	pros::lcd::register_btn1_cb([]()
								{ gui->control(0); });
	pros::lcd::register_btn2_cb([]()
								{ gui->control(1); });
	pros::lcd::print(0, "Don't touch!");
	pros::lcd::print(1, "IMUs are calibrating...");
}

void skills()
{
}

void match()
{
}

void special()
{
}

void turnRoller(int rotation)
{
	drive->forward(-20_ft, 0.5_s, 25);
	roller->runForAt(rotation, 200);
	drive->forward(-20_ft, 1.5_s, 25);
}

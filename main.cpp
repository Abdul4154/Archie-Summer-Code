#include "main.h"

//Motors

pros::Motor leftFront(9, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor leftBack(8, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rightFront(2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor rightBack(1, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lift1(4, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor lift2(5, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intakeLeft(3, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor intakeRight(10, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);


//CONTROLLER
	pros::Controller master(pros::E_CONTROLLER_MASTER);
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

 	//Lift
	//Adjust target values in opcontrol for your target

 	int liftPower[] = {0, -80, 80, 127};
	bool liftToggle = false;
	int liftAvg;
	int p;
	float kP = .6;
	int power = 0;

	void liftUp(int target){
		p = target - liftAvg;
		if(liftAvg >= target){
			power = liftPower[0];
			lift1.tare_position();
			lift2.tare_position();
		}
		else if(liftAvg < (target * .7)){
			power = liftPower[3];
		}
		else if(liftAvg < target){
			power = liftPower[3] * (p * kP);
		}
	}
	void liftDown(int target){
		p = target - liftAvg;
		if(liftAvg >= target){
			power = liftPower[0];
			lift1.tare_position();
			lift2.tare_position();
		}
		else if(liftAvg < (target * .7)){
			power = liftPower[1];
		}
		else if(liftAvg < target){
			power = liftPower[1] * (p * kP);
		}
	}

	void moveLiftUp(int target){
		if(liftAvg < target){
			lift1 = 127;
			lift2 = 127;
		}
		else{
			lift1 = 0;
			lift2 = 0;
		}
	}

	void moveLiftDown(int target){
		if(liftAvg > target){
			lift1 = -127;
			lift2 = -127;
		}
		else{
			lift1 = 0;
			lift2 = 0;
		}
	}
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	leftFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightFront.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	leftBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	rightBack.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	intakeLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	intakeRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	lift1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	lift2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);


	while (true) {
		liftAvg = (fabs(lift1.get_position()) + fabs(lift2.get_position())/2);
	pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
									 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
									 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
	int left = master.get_analog(ANALOG_LEFT_Y);
	int right = master.get_analog(ANALOG_RIGHT_Y);
	pros::lcd::set_text(6, "Lift: " + std::to_string(((lift1.get_position() + lift2.get_position())/2)));

	//Drive

	leftFront = left;
	leftBack = left;
	rightFront = right;
	rightBack = right;

	//Lift
	if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
		while(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
			pros::delay(1);
		}
			liftToggle = !liftToggle;
	}
	if(liftToggle){
		moveLiftUp(3000);
	}
	else{
		moveLiftDown(0);
	}

	//Intake

	int intakePower;
	intakeLeft = intakePower;
	intakeRight = intakePower;

	if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
		intakePower = 127;
	}
	else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
		intakePower = -127;
	}
	else{
		intakePower = 0;
	}
	}
}

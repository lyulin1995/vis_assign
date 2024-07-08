#include "robot_controller.h"
#include <iostream>

RobotController::RobotController() {
  motors.fill(RobotController::MotorState::kDisabled);
  state = RobotController::State::kDisabled;
}

void RobotController::Enable() {
  switch(state) {
  case RobotController::State::kDisabled:
    state = RobotController::State::kEnabling;
    return;
  default:
    return;
  }
}

void RobotController::Disable() {
  switch(state) {
  case RobotController::State::kEnabled:
  case RobotController::State::kEnabling:
    motors.fill(RobotController::MotorState::kDisabled);
    state = RobotController::State::kDisabled;
    return;
  default:
    return;
  }
}

void RobotController::ResetFault() {
  switch(state) {
  case RobotController::State::kFault:
    motors.fill(RobotController::MotorState::kDisabled);
    state = RobotController::State::kDisabled;
    return;
  default:
    return;
  }
}

void RobotController::Update() {
  // Check for new faults
  bool hasFault;
  for (auto motor: motors) {
    if (motor == RobotController::MotorState::kFault) {
      state = RobotController::State::kFault;
      return;
    }
  }

  // Enable next motor in enabling state
  if (state == RobotController::State::kEnabling) {
    int motor_i = 0;
    for (; motor_i < MOTOR_COUNT; motor_i++) {
      if (motors[motor_i] == RobotController::MotorState::kDisabled) {
        motors[motor_i] = RobotController::MotorState::kEnabled;
        break;
      }
    }

    // All motors are enabled
    if (motor_i == MOTOR_COUNT -1) {
      state = RobotController::State::kEnabled;
    }
  }
}

void RobotController::SimulateMotorFault(int motor_index) {
  motors[motor_index] = RobotController::MotorState::kFault;
}
#pragma once
#include <array>

#define MOTOR_COUNT 6

class RobotController {
public:
  enum class State { kDisabled = 1, kEnabling, kEnabled, kFault };
  enum class Command { kDisable = 1, kEnable, kResetFault };
  enum class MotorState {kDisabled = 1, kEnabled, kFault};

  RobotController();

  // Start the enabling process of all motors of the RobotController
  void Enable();
  // Disable all motors of the RobotController at once
  void Disable();
  // Reset all motor values to disabled
  void ResetFault();
  // Run a single step of the RobotController process
  void Update();

  // Testing function
  void SimulateMotorFault(int motor_index);

  RobotController::State state;
  std::array<RobotController::MotorState,MOTOR_COUNT> motors;
};

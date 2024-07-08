#pragma once
#include "image_processing_service.h"
#include "robot_controller.h"

class VenipunctureService {
public:
  enum class Command { kOff = 1, kOn, kReset };
  enum class State { kOff = 1, kSwitchingOn, kOn, kFault };

  VenipunctureService(RobotController& robot_controller);

  // Switch to kSwitchingOn state
  void HandleOn();
  // Switch to kOff state
  void HandleOff();
  // Resets fault state and turns of robotcontroller
  void handleReset();
  // Check for robot controller updates and update own state accordingly
  void Update();

  VenipunctureService::State state;
  ImageProcessingService image_processing_service;
  RobotController& robot_controller;
};

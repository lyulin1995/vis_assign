#pragma once

#include "robot_controller.h"
#include "venipuncture_service.h"

class RobotControllerUI {
public:
  RobotControllerUI(RobotController& robot_controller, VenipunctureService& venipuncture_service);
  void Run();

private:
  int RequestCommand();
  void ShowCommandNotYetImplemented(int command);
  // NOTE: added venipuncture service here to call its update function from the UI update command
  void HandleCommand(RobotController& robot_controller, VenipunctureService & venipuncture_service, int command);

  void PrintState();

  RobotController& robot_controller_;
  VenipunctureService& venipuncture_service_;
};

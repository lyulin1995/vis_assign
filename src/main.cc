#include "robot_controller.h"
#include "robot_controller_ui.h"
#include "venipuncture_service.h"

#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "====> Software engineer assignment - Development exercise <===="
            << std::endl
            << std::endl;

  RobotController robot_controller;
  VenipunctureService venipuncture_service(robot_controller);
  RobotControllerUI robot_controller_ui(robot_controller, venipuncture_service);
  robot_controller_ui.Run();

  return 0;
}

#include "robot_controller_ui.h"

#include <iostream>

RobotControllerUI::RobotControllerUI(RobotController &robot_controller, VenipunctureService &venipuncture_service)
    : robot_controller_(robot_controller), venipuncture_service_(venipuncture_service) {}

int RobotControllerUI::RequestCommand() {
  std::cout << "Available commands:" << std::endl
            << "0. Quit" << std::endl
            << "1. Disable robot controller" << std::endl
            << "2. Enable robot controller" << std::endl
            << "3. Reset robot controller fault" << std::endl
            << "4. Execute single robot controller step" << std::endl
            << "==> Enter command: ";

  int command;
  std::cin >> command;

  if (std::cin.eof() || std::cin.bad()) {
    command = -1;
  } else if (std::cin.fail()) {
    std::cin.clear(); // unset failbit
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                    '\n'); // skip bad input
    command = -1;
  }

  std::cout << std::endl;

  return command;
}

void RobotControllerUI::ShowCommandNotYetImplemented(int command) {
  std::cout << "WARNING: Command " << command << " not yet implemented."
            << std::endl
            << std::endl;
}

void RobotControllerUI::HandleCommand(RobotController &robot_controller,
                                      VenipunctureService & venipuncture_service,
                                      int command) {
  switch (command) {
  case 1:
    robot_controller.Disable();
    venipuncture_service.HandleOff();
    break;
  case 2:
    robot_controller.Enable();
    venipuncture_service.HandleOn();
    break;
  case 3:
    robot_controller.ResetFault();
    venipuncture_service.handleReset();
    break;
  case 4:
    robot_controller.Update();
    venipuncture_service.Update();
    std::cout << "INFO: Executed single robot controller step" << std::endl
              << std::endl;
    break;
  default:
    std::cout << "ERROR: Invalid command " << command << std::endl << std::endl;
    break;
  }
}

void RobotControllerUI::Run() {
  bool stop_command_received = false;
  while (!stop_command_received) {
    PrintState();
    const int command = RequestCommand();
    if (command > 0) {
      HandleCommand(robot_controller_,venipuncture_service_, command);
    } else if (command < 0) {
      std::cout << "ERROR: Input error occurred" << std::endl << std::endl;
    } else if (command == 0) {
      stop_command_received = true;
    }
  }
}

void RobotControllerUI::PrintState() {
  std::cout << "=== Current status ===" << std::endl;
//  std::cout << "Current state:" << std::endl;
  std::cout << "Light status:                " << (int) venipuncture_service_.image_processing_service.state() << std::endl;
  std::cout << "VeniPunctureService status:  " << (int) venipuncture_service_.state << std::endl;
  std::cout << "RobotController status:      " << (int) robot_controller_.state << std::endl;
  std::cout << "Robots:                      [";
  for (auto motor: robot_controller_.motors) {
    std::cout << (int) motor << ", ";
  }
  std::cout << "]" << std::endl;
  std::cout << "======" << std::endl;
}

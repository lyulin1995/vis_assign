#include "venipuncture_service.h"
#include "robot_controller.h"

VenipunctureService::VenipunctureService(RobotController& robot_controller): state(VenipunctureService::State::kOff), robot_controller(robot_controller) {}

void VenipunctureService::HandleOn() {
  switch (state) {
  case VenipunctureService::State::kOff:
    image_processing_service.set_state(ImageProcessingService::LampState::kOn);
    state = VenipunctureService::State::kSwitchingOn;
    return;
  default:
    return;
  }
}

void VenipunctureService::HandleOff() {
  switch (state) {
  case VenipunctureService::State::kOn:
  case VenipunctureService::State::kSwitchingOn:
    image_processing_service.set_state(
        ImageProcessingService::LampState::kOff);
    return;
  default:
    return;
  }
}

void VenipunctureService::handleReset(){
  image_processing_service.set_state(
      ImageProcessingService::LampState::kOff);
}

void VenipunctureService::Update(){
  switch (state) {
  case VenipunctureService::State::kFault:
    if (robot_controller.state == RobotController::State::kDisabled) {
      state = VenipunctureService::State::kOff;
    }
    return;
  case VenipunctureService::State::kOff:
  case VenipunctureService::State::kOn:
    if (robot_controller.state == RobotController::State::kFault) {
      state = VenipunctureService::State::kFault;
    }
    if (robot_controller.state == RobotController::State::kDisabled) {
      state = VenipunctureService::State::kOff;
    }
    return;
  case VenipunctureService::State::kSwitchingOn:
    if (robot_controller.state == RobotController::State::kEnabled) {
      state = VenipunctureService::State::kOn;
    }
    if (robot_controller.state == RobotController::State::kFault) {
      state = VenipunctureService::State::kFault;
    }
    if (robot_controller.state == RobotController::State::kDisabled) {
      state = VenipunctureService::State::kOff;
    }
    return;
  }
}
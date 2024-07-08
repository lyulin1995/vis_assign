#include "image_processing_service.h"

#include <gmock/gmock.h>
#include <robot_controller.h>
#include <venipuncture_service.h>

TEST(ImageProcessingService, TestInitialState) {
  ImageProcessingService image_processing_service;
  EXPECT_EQ(image_processing_service.state(), ImageProcessingService::LampState::kOff);

  RobotController robot_controller;
  EXPECT_EQ(robot_controller.state, RobotController::State::kDisabled);
}


TEST(RobotController, TestInitialState) {
  RobotController robot_controller;
  EXPECT_EQ(robot_controller.state, RobotController::State::kDisabled);
}


TEST(RobotController, TestTurningOn) {
  RobotController robot_controller;
  robot_controller.Enable();
  EXPECT_EQ(robot_controller.state, RobotController::State::kEnabling);
  for (int i = 0; i < MOTOR_COUNT; i++) {
    robot_controller.Update();
  }
  EXPECT_EQ(robot_controller.state, RobotController::State::kEnabled);
}

TEST(RobotController, TestTurningOff) {
  RobotController robot_controller;
  robot_controller.Enable();
  for (int i = 0; i < MOTOR_COUNT; i++) {
    robot_controller.Update();
  }
  robot_controller.Disable();
  EXPECT_EQ(robot_controller.state, RobotController::State::kDisabled);
}

TEST(VenipunctureService, TestInitialState) {
  RobotController robot_controller;
  ImageProcessingService image_processing_service;
  VenipunctureService venipuncture_service(robot_controller);
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kOff);
}

TEST(VenipunctureService, TestHandleOn) {
  RobotController robot_controller;
  VenipunctureService venipuncture_service(robot_controller);

  venipuncture_service.HandleOn();
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kSwitchingOn);
  EXPECT_EQ(venipuncture_service.image_processing_service.state(), ImageProcessingService::LampState::kOn);

  robot_controller.Enable();
  for (int i = 0; i < MOTOR_COUNT; i++) {
    robot_controller.Update();
  }
  venipuncture_service.Update();
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kOn);
}

TEST(VenipunctureService, TestHandleOff) {
  RobotController robot_controller;
  VenipunctureService venipuncture_service(robot_controller);

  // Enable
  venipuncture_service.HandleOn();
  robot_controller.Enable();
  for (int i = 0; i < MOTOR_COUNT; i++) {
    robot_controller.Update();
  }
  venipuncture_service.Update();

  // Disable
  venipuncture_service.HandleOff();
  robot_controller.Disable();

  // After disabling, lamp should be disabled, status still on until motorcontroller has changes status
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kOn);
  EXPECT_EQ(venipuncture_service.image_processing_service.state(), ImageProcessingService::LampState::kOff);

  venipuncture_service.Update();

  // After updating and receiving new robot controller state, state should be updated
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kOff);
}

TEST(Integration, TestFaultFromEnabled) {
  RobotController robot_controller;
  VenipunctureService venipuncture_service(robot_controller);

  // Enable
  venipuncture_service.HandleOn();
  robot_controller.Enable();
  for (int i = 0; i < MOTOR_COUNT; i++) {
    robot_controller.Update();
  }
  venipuncture_service.Update();

  // Insert fault
  robot_controller.SimulateMotorFault(4);
  robot_controller.Update();
  venipuncture_service.Update();

  EXPECT_EQ(robot_controller.state, RobotController::State::kFault);
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kFault);
}

TEST(Integration, TestFaultFromEnabling) {
  RobotController robot_controller;
  VenipunctureService venipuncture_service(robot_controller);

  // Turn on, but fault occurs during startup of motors.
  venipuncture_service.HandleOn();
  robot_controller.Enable();
  for (int i = 0; i < MOTOR_COUNT-1; i++) {
    robot_controller.Update();
  }
  venipuncture_service.Update();

  // Insert fault
  robot_controller.SimulateMotorFault(2);
  robot_controller.Update();
  venipuncture_service.Update();

  EXPECT_EQ(robot_controller.state, RobotController::State::kFault);
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kFault);
}

TEST(Integration, ResetFault) {
  RobotController robot_controller;
  VenipunctureService venipuncture_service(robot_controller);

  // Turn on
  venipuncture_service.HandleOn();
  robot_controller.Enable();
  for (int i = 0; i < MOTOR_COUNT; i++) {
    robot_controller.Update();
  }
  venipuncture_service.Update();

  // Insert fault
  robot_controller.SimulateMotorFault(2);
  robot_controller.Update();
  venipuncture_service.Update();

  // Reset
  robot_controller.ResetFault();
  venipuncture_service.handleReset();
  robot_controller.Update();
  venipuncture_service.Update();
  EXPECT_EQ(robot_controller.state, RobotController::State::kDisabled);
  EXPECT_EQ(venipuncture_service.state, VenipunctureService::State::kOff);
  EXPECT_EQ(venipuncture_service.image_processing_service.state(), ImageProcessingService::LampState::kOff);
}

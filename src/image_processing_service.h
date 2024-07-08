#pragma once

class ImageProcessingService {
public:
  enum class LampState { kOff = 1, kOn };

  void set_state(LampState state) { lamp_state_ = state; }

  LampState state() const { return lamp_state_; }

private:
  LampState lamp_state_ = LampState::kOff;
};

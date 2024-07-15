#pragma once

#include <controller/reflector_state.hpp>

namespace voltiris::controller::utils {

template <typename T>
class Changeable {
public:
  explicit Changeable() = default;
  explicit Changeable(T value) : _value(value) {}

  [[nodiscard]] bool IsChanged() const { return _changed; }
  void Changed() { _changed = true; }
  void ResetChanged() { _changed = false; }

  T& GetValue() { return _value; }

private:
  T _value;
  bool _changed = false;
};

}  // namespace voltiris::controller::utils
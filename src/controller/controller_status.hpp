#pragma once

namespace voltiris::controller {

enum class ControllerStatus { kIdle, kOperating, kMoving, kTracking, kCommunicating };

}  // namespace voltiris::controller
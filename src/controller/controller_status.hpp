#pragma once

namespace voltiris::controller {

enum class ControllerStatus { kOperating, kMoving, kTracking, kCommunicating, kTrackingWaiting, kTrackingReturningNight };

}  // namespace voltiris::controller
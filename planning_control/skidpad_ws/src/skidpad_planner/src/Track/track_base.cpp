#include "Track/track_base.hpp"

namespace ns_skidpad_planner {

void Track::setState(const VehicleState &state) { state_ = state; }
void Track::setTransMat(const Eigen::Matrix4f &transMat) { transMat_ = transMat; }

} // namespace ns_skidpad_planner
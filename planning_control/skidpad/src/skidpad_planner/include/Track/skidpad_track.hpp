#pragma once
#include "track_base.hpp"

namespace ns_skidpad_planner {

class Skidpad_Track : public Track {
public:
    bool genTraj() override;
    bool CalculateTraj(Trajectory &refline) override;
    int getPhase() const { return current_phase_; }

private:
    int now_state = 0;
    int current_phase_ = 0;
    int prev_phase_ = -1;
    // 各阶段起始索引
    size_t idx_end_straight1_ = 0;
    size_t idx_end_right_ = 0;
    size_t idx_end_left_ = 0;
    int computePhase(size_t index) const;
};

} // namespace ns_skidpad_planner
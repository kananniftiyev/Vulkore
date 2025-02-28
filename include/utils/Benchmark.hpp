#pragma once

#include <iostream>
#include <vector>

namespace Utils::Benchmark
{
    float calculateAverageFps(std::vector<int> &fps_list);
    float calculateAverageMs(std::vector<float> &ms_list);
} // namespace Utils

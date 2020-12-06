#pragma once
#include <vector>

namespace cv
{
    class Mat;
}

class HistrogramStretcher
{
public:
    cv::Mat Stretch(const cv::Mat& input);
    cv::Mat GetHistogram(const cv::Mat& input);

private:
    std::vector<size_t> CalculateHistogram(const cv::Mat& input);
    std::vector<size_t> CalculateLookupTable(const std::vector<size_t>& histogram);
    std::pair<size_t, size_t> CalculateNewBoundaries(const std::vector<size_t>& histogram);
    cv::Mat ApplyStretching(const cv::Mat& input, const std::vector<size_t>& lookupTable);
};


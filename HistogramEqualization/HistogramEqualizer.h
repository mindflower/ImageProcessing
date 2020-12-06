#pragma once
#include <vector>

namespace cv
{
    class Mat;
}

class HistogramEqualizer
{
public:
    cv::Mat Equalize(const cv::Mat& input);
    cv::Mat GetHistogram(const cv::Mat& input);

private:
    std::vector<size_t> CalculateHistogram(const cv::Mat& input);
    std::vector<size_t> CalculateLookupTable(const cv::Mat& input, const std::vector<size_t>& histogram);
    cv::Mat ApplyEqualization(const cv::Mat& input, const std::vector<size_t>& lookupTable);
};

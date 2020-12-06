#pragma once

namespace cv
{
    class Mat;
}

class RobertsFilter
{
public:
    cv::Mat ProcessImage(const cv::Mat& input);
};


#pragma once

namespace cv
{
    class Mat;
}

class SobelFilter
{
public:
    cv::Mat ProcessImage(const cv::Mat& input);
};


#pragma once

namespace cv
{
    class Mat;
}

class PrewittFilter
{
public:
    cv::Mat ProcessImage(const cv::Mat& input);
};


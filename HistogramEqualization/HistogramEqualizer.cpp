#include "HistogramEqualizer.h"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace cv;

namespace
{
    const size_t MAX_COLOR_VALUE = 255;
    const size_t HISTOGRAM_SIZE = 256;
}

Mat HistogramEqualizer::Equalize(const cv::Mat& input)
{
    Mat output;
    input.copyTo(output);
    const auto histogram = CalculateHistogram(output);
    const auto lookupTable = CalculateLookupTable(output, histogram);
    return ApplyEqualization(output, lookupTable);
}

Mat HistogramEqualizer::GetHistogram(const Mat& input)
{
    const auto histogram = CalculateHistogram(input);
    cv::Mat histgramImage = Mat::zeros(256, 256, CV_8UC1);
    const auto max = *minmax_element(histogram.cbegin(), histogram.cend()).second;
    size_t x = 0;
    for (const auto val : histogram)
    {
        const auto height = cvRound(val * 256.0 / max);
        line(histgramImage, Point(x, 256 - height), Point(x, 256), Scalar::all(255));
        ++x;
    }
    return histgramImage;
}

vector<size_t> HistogramEqualizer::CalculateHistogram(const Mat& input)
{
    vector<size_t> histogram(HISTOGRAM_SIZE, 0);
    for (int row = 0; row < input.rows; row++)
    {
        for (int col = 0; col < input.cols; col++)
        {
            const int color = static_cast<size_t>(input.at<uchar>(row, col));
            histogram[color]++;
        }
    }
    return histogram;
}

vector<size_t> HistogramEqualizer::CalculateLookupTable(const Mat& input, const vector<size_t>& histogram)
{
    const auto nonZeroColor = *find_if(cbegin(histogram), cend(histogram), [](const auto color) -> bool
    {
        return color != 0;
    });
    const float scale = static_cast<float>(MAX_COLOR_VALUE) / ((input.rows * input.cols) - histogram[nonZeroColor]);

    size_t sum = 0;
    vector<size_t> lookupTable(HISTOGRAM_SIZE, 0);
    transform(begin(histogram), end(histogram), begin(lookupTable), [&sum, &scale](const auto colorValue) -> size_t
    {
        sum += colorValue;
        return cv::saturate_cast<unsigned char>(sum * scale);
    });
    return lookupTable;
}

Mat HistogramEqualizer::ApplyEqualization(const Mat& input, const vector<size_t>& lookupTable)
{
    Mat output;
    input.copyTo(output);
    for (int row = 0; row < output.rows; row++)
    {
        for (int col = 0; col < output.cols; col++)
        {
            output.at<uchar>(row, col) = lookupTable[static_cast<size_t>(output.at<uchar>(row, col))];
        }
    }
    return output;
}

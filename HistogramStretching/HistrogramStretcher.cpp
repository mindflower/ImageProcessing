#include "HistrogramStretcher.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

namespace
{
    const size_t MAX_COLOR_VALUE = 255;
    const size_t HISTOGRAM_SIZE = 256;

    const size_t LEFT_BOUND = 0;
    const size_t RIGHT_BOUND = 255;
    const size_t THRESHOLD = 10;
}

Mat HistrogramStretcher::Stretch(const Mat& input)
{
    Mat output;
    input.copyTo(output);
    const auto histogram = CalculateHistogram(output);
    const auto lookupTable = CalculateLookupTable(histogram);
    return ApplyStretching(output, lookupTable);
}

Mat HistrogramStretcher::GetHistogram(const Mat& input)
{
    const auto histogram = CalculateHistogram(input);
    cv::Mat histgramImage = Mat::zeros(256, 256, CV_8UC1);
    const auto [minIt, maxIt] = minmax_element(histogram.cbegin(), histogram.cend());
    const auto max = *maxIt;
    size_t x = 0;
    for (const auto val : histogram)
    {
        const auto height = cvRound(val * 256.0 / max);
        line(histgramImage, Point(x, 256 - height), Point(x, 256), Scalar::all(255));
        ++x;
    }
    return histgramImage;
}

vector<size_t> HistrogramStretcher::CalculateHistogram(const Mat& input)
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

vector<size_t> HistrogramStretcher::CalculateLookupTable(const vector<size_t>& histogram)
{
    const auto [leftBound, rightBound] = CalculateNewBoundaries(histogram);
    vector<size_t> lookupTable(HISTOGRAM_SIZE, 0);
    const auto RIGHT_MINUS_LEFT = RIGHT_BOUND - LEFT_BOUND;
    for (size_t i = 0; i < HISTOGRAM_SIZE; ++i)
    {
        lookupTable[i] = static_cast<unsigned char>((i - leftBound) * (RIGHT_MINUS_LEFT) / (rightBound - leftBound) + LEFT_BOUND);
    }
    return lookupTable;
}


pair<size_t, size_t> HistrogramStretcher::CalculateNewBoundaries(const vector<size_t>& histogram)
{
    const auto leftBoundIt = find_if(cbegin(histogram), cend(histogram), [](const auto color) -> bool
    {
        return color > THRESHOLD;
    });

    const auto rightBoundIt = find_if(crbegin(histogram), make_reverse_iterator(leftBoundIt), [](const auto color) -> bool
    {
        return color > THRESHOLD;
    }).base();

    return {distance(cbegin(histogram), leftBoundIt), distance(cbegin(histogram), rightBoundIt)};
}

Mat HistrogramStretcher::ApplyStretching(const Mat& input, const vector<size_t>& lookupTable)
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

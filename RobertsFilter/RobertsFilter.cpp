#include "RobertsFilter.h"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

Mat RobertsFilter::ProcessImage(const Mat& input)
{
    Mat output;
    input.copyTo(output);
    const vector<vector<int>> xKernel = { { 1, 0}, { 0, -1} };
    const vector<vector<int>> yKernel = { { 0, 1}, {-1,  0} };

    const auto GetPixel = [](Mat input, int x, int y) -> uchar
    {
        if ((x < 0) || (x >= input.cols) || (y < 0) || (y >= input.rows))
        {
            return 0;
        }
        else
        {
            return input.at<uchar>(y, x);
        }
    };

    for (int col = 0; col < output.cols; ++col)
    {
        for (int row = 0; row < output.rows; ++row)
        {
            double hAccumulator = 0;
            double vAccumulator = 0;

            for (int kCol = 0; kCol <= 1; ++kCol)
            {
                for (int kRow = 0; kRow <= 1; ++kRow)
                {
                    hAccumulator += static_cast<double>(GetPixel(output, col + kCol, row + kRow)) * xKernel[kRow][kCol];
                    vAccumulator += static_cast<double>(GetPixel(output, col + kCol, row + kRow)) * yKernel[kRow][kCol];
                }
            }

            double sum = clamp(abs(hAccumulator) + abs(vAccumulator), 0.0, 255.0);
            output.at<uchar>(row, col) = static_cast<uchar>(sum);
        }
    }
    return output;
}

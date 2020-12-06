#include "SobelFilter.h"
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat SobelFilter::ProcessImage(const Mat& input)
{
    Mat output;
    input.copyTo(output);

    const vector<vector<int>> xKernel = { { -1, 0, 1}, { -2, 0, 2}, {-1, 0, 1} };
    const vector<vector<int>> yKernel = { { -1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

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

    for (int col = 0; col < input.cols; ++col)
    {
        for (int row = 0; row < input.rows; ++row)
        {
            int hAccumulator = 0;
            int vAccumulator = 0;

            for (int kCol = 0; kCol <= 2; ++kCol)
            {
                for (int kRow = 0; kRow <= 2; ++kRow)
                {
                    hAccumulator += GetPixel(input, col + kCol, row + kRow) * xKernel[kRow][kCol];
                    vAccumulator += GetPixel(input, col + kCol, row + kRow) * yKernel[kRow][kCol];
                }
            }

            int sum = clamp(abs(hAccumulator) + abs(vAccumulator), 0, 255);
            output.at<uchar>(row, col) = static_cast<uchar>(sum);
        }
    }
    return output;
}

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "HistogramEqualizer.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    const string input = argc == 2 ? argv[1] : "image.jpg";
    Mat src = imread(samples::findFile(input), IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }
    Mat out;
    cvtColor(src, out, COLOR_BGR2GRAY);
    HistogramEqualizer equalizer;
    const auto equalizedImage = equalizer.Equalize(out);
    imshow("Source image", out);
    imshow("Equalized Image", equalizedImage);
    imshow("Source histogram", equalizer.GetHistogram(out));
    imshow("Equalized histogram", equalizer.GetHistogram(equalizedImage));
    waitKey();
    return 0;
}

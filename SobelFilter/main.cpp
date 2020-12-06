#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "SobelFilter.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    const string input = argc == 2 ? argv[1] : "image.png";
    Mat src = imread(samples::findFile(input), IMREAD_COLOR);
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }
    Mat out;
    cvtColor(src, out, COLOR_BGR2GRAY);
    SobelFilter filter;
    imshow("Source image", out);
    imshow("Sobel filter Image", filter.ProcessImage(out));
    waitKey();
    return 0;
}

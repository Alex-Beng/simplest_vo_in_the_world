#include "image_process.h"

int main()
{
    cv::VideoCapture camera(0);
    cv::Mat frame;
    cv::namedWindow("raw_input");
    while (1){
        camera>>frame;
        if (frame.empty()){
            break;
        }
        cv::imshow("raw_input",frame);
        cv::waitKey(1);
    }
    return 0;
}
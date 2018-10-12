#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
int main(){
    int i=0;
    double j = i;
    cv::VideoCapture camera(1);
    cv::Mat frame;
    cv::namedWindow("test");
    while (1){
        camera>>frame;
        if (frame.empty()){
            // break;
            continue;
        }
        cv::line(frame,cv::Point(frame.cols/2,0),cv::Point(frame.cols,frame.rows),cv::Scalar(255,0,0));
        cv::imshow("test",frame);
        char t = cv::waitKey(1);
        if (t == 'q') {
            break;
        }
    }
    return 0;
}
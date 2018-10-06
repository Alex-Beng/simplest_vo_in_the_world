#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
int main(){
    int i=0;
    double j = i;
    cv::VideoCapture camera(0);
    cv::Mat frame;
    cv::namedWindow("test");
    while (1){
        camera>>frame;
        if (frame.empty()){
            break;
        }
        cv::imshow("test",frame);
        cv::waitKey(1);
    }
    return 0;
}
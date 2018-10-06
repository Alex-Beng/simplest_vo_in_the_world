#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

const int max_thre = 175;
int thre;
int block_size = 2;
int ksize = 3;
cv::Mat used_channel;
void on_trackerbar( int, void* );

int main() {
    cv::VideoCapture cp(0);
    cv::Mat frame;
    cv::namedWindow("src");
    cv::createTrackbar("src","block_size",&thre,max_thre,on_trackerbar);
    while (1) {
        cp>>frame;
        if (frame.empty()) {
            break;
        }
        cv::imshow("src",frame);
        cv::Mat t_channels[3];
        cv::cvtColor(frame,frame,CV_BGR2HLS);
        cv::split(frame,t_channels);
        used_channel = t_channels[1];
        on_trackerbar(0,0);
        cv::waitKey(1);
    }
    return 0;
}
void on_trackerbar(int,void*) {
    cv::Mat dst_image = cv::Mat::zeros(used_channel.size(),CV_32FC1);
    cv::Mat scaled_image;
    cv::cornerHarris(used_channel,dst_image,block_size,ksize,0.4,cv::BORDER_DEFAULT);
    cv::convertScaleAbs(dst_image,scaled_image);
    for (int j=0; j<scaled_image.rows; j++) {
        for (int i=0; i<scaled_image.cols; i++){
            if ( static_cast<int>(scaled_image.at<float>(j,i)) > thre ) {
                cv::circle(used_channel,cv::Point(i,j),3,cv::Scalar(0,255,255));
            }
        }
    }
    cv::imshow("proc",used_channel);
}
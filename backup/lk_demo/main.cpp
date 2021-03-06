#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;
using namespace cv;

void tracking(Mat &frame, Mat &output);
bool acceptTrackedPoint(int i);

string window_name = "optical flow tracking";
Mat gray;    
Mat gray_prev;
Mat map_ = cv::Mat(600,600,CV_8UC1,cv::Scalar(255));
Mat transf_element = cv::Mat(3,3,CV_64FC1);
Point2f current_position(290.0,290.0);
Point2f forward_direction;
vector<Point2f> points[2];    
vector<Point2f> initial;
vector<Point2f> features;
int maxCount = 500;    
double qLevel = 0.01;    
double minDist = 10.0;    
vector<uchar> status;    
vector<float> err;


int main() {
    // load the element.
    std::ifstream in_file("../../src/ini/777.txt");
    if (!in_file) {
        cerr<<"error in Load element"<<endl;
    }
    double t;
    for (int i=0; i< transf_element.cols; i++) {
        for (int j=0; j<transf_element.rows; j++) {
            double t;
            in_file>>t;
            transf_element.at<double>(i,j) = t;
        }
    }
    in_file.close();
    cout<<transf_element<<endl;

    // load the forward direction.
    in_file.open("../../src/ini/666.txt");
    if (!in_file) {
        cerr<<"error in Load direction"<<endl;
    }
    in_file>>forward_direction.x>>forward_direction.y;
    cout<<forward_direction<<endl;

    Mat frame;
    Mat result;

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cv::waitKey(1000);
    }
    while(true) {
        capture >> frame;

        if (frame.empty()) { 
            printf("wait for camera initing...\n");
            continue;
        }
        // apply the perspective transform
        cv::warpPerspective(frame,frame,transf_element,frame.size());
        cv::flip(frame,frame,-1);

        tracking(frame,result);
        int c = waitKey(1);
        if( (char)c == 27 )
        {
            break; 
        } 
    }
    return 0;
}


void tracking(Mat &frame, Mat &output) {
    cv::Mat t_mat, t_channels[3];
    cvtColor(frame, t_mat, CV_BGR2HLS);
    cv::split(t_mat,t_channels);
    gray = t_channels[1];
    // cvtColor(frame, gray, CV_BGR2GRAY);

    frame.copyTo(output);

    if (points[0].size() <= 10) { 
        goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
        points[0].insert(points[0].end(), features.begin(), features.end());
        initial.insert(initial.end(), features.begin(), features.end());
    }

    if (gray_prev.empty()) {   
        gray.copyTo(gray_prev);
    }

    calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], status, err);

    int k = 0;
    for (size_t i=0; i<points[1].size(); i++)
    {
        if (acceptTrackedPoint(i))
        {
            initial[k] = initial[i];
            points[1][k++] = points[1][i];
        }
    }
    points[1].resize(k);
    initial.resize(k);

    double sum_delta_row = 0.0;
    double sum_delta_col = 0.0;
    
    for (size_t i=0; i<points[1].size(); i++) {
        sum_delta_row += (points[1][i].y - initial[i].y);
        sum_delta_col += (points[1][i].x - initial[i].x); 
        line(output, initial[i], points[1][i], Scalar(0, 0, 255));
        circle(output, points[1][i], 3, Scalar(0, 255, 0), -1);
    }
    
    sum_delta_col /= points[1].size();
    sum_delta_row /= points[1].size();
    if (fabs(sum_delta_col)>10 || fabs(sum_delta_row)>10) { // 平均delta超过阈值
        current_position.y += sum_delta_row*forward_direction.y + sum_delta_col*forward_direction.x; // row 方向上的delta 
        current_position.x += sum_delta_row*(-1.0*forward_direction.x) + sum_delta_col*forward_direction.y;// col 方向上的

        // current_position.x = current_position.x + sum_delta_col*0.1;
        // current_position.y = current_position.y + sum_delta_row*0.1;
        // cout<<current_position<<endl<<endl;
        // current_position.y = 
    }
    map_.copyTo(t_mat);
    // cv::rectangle(t_mat,current_position,Scalar(0),2);
    cv::circle(t_mat,current_position,10,cv::Scalar(0),2);

    std::swap(points[1], points[0]);
    std::swap(gray_prev, gray);
    cv::imshow("result",t_mat);
    cv::imshow(window_name, output);
}

bool acceptTrackedPoint(int i)
{
    return status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
}



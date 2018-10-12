#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;

void tracking(Mat &frame, Mat &output);
bool acceptTrackedPoint(int i);

string window_name = "optical flow tracking";
Mat gray;    
Mat gray_prev;

Mat transf_element = cv::Mat(3,3,CV_64FC1);
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
    cout<<transf_element<<endl;
    Mat frame;
    Mat result;

    VideoCapture capture(1);
    if (!capture.isOpened()) {
        cv::waitKey(1000);
    }
    while(true) {
        capture >> frame;

        if (frame.empty()) { 
            printf("wait for camera initing...");
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

    if (points[0].size() <= 10) { // ÉÏÒ»Ö¡µÄµã²»¹»ÓÃÁË, ÐèÒªÌí¼ÓÐÂµã
        goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
        points[0].insert(points[0].end(), features.begin(), features.end());
        initial.insert(initial.end(), features.begin(), features.end());
    }

    if (gray_prev.empty()) {    // µÚÒ»Ö¡perv»¹Îª¿Õ
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
    // ï¿½ï¿½Ê¾ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ë¶ï¿½ï¿½ì¼?
    for (size_t i=0; i<points[1].size(); i++)
    {
        line(output, initial[i], points[1][i], Scalar(0, 0, 255));
        circle(output, points[1][i], 3, Scalar(0, 255, 0), -1);
    }

    // ï¿½Ñµï¿½Ç°ï¿½ï¿½ï¿½Ù½ï¿½ï¿½ï¿½ï¿½Î?ï¿½ï¿½Ò»ï¿½Ë²Î¿ï??
    swap(points[1], points[0]);
    swap(gray_prev, gray);

    imshow(window_name, output);
}

bool acceptTrackedPoint(int i)
{
    return status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
}



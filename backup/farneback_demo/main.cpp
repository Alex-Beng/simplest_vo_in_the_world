#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}

int main(int, char**)
{
	cv::Mat transf_element = cv::Mat(3,3,CV_64FC1);
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

    VideoCapture cap(1);

    Mat prevgray, gray, flow, cflow, frame;
    namedWindow("flow", 1);

    for(;;)
    {
        cap >> frame;
		if (frame.empty()) { // 等待相机开启
			continue;
		}
		cv::warpPerspective(frame,frame,transf_element,frame.size());
		cv::flip(frame,frame,-1);
		
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( prevgray.data )
        {
            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));
            imshow("flow", cflow);
        }
        if(waitKey(30)>=0)
            break;
        std::swap(prevgray, gray);
    }
    return 0;
}

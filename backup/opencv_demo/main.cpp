#include <iostream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

void help()
{
    cout<<"\nThis program demonstrates finding the minimum enclosing box or circle of a set\n"
        "of points using functions: minAreaRect() minEnclosingCircle().\n"
        "Random points are generated and then enclosed.\n"
        "Call:\n"
        "./minarea\n";
}

int main( int argc, char** argv ){
    help();
    cv::Mat srcImg = cv::imread("/cat2.jpg");
    if (srcImg.empty()){
        cout<<"the srcImg is empty!"<<endl;
        srcImg = cv::Mat();
    }
    cv::namedWindow("hello world !",CV_WINDOW_NORMAL);
    cv::imshow("hello world !",srcImg);
    cv::waitKey(0);
    return 0;
}

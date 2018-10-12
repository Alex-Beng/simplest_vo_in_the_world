#include "../../src/ini/minIni.h"
#include <algorithm>
#include <iostream> 
#include <iomanip>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>    
 
using namespace std;      
using namespace cv;

const int max_thre = 255;
int binary_thre = 100;
int hough_thre = 93;
int hough_stn_thre = 0;
int hough_srn_thre = 0;
int corner_distance_thre = 90;
// for forward direction
cv::Point forward_points[2];
int forward_count = 0;

static void on_mouse(int,int,int,int,void*);
void on_trackerbar(int,void*);
bool sort_method(cv::Point2f,cv::Point2f);
double get_distance(cv::Point2f,cv::Point2f);

cv::Mat imageSource;
cv::Mat image;
cv::Mat transf_element; // the ans mat;

int camera_count = 1;
cv::VideoCapture cp(camera_count);

int main(int argc,char*argv[])    
{  
    while (1) {
        cv::namedWindow("src");
        cv::createTrackbar("binary_thre","binary",&binary_thre,max_thre,on_trackerbar);
        cv::createTrackbar("hough_thre","line",&hough_thre,max_thre,on_trackerbar);
        cv::createTrackbar("hough_stn_thre","line",&hough_stn_thre,100,on_trackerbar);
        cv::createTrackbar("hough_srn_thre","line",&hough_srn_thre,100,on_trackerbar);
        cv::createTrackbar("corner_distance_thre","points",&corner_distance_thre,max_thre,on_trackerbar);
        cp>>image;
        if (image.empty()) {    // 解决打不带usb摄像头的问题...
            // cp = cv::VideoCapture(++camera_count);
            // cout<<camera_count<<endl;
            cout<<"waiting for camera init..."<<endl;
            continue;
        }
        image.copyTo(imageSource);
        // cv::cvtColor(image,image,CV_BGR2GRAY);
        cv::Mat t_channels[3];
        cv::cvtColor(image,image,CV_BGR2Lab);
        cv::split(image,t_channels);
        image = t_channels[0];

        cv::imshow("src",imageSource);
        GaussianBlur(image,image,Size(3,3),0);  //滤波
        image = image > binary_thre; 
        imshow("binary",image);	
        
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        
        cv::findContours(image,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
        image = cv::Mat::zeros(image.size(),CV_8UC1);
        for (int i=0;i<contours.size();i++){
            cv::drawContours(image,contours,i,cv::Scalar(255));
        }
        cv::imshow("contours",image);
        
        std::vector<cv::Vec2f> lines;
        //寻找直线 边界定位也可以用findContours实现
        HoughLines(image,lines,1,CV_PI/150,hough_thre,0.0001*hough_srn_thre,0.0001*hough_stn_thre);
        cv::Mat draw_points= cv::Mat::zeros(image.size(),CV_8UC1);
        // cv::cvtColor(image,draw_points,CV_GRAY2BGR);


        for(int i=0;i<lines.size();i++)  
        {
            float rho=lines[i][0];
            float theta=lines[i][1];
            Point pt1,pt2;
            double a=cos(theta),b=sin(theta);
            double x0=a*rho,y0=b*rho;
            pt1.x=cvRound(x0+1000*(-b));
            pt1.y=cvRound(y0+1000*a);
            pt2.x=cvRound(x0-1000*(-b));
            pt2.y=cvRound(y0-1000*a);
            line(draw_points,pt1,pt2,Scalar(255),1,CV_AA);
        }
        imshow("line",draw_points);

        Point2f ori_points[4];
        Point2f train_points[4];
        vector<Point2f>corners;
        goodFeaturesToTrack(draw_points,corners,4,0.1,corner_distance_thre,Mat()); //角点检测
        for(int i=0;i<corners.size();i++)
        {
            circle(draw_points,corners[i],3,Scalar(255),3);
            ori_points[i]=corners[i];
        }
        imshow("points",draw_points);

        // 排序后, 2,3为下边, 1,2为上边, 最下方的为3, 然后2, 1, 0
        std::sort(ori_points,ori_points+4,sort_method);
        // cout<<ori_points[2].y<<endl<<ori_points[3].y<<endl<<endl;
        // cv::line(imageSource,ori_points[0],ori_points[1],cv::Scalar(0,0,255));
        // cv::line(imageSource,ori_points[1],ori_points[2],cv::Scalar(0,255,0));
        // cv::line(imageSource,ori_points[2],ori_points[3],cv::Scalar(255,0,0));
        
        // cv::imshow("src",imageSource);

        double rect_lenght = get_distance(ori_points[2],ori_points[3]); 

        double real_lenght = 8.2; // 边长为8.2厘米
        double cms_per_pix = real_lenght/rect_lenght;
        cout<<cms_per_pix<<endl;

        double ori_tan = (ori_points[2].y - ori_points[3].y)
                            /(ori_points[2].x-ori_points[3].x);   // atan()返回弧度制鸭
        double tan_3_1 = -1/ori_tan;
        double cos_3_1 = cos(atan(ori_tan));
        double sin_3_1 = sin(atan(ori_tan));

        train_points[2] = ori_points[2];
        train_points[3] = ori_points[3];

        train_points[1].x = ori_points[3].x + rect_lenght*sin_3_1;
        train_points[1].y = ori_points[3].y - rect_lenght*cos_3_1;

        train_points[0].x = ori_points[2].x + rect_lenght*sin_3_1;
        train_points[0].y = ori_points[2].y - rect_lenght*cos_3_1;

        transf_element = cv::getPerspectiveTransform(ori_points,train_points);
        cv::warpPerspective(imageSource,imageSource,transf_element,imageSource.size());
        for (int i=0;i<4;i++) {
            cv::line(imageSource,train_points[i],train_points[(i+1)%4],cv::Scalar(0,0,50*i),2);
        }
        cv::imshow("fixed",imageSource);

        // debug print
        cout<<transf_element<<endl<<endl;
        // for (int i=0; i< transf_element.cols; i++) {
        //     for (int j=0; j<transf_element.rows; j++) {
        //         cout<<transf_element.at<double>(i,j)<<endl;
        //     }
        // }
        // cv::convertScaleAbs(transf_element,transf_element);
        // cv::resize(transf_element,transf_element,cv::Size(200,200));
        // cv::imshow("viewable",transf_element);
        // for (int i=0;i<4;i++) {
        //     cv::line(imageSource,train_points[i],train_points[(i+1)%4],cv::Scalar(0,0,50*i),2);
        // }
        // cv::imshow("train",imageSource);
        // cout<<'t'<<ori_tan<<endl;
        // cout<<train_points[1]<<endl;
        // cv::circle(image,train_points[1],3,255,2);
        // cv::imshow("points",image);
        // cout<<atan(ori_tan)<<' '<<atan(ori_tan)/CV_PI*180<<endl;
        char t_char = cv::waitKey(50);
        if (t_char == 's') {
            // write the element with std c++ fstream !!!!!!!!!!!!
            ofstream out_file("../../src/ini/777.txt");

            // ini cut off the double!!!!!
            // minIni* ini = new minIni("../../src/ini/data.ini");
            // stringstream t_ss;
            // int t_count = 0;
            for (int i=0; i< transf_element.cols; i++) {
                for (int j=0; j<transf_element.rows; j++) {
                    out_file<<fixed<<setprecision(16)<<transf_element.at<double>(i,j)<<endl;
                    // cout<<fixed<<setprecision(15)<<transf_element.at<double>(i,j)<<endl;
                    // t_ss<<t_count++;
                    // ini->put("transf_element","element"+t_ss.str(),transf_element.at<double>(i,j));
                    // t_ss.str("");
                    // t_ss.clear();
                }
            }
            out_file<<cms_per_pix;
            out_file.close();
            cv::destroyAllWindows();
            break;
        }
        else if (t_char == 'q'){
            cv::destroyAllWindows();
            break;
        }
    }

    cout<<"demarcate the forward direction..."<<endl;
    cv::namedWindow("fixed_src");
    cv::setMouseCallback("fixed_src", on_mouse, 0);
    while (1) {
        cp>>image;
        if (image.empty()) {
            cout<<"waitint for camera init..."<<endl;
            continue;
        }  
        cv::warpPerspective(image,image,transf_element,image.size()); 
        image.copyTo(imageSource);// 供回调函数画画使用
        // cv::imshow("fixed_src",image);
        char t_char = cv::waitKey(1);
        if (t_char == 'q') {
            cv::destroyAllWindows();
            break;
        }
        else if (t_char == 's') {
            cv::destroyAllWindows();
            // do sth to save the data.
            break;
        }
    }
	return 0;  
}

void on_trackerbar(int,void*) {
    ;
    return ;
}

bool sort_method(cv::Point2f p1, cv::Point2f p2) {
    return p1.y<p2.y;
}
double get_distance(cv::Point2f p1, cv::Point2f p2) {
    return sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );
}

static void on_mouse(int event, int x, int y, int flags, void *) {
    if (x<0 || x>=image.cols || y<0 || y>= image.rows) { //如果在窗口外..什么都不干
        return ;
    }
    if (event == CV_EVENT_LBUTTONDOWN) { //左键点两次定位
        switch (forward_count) {
        case 0:
            forward_points[forward_count] = cv::Point2i(x,y);
            forward_count++;
            cv::circle(image,cv::Point2i(x,y),2,cv::Scalar(0,0,255),2);
            break;
        case 1:
            forward_points[forward_count] = cv::Point2i(x,y);
            forward_count++;
            break;
        case 2:
            cout<<forward_points[0]<<endl<<forward_points[1]<<endl<<endl;
            cv::line(image,forward_points[0],forward_points[1],cv::Scalar(255,0,0),2);
            // write data
            ofstream out_file("../../src/ini/666.txt");
            out_file<<forward_points[0].x<<endl<<forward_points[0].y<<endl<<forward_points[1].x<<endl<<forward_points[1].y;
            out_file.close();
            break;
        }
        cv::imshow("fixed_src",image);
    }
    else if (event == CV_EVENT_RBUTTONDOWN) { // 点一次右键复位
        cv::imshow("fixed_src",imageSource);
        forward_count = 0;
    }
}
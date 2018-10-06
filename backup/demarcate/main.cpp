#include <algorithm>
#include <iostream>  
#include <opencv2/opencv.hpp>    
 
using namespace std;      
using namespace cv;

const int max_thre = 255;
int binary_thre = 100;
int hough_thre = 93;
int hough_stn_thre = 0;
int hough_srn_thre = 0;
int corner_distance_thre = 90;

void on_trackerbar(int,void*);
bool sort_method(cv::Point2f,cv::Point2f);
double get_distance(cv::Point2f,cv::Point2f);

int main(int argc,char*argv[])    
{  
    cv::Mat imageSource;
    cv::Mat image;
    cv::VideoCapture cp(0);
    while (1) {
        cv::namedWindow("src");
        cv::createTrackbar("binary_thre","binary",&binary_thre,max_thre,on_trackerbar);
        cv::createTrackbar("hough_thre","line",&hough_thre,max_thre,on_trackerbar);
        cv::createTrackbar("hough_stn_thre","line",&hough_stn_thre,100,on_trackerbar);
        cv::createTrackbar("hough_srn_thre","line",&hough_srn_thre,100,on_trackerbar);
        cv::createTrackbar("corner_distance_thre","points",&corner_distance_thre,max_thre,on_trackerbar);
        cp>>image;
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


        // int width=ori_points[1].x-ori_points[0].x;
        // int hight=ori_points[2].y-ori_points[0].y;
        // train_points[0]=ori_points[0];
        // train_points[1]=Point2f(train_points[0].x+width,train_points[0].y);
        // train_points[2]=Point2f(train_points[0].x,train_points[1].y+hight);
        // train_points[3]=Point2f(train_points[1].x,train_points[2].y);
        // Mat elementTransf;
        // elementTransf=	getAffineTransform(ori_points,train_points);
        // cout<<elementTransf<<endl;
        // warpAffine(imageSource,imageSource,elementTransf,imageSource.size(),1,0,Scalar(255));
        // imshow("fixed",imageSource);	
        
        waitKey(1);   
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

}
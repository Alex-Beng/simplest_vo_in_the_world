
//--------------------------------------������˵����-------------------------------------------
//		����˵������OpenCV3������š�OpenCV3���鱾����ʾ������09
//		��������������OpenCV��װĿ¼��Samples�ļ����еĹٷ�ʾ������-���ù����������˶�Ŀ����
//		�������ò���ϵͳ�� Windows 7 64bit
//		��������IDE�汾��Visual Studio 2010
//		��������OpenCV�汾��	3.0 beta
//		2014��11�� Revised by @ǳī_ë����
//------------------------------------------------------------------------------------------------


/************************************************************************
* Copyright(c) 2011  Yang Xian
* All rights reserved.
*
* File:	opticalFlow.cpp
* Brief: lk���������˶�Ŀ����
* Version: 1.0
* Author: Yang Xian
* Email: xyang2011@sinano.ac.cn
* Date:	2011/11/18
* History:
************************************************************************/


//---------------------------------��ͷ�ļ��������ռ�������֡�----------------------------
//		����������������ʹ�õ�ͷ�ļ��������ռ�
//-------------------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;





//-----------------------------------��ȫ�ֺ���������-----------------------------------------
//		����������ȫ�ֺ���
//-------------------------------------------------------------------------------------------------
void tracking(Mat &frame, Mat &output);
bool addNewPoints();
bool acceptTrackedPoint(int i);

//-----------------------------------��ȫ�ֱ���������-----------------------------------------
//		����������ȫ�ֱ���
//-------------------------------------------------------------------------------------------------
string window_name = "optical flow tracking";
Mat gray;	// ��ǰͼƬ
Mat gray_prev;	// Ԥ��ͼƬ
Mat transf_element = cv::Mat(3,3,CV_64FC1);
vector<Point2f> points[2];	// point0Ϊ�������ԭ��λ�ã�point1Ϊ���������λ��
vector<Point2f> initial;	// ��ʼ�����ٵ��λ��
vector<Point2f> features;	// ��������
int maxCount = 500;	// �������������
double qLevel = 0.01;	// �������ĵȼ�
double minDist = 10.0;	// ��������֮�����С����
vector<uchar> status;	// ����������״̬��������������Ϊ1������Ϊ0
vector<float> err;


//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-------------------------------------------------------------------------------------------------
int main()
{
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

	VideoCapture capture(0);

	if(capture.isOpened())	// ����ͷ��ȡ�ļ�����
	{
		while(true)
		{
			capture >> frame;
			cv::warpPerspective(frame,frame,transf_element,frame.size());
			cv::flip(frame,frame,-1);
			if(!frame.empty())
			{ 
				tracking(frame, result);
			}
			else
			{ 
				printf(" --(!) No captured frame -- Break!");
				break;
			}

			int c = waitKey(1);
			if( (char)c == 27 )
			{
				break; 
			} 
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------
// function: tracking
// brief: ����
// parameter: frame	�������Ƶ֡
//			  output �и��ٽ������Ƶ֡
// return: void
//-------------------------------------------------------------------------------------------------
void tracking(Mat &frame, Mat &output)
{

	//�˾�����OpenCV3��Ϊ��
	// cvtColor(frame, gray, COLOR_BGR2GRAY);
	//�˾�����OpenCV2��Ϊ��
	// cv::Mat t_mat, t_channels[3];
	// cvtColor(frame, t_mat, CV_BGR2HLS);
	// cv::split(t_mat,t_channels);
	// gray = t_channels[1];
	cvtColor(frame, gray, CV_BGR2GRAY);

	frame.copyTo(output);

	// ���������
	if (addNewPoints())
	{
		goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
		points[0].insert(points[0].end(), features.begin(), features.end());
		initial.insert(initial.end(), features.begin(), features.end());
	}

	if (gray_prev.empty())
	{
		gray.copyTo(gray_prev);
	}
	// l-k�������˶�����
	calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], status, err);
	// ȥ��һЩ���õ�������
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
	// ��ʾ��������˶��켣
	for (size_t i=0; i<points[1].size(); i++)
	{
		line(output, initial[i], points[1][i], Scalar(0, 0, 255));
		circle(output, points[1][i], 3, Scalar(0, 255, 0), -1);
	}

	// �ѵ�ǰ���ٽ����Ϊ��һ�˲ο�
	swap(points[1], points[0]);
	swap(gray_prev, gray);

	imshow(window_name, output);
}

//-------------------------------------------------------------------------------------------------
// function: addNewPoints
// brief: ����µ��Ƿ�Ӧ�ñ����
// parameter:
// return: �Ƿ���ӱ�־
//-------------------------------------------------------------------------------------------------
bool addNewPoints()
{
	return points[0].size() <= 10;
}

//-------------------------------------------------------------------------------------------------
// function: acceptTrackedPoint
// brief: ������Щ���ٵ㱻����
// parameter:
// return:
//-------------------------------------------------------------------------------------------------
bool acceptTrackedPoint(int i)
{
	return status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
}



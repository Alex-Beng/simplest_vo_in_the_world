#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
/**
 * Author: Alex Beng
 * Date:   2018-9-22 ~ 2018-10-??..
 * Version: xx.xx
*/

#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

/**
 * for seting parameter
*/
struct AllParameter {
    int channel_thre_min_;
    int channel_thre_max_;
};

/**
 * Parameters
 * -----------x-→
 * |
 * |
 * |
 * y          ___
 * ↓          |b|
 * current_position: the x is the pic's x, the y is the pic's y;
*/
class ImgProcResult {
public:
    cv::Point2f current_position;
};

/**
 * ***Functions***
 * ImageProcess:   the overall api for calling
 * Pretreat:       perspective trans 
 *                 & L channel threshold (need param setting)
 *                 return the thresholded L channel;
 * MotionEstimate: use lk method to estimate the motion.
 * LoadParam:      load the transf_element & forward_direction.
 * 
 * ***Parameter***
 * transf_element_:    the perspective transform element.
 * forward_direction_: the fixed forward direction.
 * raw_image_:         raw input image.
 * used_channel:       the proc-ed used channel;
 * 
 * channel_thre_min_:  thre min value.
 * channel_thre_max_:  thre max value.
*/
class ImgProc {
public:
    ImgProc();
public:
    void ImageProcess (cv::Mat, ImgProcResult*);
    cv::Mat Pretreat (cv::Mat);
    cv::Point2f MotionEstimate(cv::Mat);
    void LoadParam();
    void SetParam(AllParameter);
    //Parameters
    cv::Mat transf_element_;
    cv::Point2f forward_direction_;

    cv::Mat raw_image_;
    cv::Mat used_channel_;

    int channel_thre_min_;
    int channel_thre_max_;
};

#endif //IMAGE_PROCESS_H
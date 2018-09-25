#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;

/**
 * Parameters
 * ver_*  for vertical line
 * hori_* for horizontal line
 * 
 * ver_slope_: the slope angle's tan(+/-)
 * ver_bias_:  the bias between ver-line and middle
 * 
 * hori_pass_line_: whether pass a horizontal line,
 *                  if true,  pass a line, journey += a grid's length
 *                  if false, passing a line, is in a grid
*/
class ImgProcResult
{
    double ver_slope_;
    double ver_bias_;
    bool hori_pass_line_;
};

/**
 * ***Functions***
 * ImageProcess: the overall api for calling
 * Pretreat:     perspective trans 
 *              & L channel threshold 
 *              & hori- and ver- projection
 * GetVerInfo:   get the ver-line slope and bias
 * GetHoriInfo:  judge whether pass a grid
 * JudgeResult:  judge the return result
 * 
 * ***Parameter***
*/
class ImgProc
{
    void ImageProcess( cv::Mat, ImgProcResult* );
    void Pretreat();
    void GetVerInfo();
    void GetHoriInfo();
    bool JudgeResult();

    
};

#endif //IMAGE_PROCESS_H
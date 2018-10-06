#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
/**
 * Author: Alex Beng
 * Date:   2018-9-22 ~ 2018-9-??
 * Version: xx.xx
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include "./ini/minIni.h"
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
public:
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
 * raw_image_:         raw input image
 * proc_without_hori_: throw the horizontal line
 * proc_without_ver_:  throw the vertical line
 * ini_reader:         read the parameter in the .ini file
*/
class ImgProc
{
public:
    ImgProc();
public:
    void ImageProcess ( cv::Mat, ImgProcResult* );
    void Pretreat ();
    void GetVerInfo ();
    void GetHoriInfo ();
    bool JudgeResult ();

    //Parameters
    cv::Mat raw_image_;
    cv::Mat proc_without_hori_;
    cv::Mat proc_without_ver_;
    minIni* ini_reader_;
};

#endif //IMAGE_PROCESS_H
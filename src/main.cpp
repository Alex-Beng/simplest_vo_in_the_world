#include "image_process.h"

int main()
{
    cv::VideoCapture camera(0);
    cv::Mat frame;
    ImgProc proc_header;
    ImgProcResult proc_result;

    cv::namedWindow("raw_input");
    while (1){
        camera>>frame;
        if (frame.empty()){
            break;
        }
        cv::imshow("raw_input",frame);
        char t_char = cv::waitKey(1);
        if (t_char == 'q') {
            break;
        }
    }
    return 0;
}
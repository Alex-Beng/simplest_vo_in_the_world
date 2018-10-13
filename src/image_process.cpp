#include "image_process.h"

ImgProc::ImgProc(){
    LoadParam();
}

void ImgProc::ImageProcess(cv::Mat src_image, ImgProcResult* result_header) {
    used_channel_ = Pretreat(src_image);
    result_header->current_position = MotionEstimate(used_channel_);
}

cv::Mat ImgProc::Pretreat(cv::Mat src_image) {
    // perspective transf
    cv::warpPerspective(src_image,src_image,transf_element_,src_image.size());
    cv::flip(src_image,src_image,-1);

    // threshold
    cv::Mat t_image;
    cv::Mat t_channels[3];
    cv::cvtColor(src_image,t_image,CV_BGR2HLS);
    cv::split(t_image,t_channels);

    t_image = t_channels[1] > channel_thre_min_
            & t_channels[1] < channel_thre_max_;
    return t_image.clone();
}

cv::Point2f ImgProc::MotionEstimate(cv::Mat channel) {
    
}

void ImgProc::LoadParam() {
    // load the transform element.
    transf_element_ = cv::Mat(3,3,CV_64FC1);
    std::ifstream in_file("./ini/777.txt");
    if (!in_file) {
        cerr<<"error in Load element"<<endl;
    }
    double t;
    for (int i=0; i< transf_element_.cols; i++) {
        for (int j=0; j<transf_element_.rows; j++) {
            double t;
            in_file>>t;
            transf_element_.at<double>(i,j) = t;
        }
    }
    in_file.close();
    cout<<transf_element_<<endl;

    // load the forward direction.
    in_file.open("./ini/666.txt");
    if (!in_file) {
        cerr<<"error in Load direction"<<endl;
    }
    in_file>>forward_direction_.x>>forward_direction_.y;
    cout<<forward_direction_<<endl;
}

void ImgProc::SetParam(AllParameter ap) {
    channel_thre_min_ = ap.channel_thre_min_;
    channel_thre_max_ = ap.channel_thre_max_;
}
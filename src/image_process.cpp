#include "image_process.h"

ImgProc::ImgProc(){
    this->ini_reader_ = new minIni("./ini/data.ini");
    
}
# g++ main.cpp ../../src/ini/minIni.c -o demo -I C:\\OpenCv2\\include -L C:\\OpenCv2\\bin -llibopencv_core244 -llibopencv_highgui244 -llibopencv_contrib244 -llibopencv_features2d244 -llibopencv_flann244 -llibopencv_gpu244 -llibopencv_imgproc244 -llibopencv_legacy244 -llibopencv_ml244 -llibopencv_nonfree244 -llibopencv_objdetect244 
# g++ main.cpp ../../src/ini/minIni.c `pkg-config --cflags --libs opencv` -o demo 
g++ main.cpp ../../src/ini/minIni.c -o demo -IC:\\opencv\\build\\include -LC:\\opencv\\build\\bin -llibopencv_calib3d249 -llibopencv_contrib249 -llibopencv_core249 -llibopencv_features2d249 -llibopencv_flann249 -llibopencv_gpu249 -llibopencv_highgui249 -llibopencv_imgproc249 -llibopencv_legacy249 -llibopencv_ml249 -llibopencv_nonfree249 -llibopencv_objdetect249 -llibopencv_ocl249 -llibopencv_photo249 -llibopencv_stitching249 -llibopencv_superres249 -llibopencv_video249 -llibopencv_videostab249
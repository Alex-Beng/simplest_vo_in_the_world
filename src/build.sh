g++ main.cpp image_process.h ./ini/minIni.h `pkg-config --cflags --libs opencv` -o demo
rm demo
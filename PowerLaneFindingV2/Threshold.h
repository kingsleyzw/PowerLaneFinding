#ifndef THRESHOLD_H
#define THRESHOLD_H

#define d_sobel_kernel_size   3
#define MIN_SOBEL_X_THRESH    100
#define MAX_SOBEL_X_THRESH    255
#define MIN_SOBEL_Y_THRESH    100
#define MAX_SOBEL_Y_THRESH    255
#define MIN_SOBEL_MAG_THRESH  200
#define MAX_SOBEL_MAG_THRESH  255
#define MIN_SOBEL_DIR_THRESH  0.7
#define MAX_SOBEL_DIR_THRESH  1.3 
#define MIN_BGR_R_THRESH      220
#define MAX_BGR_R_THRESH      255
#define MIN_HLS_S_THRESH      175
#define MAX_HLS_S_THRESH      255
#define MIN_YUV_U_THRESH      20
#define MAX_YUV_U_THRESH      100

#include <opencv2/opencv.hpp>
using namespace cv;

class Threshold {
public:
	// constructor
	Threshold();
	Threshold(int sobel_kernel_size);

	// several threshold methods
	// Including 
	// 1. sobel x y
	// 2. sobel magnitude
	// 3. sobel direction
	// 4. hls s channel 
	Mat abs_sobel_thresh(char orient = 'x', int thresh_min = -1, int thresh_max = 255);
	Mat mag_thresh(int thresh_min = -1, int thresh_max = 255);
	Mat dir_thresh(double thresh_min = -1, double thresh_max = CV_PI / 2);
	Mat rgb_thresh(int thresh_min = -1, int thresh_max = 255);
	Mat hls_thresh(int thresh_min = -1, int thresh_max = 255);
	Mat yuv_thresh(int thresh_min = -1, int thresh_max = 255);

	// combine multiple threshold methods
	Mat combine_thresh(Mat src, int type);

private:
	int _sobel_kernel_size;
	Mat _sobel_x, _sobel_y, _s_channel, _r_channel, _u_channel;

	// process source image to gray and s-channel
	void source_image_process(Mat src);

	// process threshold to source image
	Mat threshold_process(Mat src, double thresh_min, double thresh_max, bool scale = true);
};


#endif // !THRESHOLD_H


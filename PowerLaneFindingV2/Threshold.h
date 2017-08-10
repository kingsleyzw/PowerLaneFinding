#ifndef THRESHOLD_H
#define THRESHOLD_H

#define d_sobel_kernel_size   3
#define MIN_SOBEL_X_THRESH    60
#define MAX_SOBEL_X_THRESH    255
#define MIN_SOBEL_Y_THRESH    20
#define MAX_SOBEL_Y_THRESH    255
#define MIN_SOBEL_MAG_THRESH  30
#define MAX_SOBEL_MAG_THRESH  255
#define MIN_SOBEL_DIR_THRESH  0
#define MAX_SOBEL_DIR_THRESH  0.4
#define MIN_BGR_R_THRESH      140
#define MAX_BGR_R_THRESH      255
#define MIN_HLS_S_THRESH      80
#define MAX_HLS_S_THRESH      255
#define MIN_YUV_U_THRESH      60
#define MAX_YUV_U_THRESH      125
#define MIN_LAP_THRESH        30
#define MAX_LAP_THRESH        200
#define MIN_CAN_THRESH        50
#define MAX_CAN_THRESH        150
#define ENTROPY_THRESHOLD     3.6

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
	Mat lap_thresh(int thresh_min = -1, int thresh_max = 255);
	Mat can_thresh(int thresh_min = -1, int thresh_max = 255);

	// combine multiple threshold methods
	Mat combine_thresh(Mat src);

private:
	int _sobel_kernel_size;
	Mat _sobel_x, _sobel_y, _s_channel, _r_channel, _u_channel, _clahe_gray, _gray;
	int _type;
	Ptr<CLAHE> _clahe;

	// process source image to gray and s-channel
	void source_image_process(Mat src);

	// process threshold to source image
	Mat threshold_process(Mat src, double thresh_min, double thresh_max, bool scale = true);

	// calculate entrophy of source picture
	void entrophy_cal();
};


#endif // !THRESHOLD_H


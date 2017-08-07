#include "Threshold.h"
#include "Parameter.h"
#include "time.h"
#include <vector>
#include <queue>
#include <fstream>
#include <future>
using namespace std;

Threshold::Threshold() {
	_sobel_kernel_size = d_sobel_kernel_size;
}

Threshold::Threshold(int sobel_kernel_size) {
	_sobel_kernel_size = sobel_kernel_size;
}

Mat Threshold::abs_sobel_thresh(char orient, int thresh_min, int thresh_max) {
	if (orient == 'y') 
		return threshold_process(_sobel_y, thresh_min, thresh_max);
	else // orient == 'x' 
		return threshold_process(_sobel_x, thresh_min, thresh_max);
}

Mat Threshold::mag_thresh(int thresh_min, int thresh_max) {
	Mat grad_mag;
	magnitude(_sobel_x, _sobel_y, grad_mag);
	return threshold_process(grad_mag, thresh_min, thresh_max);
}

Mat Threshold::dir_thresh(double thresh_min, double thresh_max) {
	Mat abs_grad_x, abs_grad_y;
	Mat grad_dir;
	abs_grad_x = abs(_sobel_x);
	abs_grad_y = abs(_sobel_y);
	phase(abs_grad_x, abs_grad_y, grad_dir);
	return threshold_process(grad_dir, thresh_min, thresh_max, false);
}

Mat Threshold::rgb_thresh(int thresh_min, int thresh_max) {
	return threshold_process(_r_channel, thresh_min, thresh_max, false);
}

Mat Threshold::hls_thresh(int thresh_min, int thresh_max) {
	return threshold_process(_s_channel, thresh_min, thresh_max, false);
}

Mat Threshold::yuv_thresh(int thresh_min, int thresh_max) {
	return threshold_process(_u_channel, thresh_min, thresh_max, false);
}

Mat Threshold::combine_thresh(Mat src, int type) {
	clock_t tStart = clock();
	Mat grad_x, grad_y, grad_mag, grad_dir;
	Mat grad_hls, grad_bgr, grad_yuv;
	Mat grad_xy, grad_magdir, grad_maghls;
	Mat dst, col, grad;
	queue<future<Mat>> f;

	source_image_process(src);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	/*if (type & SOBEL_X) grad_x = abs_sobel_thresh('x', MIN_SOBEL_X_THRESH, MAX_SOBEL_X_THRESH);
	if (type & SOBEL_Y) grad_y = abs_sobel_thresh('y', MIN_SOBEL_Y_THRESH, MAX_SOBEL_Y_THRESH);
	if (type & SOBEL_MAG) grad_mag = mag_thresh(MIN_SOBEL_MAG_THRESH, MAX_SOBEL_MAG_THRESH);
	if (type & SOBEL_DIR) grad_dir = dir_thresh(MIN_SOBEL_DIR_THRESH, MAX_SOBEL_DIR_THRESH);
	if (type & HLS_S) grad_hls = hls_thresh(MIN_HLS_S_THRESH, MAX_HLS_S_THRESH);
	*/
	if (type & SOBEL_DIR) f.push(async(launch::async, &Threshold::dir_thresh, this, MIN_SOBEL_DIR_THRESH, MAX_SOBEL_DIR_THRESH));
	if (type & SOBEL_X) f.push(async(launch::async, &Threshold::abs_sobel_thresh, this, 'x', MIN_SOBEL_X_THRESH, MAX_SOBEL_X_THRESH));
	if (type & SOBEL_Y) f.push(async(launch::async, &Threshold::abs_sobel_thresh, this, 'y', MIN_SOBEL_Y_THRESH, MAX_SOBEL_Y_THRESH));
	if (type & SOBEL_MAG) f.push(async(launch::async, &Threshold::mag_thresh, this, MIN_SOBEL_MAG_THRESH, MAX_SOBEL_MAG_THRESH));
	if (type & HLS_S) f.push(async(launch::async, &Threshold::hls_thresh, this, MIN_HLS_S_THRESH, MAX_HLS_S_THRESH));
	if (type & BGR_R) f.push(async(launch::async, &Threshold::rgb_thresh, this, MIN_BGR_R_THRESH, MAX_BGR_R_THRESH));
	if (type & YUV_U) f.push(async(launch::async, &Threshold::yuv_thresh, this, MIN_YUV_U_THRESH, MAX_YUV_U_THRESH));

	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	if (type & SOBEL_DIR) {
		grad_dir = f.front().get();
		f.pop();
	}
	if (type & SOBEL_X) {
		grad_x = f.front().get();
		f.pop();
	}
	if (type & SOBEL_Y) {
		grad_y = f.front().get();
		f.pop();
	}
	if (type & SOBEL_MAG) {
		grad_mag = f.front().get();
		f.pop();
	}
	if (type & HLS_S) {
		grad_hls = f.front().get();
		f.pop();
	} 
	else grad_hls = Mat::zeros(Size(src.cols, src.rows), CV_8U);
	if (type & BGR_R) {
		grad_bgr = f.front().get();
		f.pop();
	}
	else grad_bgr = Mat::zeros(Size(src.cols, src.rows), CV_8U);
	if (type & YUV_U) {
		grad_yuv = f.front().get();
		f.pop();
	}
	else grad_yuv = Mat::zeros(Size(src.cols, src.rows), CV_8U);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	if ( (type & SOBEL_XY) == SOBEL_XY) bitwise_and(grad_x, grad_y, grad_xy);
	else if (type & SOBEL_X) grad_xy = grad_x;
	else if (type & SOBEL_Y) grad_xy = grad_y;
	else grad_xy = Mat::zeros(Size(src.cols, src.rows), CV_8U);
	
	if ( (type & SOBEL_MAGDIR) == SOBEL_MAGDIR) bitwise_and(grad_mag, grad_dir, grad_magdir);
	else if (type & SOBEL_MAG) grad_magdir = grad_mag;
	else if (type & SOBEL_DIR) grad_magdir = grad_dir;
	else grad_magdir = Mat::zeros(Size(src.cols, src.rows), CV_8U);
	//imshow("x", grad_x);
	//imshow("y", grad_y);
	//imshow("xy", grad_xy);
	//imshow("mag", grad_mag);
	//imshow("dir", grad_dir);
	//imshow("magdir", grad_magdir);
	//imshow("bgr", grad_bgr);
	//imshow("yuv", grad_yuv);
	//imshow("hls", grad_hls);
	//ofstream file1("th_s_ch.txt"); file1 << grad_hls; file1.close();
	bitwise_or(grad_xy, grad_magdir, grad);
	bitwise_or(grad_bgr, grad_hls, col);
	bitwise_or(col, grad_yuv, col);
	bitwise_or(grad, col, dst);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	return dst;
}

Mat Threshold::threshold_process(Mat src, double thresh_min, double thresh_max, bool scale) {
	Mat abs_src, dst;

	dst = Mat::zeros(src.rows, src.cols, CV_8U);
	if (scale) {
		convertScaleAbs(src, abs_src);
		for (int i = 0; i < abs_src.rows; i++) {
			for (int j = 0; j < abs_src.cols; j++) {
				int v = abs_src.at<uchar>(i, j);
				if (v >= thresh_min && v <= thresh_max)
					dst.at<uchar>(i, j) = 255;
			}
		}
	}
	else {
		src.convertTo(src, CV_8U);
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				int v = src.at<uchar>(i, j);
				if (v >= thresh_min && v <= thresh_max)
					dst.at<uchar>(i, j) = 255;
			}
		}
	}

	return dst;
}

void Threshold::source_image_process(Mat src) {
	Mat src_blur, gray, hls, yuv;
	vector<Mat> mv;

	// reduce noise
	GaussianBlur(src, src_blur, Size(3,3), 0, 0, BORDER_DEFAULT);
	
	// convert source image to GRAY-scale and HLS color space
	cvtColor(src_blur, gray, COLOR_BGR2GRAY);
	cvtColor(src, hls, COLOR_BGR2HLS);
	cvtColor(src, yuv, COLOR_BGR2YUV);

	// split R-Channel from BGR
	split(src, mv);
	_r_channel = mv[2];
	
	// split S-Channel from HLS
	mv.clear();
	split(hls, mv);
	_s_channel = mv[2];
	//ofstream file1("s_ch.txt"); file1 << _s_channel; file1.close();

	// split U-Channel from YUV
	mv.clear();
	split(yuv, mv);
	_u_channel = mv[1];

	// calculate sobel x and y
	//Scharr(gray, _sobel_x, CV_64F, 1, 0);
	Sobel(gray, _sobel_x, CV_64F, 1, 0, _sobel_kernel_size);
	//Scharr(gray, _sobel_y, CV_64F, 0, 1);
	Sobel(gray, _sobel_y, CV_64F, 0, 1, _sobel_kernel_size);
}
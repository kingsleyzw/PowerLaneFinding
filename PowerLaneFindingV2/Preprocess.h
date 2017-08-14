#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
using namespace std;

class Preprocess
{
public:
	Preprocess();
	cv::Mat read_resize(string name, int scale = 1);
	int preprocess();
	cv::Mat read_undistort_resize(string name, int scale = 1);
	cv::Mat brightness_adjust(cv::Mat src);
	cv::Mat brightness_and_contrast_auto(const cv::Mat &src, float clipHistPercent = 0);
private:
	// for camera undistort
	int _nx, _ny;
	vector<vector<cv::Point3f>> _object_points;
	vector<vector<cv::Point2f>> _image_points;
	cv::Mat _intrinsic;
	cv::Mat _distCoeffs;
	cv::Mat calibration(cv::Mat img);
};

#endif // !PREPROCESS_H


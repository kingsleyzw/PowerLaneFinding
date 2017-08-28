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
	void preprocess();
	cv::Mat read(string name, int param = 0);
	cv::Mat brightness_adjust(cv::Mat src);
	cv::Mat brightness_and_contrast_auto(const cv::Mat &src, float clipHistPercent = 0);
	cv::Mat normalize_intensity(cv::Mat img);
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


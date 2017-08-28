#include "Preprocess.h"
#include "Parameter.h"
#include "dirent.h"
#include "time.h"
#include <fstream>
#include <stdexcept>

using namespace cv;

Preprocess::Preprocess() {
	_nx = 9;
	_ny = 6;
}
Mat Preprocess::read(string name, int param) {
	Mat src = imread(name);
	if(param & UNDISTORT) 
		src = calibration(src);
	if(param & RESIZE) 
		resize(src, src, Size(src.cols / SCALE, src.rows / SCALE));

	return src;
}
Mat Preprocess::brightness_adjust(Mat src) {
	Mat hsv, result;
	cvtColor(src, hsv, CV_BGR2HSV);
	
	vector<Mat> channels;
	split(hsv, channels);
	
	/*equalizeHist(channels[2], channels[2]);
	merge(channels, hsv);*/
	//hsv += Scalar(0, 0, 70);

	//const int cols = hsv.cols;
	//const int step = hsv.channels();
	//const int rows = hsv.rows;
	//for (int y = 0; y < rows; y++) {
	//	unsigned char* p_row = hsv.ptr(y) + 2; //gets pointer to the first byte to be changed in this row, SELECTED_CHANNEL_NUMBER is 3 for alpha
	//	unsigned char* row_end = p_row + cols*step;
	//	for (; p_row != row_end; p_row += step) {
	//		*p_row = 200;
	//	}
	//}

	cvtColor(hsv, result, CV_HSV2BGR);
	
	Scalar avg = mean(channels[2]);
	cout << avg[0] << endl;

	return result;
}
Mat Preprocess::brightness_and_contrast_auto(const Mat &src, float clipHistPercent) {
	Mat dst(src.rows, src.cols, src.type());

	CV_Assert(clipHistPercent >= 0);
	CV_Assert((src.type() == CV_8UC1) || (src.type() == CV_8UC3) || (src.type() == CV_8UC4));

	int histSize = 256;
	float alpha, beta;
	double minGray = 0, maxGray = 0;

	//to calculate grayscale histogram
	Mat gray;
	if (src.type() == CV_8UC1) gray = src;
	else if (src.type() == CV_8UC3) cvtColor(src, gray, CV_BGR2GRAY);
	else if (src.type() == CV_8UC4) cvtColor(src, gray, CV_BGRA2GRAY);
	if (clipHistPercent == 0) {
		// keep full available range
		minMaxLoc(gray, &minGray, &maxGray);
	}
	else {
		Mat hist; //the grayscale histogram

		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true;
		bool accumulate = false;
		calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		// calculate cumulative distribution from the histogram
		vector<float> accumulator(histSize);
		accumulator[0] = hist.at<float>(0);
		for (int i = 1; i < histSize; i++) {
			accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
		}

		// locate points that cuts at required value
		float max = accumulator.back();
		clipHistPercent *= (max / 100.0); //make percent as absolute
		clipHistPercent /= 2.0; // left and right wings
								// locate left cut
		minGray = 0;
		while (accumulator[minGray] < clipHistPercent)
			minGray++;

		// locate right cut
		maxGray = histSize - 1;
		while (accumulator[maxGray] >= (max - clipHistPercent))
			maxGray--;
	}

	// current range
	float inputRange = maxGray - minGray;

	alpha = (histSize - 1) / inputRange;   // alpha expands current range to histsize range
	beta = -minGray * alpha;             // beta shifts current range so that minGray will go to 0

										 // Apply brightness and contrast normalization
										 // convertTo operates with saurate_cast
	src.convertTo(dst, -1, alpha, beta);

	// restore alpha channel from source 
	if (dst.type() == CV_8UC4) {
		int from_to[] = { 3, 3 };
		cv::mixChannels(&src, 4, &dst, 1, from_to, 1);
	}
	return dst;
}

void Preprocess::preprocess() {
	string filename1 = "intrinsic";
	string filename2 = "distCoeffs";
	ifstream ifile1((filename1 + ".yml").c_str());
	ifstream ifile2((filename2 + ".yml").c_str());
	if ((bool)ifile1 && (bool)ifile2) {
		FileStorage storage1(filename1 + ".yml", FileStorage::READ);
		FileStorage storage2(filename2 + ".yml", FileStorage::READ);
		storage1[filename1] >> _intrinsic;
		storage1.release();
		storage2[filename2] >> _distCoeffs;
		storage2.release();
		return;
	}

	Size patternSize(_nx, _ny);
	vector<Point3f> obj;
	vector<Point2f> corners;
	char name[100];

	for (int i = 0; i < _ny; i++) {
		for (int j = 0; j < _nx; j++) {
			obj.push_back(Point3f(i, j, 0.0f));
		}
	}

	// prepare object and image points

	string inputDirectory = "camera_cal";
	DIR *directory = opendir(inputDirectory.c_str());
	struct dirent *dirent = NULL;
	if (directory == NULL) {
		throw invalid_argument("Cannot open Input Folder");
	}
	while ((dirent = readdir(directory)) != NULL) {
		string fileName = inputDirectory + "\\" + string(dirent->d_name);
		Mat rawImage = imread(fileName.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		if (rawImage.data == NULL) {
			throw invalid_argument("Cannot open Image");
			continue;
		}
		bool patternfound = findChessboardCorners(rawImage, patternSize, corners);
		if (patternfound) {
			cornerSubPix(rawImage, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(rawImage, patternSize, Mat(corners), patternfound);
			_image_points.push_back(corners);
			_object_points.push_back(obj);
		}
	}
	closedir(directory);

	// prepare intrinsic and distCoeffs matrix

	_intrinsic = Mat(3, 3, CV_32FC1);
	vector<Mat> rvecs;
	vector<Mat> tvecs;
	_intrinsic.ptr<float>(0)[0] = 1;
	_intrinsic.ptr<float>(1)[1] = 1;
	calibrateCamera(_object_points, _image_points, Size(IMG_COL_SIZE, IMG_ROW_SIZE), _intrinsic, _distCoeffs, rvecs, tvecs);

	FileStorage storage1(filename1 + ".yml", FileStorage::WRITE);
	FileStorage storage2(filename2 + ".yml", FileStorage::WRITE);
	storage1 << filename1 << _intrinsic;
	storage1.release();
	storage2 << filename2 << _distCoeffs;
	storage2.release();

	return;
}

Mat Preprocess::normalize_intensity(Mat img) {
	Mat dst;
	vector<Mat> channels;

	split(img, channels);

	//ofstream file1("I.txt"); file1 << channels[1]; file1.close();
	//normalize(channels[1], channels[1], 255, 0, NORM_INF);
	double max;
	minMaxLoc(channels[1], NULL, &max, NULL, NULL);
	//cout << max << endl;
	for (int i = 0; i < channels[1].rows; i++) {
		for (int j = 0; j < channels[1].cols; j++) {
			if (channels[1].at<uchar>(i, j) > 30) channels[1].at<uchar>(i, j) = 255;
			else channels[1].at<uchar>(i, j) *= (255/max);
		}
	}
	dilate(channels[1], channels[1], Mat());
	merge(channels, dst);
	//imshow("channel 1", channels[1]);

	return dst;
}

Mat Preprocess::calibration(Mat img) {
	Mat processedImg;
	undistort(img, processedImg, _intrinsic, _distCoeffs);
	//imshow("Original", img);
	//imshow("Processed", processedImg);

	return processedImg;
}
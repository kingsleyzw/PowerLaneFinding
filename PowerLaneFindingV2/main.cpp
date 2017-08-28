#include <opencv2/opencv.hpp>
#include "Preprocess.h"
#include "Threshold.h"
#include "Parameter.h"
#include "Perspective.h"
#include "LaneDetection.h"
#include "time.h"
using namespace cv;

Mat draw_histogram(Mat src, int channel);

int main(void)
{
	clock_t tStart = clock();
	
	Mat bgr, di, wrp, adj, tsh, dst;
	Preprocess preprocess;
	Threshold threshold;
	Perspective perspective;
	LaneDetection laneDetection;

	//VideoCapture cap("video/video3.mp4");
	//if (!cap.isOpened()) {
	//	cout << "!!! Failed to open file: " << endl;
	//	return -1;
	//}
	//Mat frame;
	//int num = 0;
	//tStart = clock();
	//for (;;) {
	//	if (!cap.read(frame))
	//		break;
	//	resize(frame, frame, Size(frame.cols / SCALE, frame.rows / SCALE));
	//	wrp = perspective.warp(frame);
	//	//Mat result = preprocess.brightness_adjust(wrp);
	//	tsh = threshold.combine_thresh(wrp);
	//	dst = laneDetection.finding_lane_line(tsh);
	//	/*imshow("frame", frame);
	//	imshow("wrp", wrp);
	//	imshow("his", result);
	//	imshow("tsh", tsh);
	//	imshow("window", dst);*/
	//	/*if (((double)(clock() - tStart) / CLOCKS_PER_SEC) > 1) {
	//		imwrite("video4/pic" + to_string(num)+".jpg", frame);
	//		num++;
	//		tStart = clock();
	//	}*/
	//	char key = waitKey(10);
	//	if (key == 27) // ESC
	//		break;
	//}

	//preprocess.preprocess();
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	bgr = preprocess.read("fusion2/897RGB.bmp", RESIZE);
	int DI_num = 7;
	int a = 897;
	char fileName[50];
	di = Mat::zeros(bgr.size(), bgr.type());
	for (int i = 0; i < DI_num; i++) {
		sprintf(fileName, "fusion2/%dDI.bmp", a-i);
		Mat tmp = preprocess.read(fileName, RESIZE);
		addWeighted(tmp, 1, di, 1, 0.0, di);
	}
	di = preprocess.normalize_intensity(di);
	//adj = preprocess.brightness_adjust(src);
	//adj = preprocess.brightness_and_contrast_auto(src);
	wrp = perspective.warp(bgr);
	tsh = threshold.combine_thresh(wrp);
	Mat di_wrp = perspective.warp(di);
	Mat di_hist = draw_histogram(di_wrp, 1);
	//imshow("di_wrp", di_wrp);
	//imshow("di_hist", di_hist);
	dst = laneDetection.finding_lane_line(tsh, di_hist);

	//imshow("bgr", bgr);
	//imshow("di", di);	
	//imshow("adj", adj);
	//imshow("wrp", wrp);
	//imshow("tsh", tsh);
	imshow("dst", dst);
	waitKey();

	/*imwrite("src.jpg", src);
	imwrite("wrp.jpg", wrp);
	imwrite("tsh.jpg", tsh);*/
	//imwrite("presentation-pics/lidar-fail, camera succeed/897-fusion.bmp", dst);
	
	//while (1);

	return 0;
}

Mat draw_histogram(Mat src, int channel) {
	vector<Mat> channels;
	split(src, channels);
	
	Mat sum;
	//Mat sub_channel = channels[channel](Rect(Point(0, src.rows*0.8), Point(src.cols-1, src.rows-1)));
	reduce(channels[channel], sum, 0, CV_REDUCE_AVG, CV_32FC1);
	normalize(sum, sum, 255, 0, NORM_INF);
	Mat histogram(Mat::zeros(256, sum.cols, CV_8UC1));
	for (int i = 0; i < sum.cols; i++) {
		Point pt1(i, 255);
		Point pt2(i, 255 - sum.at<float>(0, i));
		line(histogram, pt1, pt2, Scalar(255));
	}
	
	return histogram;
}
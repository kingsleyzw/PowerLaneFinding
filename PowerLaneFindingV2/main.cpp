#include <opencv2/opencv.hpp>
#include "Preprocess.h"
#include "Threshold.h"
#include "Parameter.h"
#include "Perspective.h"
#include "LaneDetection.h"
#include "time.h"
using namespace cv;

int main(void)
{
	clock_t tStart = clock();
	
	Mat src, wrp, adj, tsh, dst;
	Preprocess preprocess;
	Threshold threshold;
	Perspective perspective;
	LaneDetection laneDetection;

	//VideoCapture cap("video/video4.mp4");
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
	//	Mat result = preprocess.brightness_adjust(wrp);
	//	tsh = threshold.combine_thresh(result);
	//	dst = laneDetection.finding_lane_line(tsh);
	//	imshow("frame", frame);
	//	imshow("wrp", wrp);
	//	imshow("his", result);
	//	imshow("tsh", tsh);
	//	imshow("window", dst);
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
	src = preprocess.read_resize("video4/pic6.jpg", SCALE);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	wrp = perspective.warp(src);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	adj = preprocess.brightness_adjust(wrp);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	tsh = threshold.combine_thresh(adj);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//dst = laneDetection.finding_lane_line(tsh);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	//imshow("src", src);
	imshow("wrp", wrp);
	imshow("adj", adj);
	//imshow("tsh", tsh);
	//imshow("dst", dst);
	waitKey();

	/*imwrite("src.jpg", src);
	imwrite("wrp.jpg", wrp);
	imwrite("tsh.jpg", tsh);
	imwrite("dst.jpg", dst);*/
	
	//while (1);

	return 0;
}
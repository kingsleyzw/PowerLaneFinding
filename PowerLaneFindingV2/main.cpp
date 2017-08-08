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
	
	Mat src, wrp, tsh, dst;
	Preprocess preprocess;
	Threshold threshold;
	Perspective perspective;
	LaneDetection laneDetection;

	//VideoCapture cap("video3.mp4");
	//if (!cap.isOpened()) {
	//	cout << "!!! Failed to open file: " << endl;
	//	return -1;
	//}
	//Mat frame;
	//int hsv_sum = 0;
	//int frame_num = 0;
	//int num = 0;
	//tStart = clock();
	//for (;;) {
	//	if (!cap.read(frame))
	//		break;
	//	//resize(frame, frame, Size(frame.cols / SCALE, frame.rows / SCALE));
	//	//Mat hsv;
	//	//std::vector<Mat> mv;
	//	//cvtColor(frame, hsv, COLOR_BGR2HSV);
	//	//split(hsv, mv);
	//	//hsv_num += mean(mv[2]);
	//	//tsh = threshold.combine_thresh(frame, SOBEL_X | SOBEL_Y | BGR_R | YUV_U);
	//	//tsh = threshold.combine_thresh(frame, SOBEL_X | SOBEL_Y | SOBEL_DIR | SOBEL_MAG | BGR_R | YUV_U);
	//	//wrp = perspective.warp(tsh);
	//	//dst = laneDetection.finding_lane_line(wrp);
	//	//imshow("window", frame);
	//	/*if (((double)(clock() - tStart) / CLOCKS_PER_SEC) > 1) {
	//		imwrite("pics/pic" + to_string(num)+".jpg", frame);
	//		num++;
	//		tStart = clock();
	//	}*/
	//	//char key = waitKey(10);
	//	//if (key == 27) // ESC
	//	//	break;
	//}

	//preprocess.preprocess();
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	src = preprocess.read_resize("pics/pic12.jpg", SCALE);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	wrp = perspective.warp(src);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//tsh = threshold.combine_thresh(wrp, SOBEL_X);
	tsh = threshold.combine_thresh(wrp);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	dst = laneDetection.finding_lane_line(tsh);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	//imshow("src", src);
	//imshow("wrp", wrp);
	//imshow("tsh", tsh);
	imshow("dst", dst);
	waitKey();

	/*imwrite("src.jpg", src);
	imwrite("wrp.jpg", wrp);
	imwrite("tsh.jpg", tsh);
	imwrite("dst.jpg", dst);*/
	
	while (1);

	return 0;
}
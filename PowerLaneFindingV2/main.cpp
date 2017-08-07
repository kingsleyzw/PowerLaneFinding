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

	VideoCapture cap("video5.mp4");
	if (!cap.isOpened()) {
		cout << "!!! Failed to open file: " << endl;
		return -1;
	}
	Mat frame;
	int hsv_sum = 0;
	int frame_num = 0;
	for (;;) {
		if (!cap.read(frame))
			break;
		resize(frame, frame, Size(frame.cols / SCALE, frame.rows / SCALE));
		/*Mat hsv;
		std::vector<Mat> mv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		split(hsv, mv);
		hsv_num += mean(mv[2]);*/

		//tsh = threshold.combine_thresh(frame, SOBEL_X | SOBEL_Y | BGR_R | YUV_U);
		tsh = threshold.combine_thresh(frame, SOBEL_X | SOBEL_Y | SOBEL_DIR | SOBEL_MAG | BGR_R | YUV_U);
		//wrp = perspective.warp(tsh);
		//dst = laneDetection.finding_lane_line(wrp);
		imshow("window", tsh);
		char key = waitKey(10);
		if (key == 27) // ESC
			break;
		//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	}

	//preprocess.preprocess();
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//src = preprocess.read_resize("test_images/signs_vehicles_xygrad.jpg", SCALE);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//tsh = threshold.combine_thresh(src, SOBEL_X | SOBEL_Y | BGR_R | YUV_U);
	//Mat tsh2 = threshold.combine_thresh(src, SOBEL_X | HLS_S | BGR_R | YUV_U);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//wrp = perspective.warp(tsh);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//dst = laneDetection.finding_lane_line(wrp);
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	//imshow("src", src);
    //imshow("tsh", tsh);
	//imshow("tsh2", tsh2);
	//imshow("wrp", wrp);
	//imshow("dst", dst);
	//waitKey();

	/*imwrite("src.jpg", src);
	imwrite("wrp.jpg", wrp);
	imwrite("tsh.jpg", tsh);
	imwrite("dst.jpg", dst);*/
	
	while (1);

	return 0;
}
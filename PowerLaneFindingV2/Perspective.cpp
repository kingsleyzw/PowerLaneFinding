#include "Perspective.h"
#include "Parameter.h"

Perspective::Perspective() {
	_src[0] = Point2f( (200. / 1280.*IMG_COL_SIZE)/SCALE, (720. / 720.*IMG_ROW_SIZE)/SCALE );
	_src[1] = Point2f( (1100. / 1280.*IMG_COL_SIZE)/SCALE, (720. / 720.*IMG_ROW_SIZE)/SCALE );
	_src[2] = Point2f( (595. / 1280.*IMG_COL_SIZE)/SCALE, (450. / 720.*IMG_ROW_SIZE)/SCALE );
	_src[3] = Point2f( (685. / 1280.*IMG_COL_SIZE)/SCALE, (450. / 720.*IMG_ROW_SIZE)/SCALE );

	_dst[0] = Point2f( (300. / 1280.*IMG_COL_SIZE)/SCALE, (720. / 720.*IMG_ROW_SIZE)/SCALE );
	_dst[1] = Point2f( (980. / 1280.*IMG_COL_SIZE)/SCALE, (720. / 720.*IMG_ROW_SIZE)/SCALE );
	_dst[2] = Point2f( (300. / 1280.*IMG_COL_SIZE)/SCALE, (0. / 720.*IMG_ROW_SIZE)/SCALE );
	_dst[3] = Point2f( (980. / 1280.*IMG_COL_SIZE)/SCALE, (0. / 720.*IMG_ROW_SIZE)/SCALE );

	/*_src[0] = Point2f( (120. / 1280.*IMG_COL_SIZE)/SCALE, (720. / 720.*IMG_ROW_SIZE)/SCALE );
	_src[1] = Point2f( (586. / 1280.*IMG_COL_SIZE)/SCALE, (450. / 720.*IMG_ROW_SIZE)/SCALE );
	_src[2] = Point2f( (694. / 1280.*IMG_COL_SIZE)/SCALE, (450. / 720.*IMG_ROW_SIZE)/SCALE );
	_src[3] = Point2f( (1160. / 1280.*IMG_COL_SIZE)/SCALE, (720. / 720.*IMG_ROW_SIZE)/SCALE );

	_dst[0] = Point2f( (120. / 1280.*IMG_COL_SIZE)/SCALE, (1280. / 720.*IMG_ROW_SIZE)/SCALE );
	_dst[1] = Point2f( (120. / 1280.*IMG_COL_SIZE)/SCALE, (0. / 720.*IMG_ROW_SIZE)/SCALE );
	_dst[2] = Point2f( (600. / 1280.*IMG_COL_SIZE)/SCALE, (0. / 720.*IMG_ROW_SIZE)/SCALE );
	_dst[3] = Point2f( (600. / 1280.*IMG_COL_SIZE)/SCALE, (1280. / 720.*IMG_ROW_SIZE)/SCALE );*/
	
	_M = getPerspectiveTransform(_src, _dst);
}

Mat Perspective::warp(Mat img) {
	Mat dst;

	warpPerspective(img, dst, _M, img.size());

	return dst;
}
#include "Perspective.h"
#include "Parameter.h"

Perspective::Perspective() {
	float src_offset = 20.;
	_src[0] = Point2f( (330.) /SCALE, 240. /SCALE );
	_src[1] = Point2f( (480.) /SCALE, 240. /SCALE );
	_src[2] = Point2f( (704.) /SCALE, 382. /SCALE );
	_src[3] = Point2f( (164.) /SCALE, 382. /SCALE );
	float offset = 100.;
	_dst[0] = Point2f( (0.  +offset) /SCALE, 0.   /SCALE );
	_dst[1] = Point2f( (854.-offset) /SCALE, 0.   /SCALE );
	_dst[2] = Point2f( (854.-offset) /SCALE, 480. /SCALE );
	_dst[3] = Point2f( (0.  +offset) /SCALE, 480. /SCALE );

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

	/*line(img, Point2f(361. / SCALE, 203. / SCALE), Point2f(436. / SCALE, 203. / SCALE), Scalar(255), 2);
	line(img, Point2f(436. / SCALE, 203. / SCALE), Point2f(704. / SCALE, 382. / SCALE), Scalar(255), 2);
	line(img, Point2f(704. / SCALE, 382. / SCALE), Point2f(164. / SCALE, 382. / SCALE), Scalar(255), 2);
	line(img, Point2f(164. / SCALE, 382. / SCALE), Point2f(361. / SCALE, 203. / SCALE), Scalar(255), 2);*/

	return dst;
}
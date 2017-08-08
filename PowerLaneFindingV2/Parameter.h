#ifndef PARAMETER_H
#define PARAMETER_H

#define SOBEL_X               (0b1<<0)
#define SOBEL_Y               (0b1<<1)
#define SOBEL_XY              (SOBEL_X|SOBEL_Y)
#define SOBEL_MAG             (0b1<<2)
#define SOBEL_DIR             (0b1<<3)
#define SOBEL_MAGDIR          (SOBEL_MAG|SOBEL_DIR)
#define HLS_S                 (0b1<<4)
#define BGR_R                 (0b1<<5)
#define YUV_U                 (0b1<<6)
#define LAP                   (0b1<<7)
#define CAN                   (0b1<<8)
#define IMG_COL_SIZE          854
#define IMG_ROW_SIZE          480
#define SCALE                 3

#endif // !PARAMETER_H

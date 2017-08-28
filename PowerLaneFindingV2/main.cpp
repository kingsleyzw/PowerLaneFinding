#include "Pipeline.h";
int main(void)
{
	Pipeline pipeline;

	//pipeline.camera_only("fusion2/897RGB.bmp");
	pipeline.fusion("fusion2", "897RGB.bmp", "897DI.bmp", 7);
	pipeline.show_img();

	return 0;
}
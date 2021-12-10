#include "camera_hal_ps5.h"

namespace example {

camera_hal_ps5::camera_hal_ps5(const int camera_device_number) :
		camera_hal_cv_base(camera_device_number) {
}

camera_hal_ps5::~camera_hal_ps5() {}

bool camera_hal_ps5::init(const unsigned int width, const unsigned int height, const double fps) {
	return camera_hal_cv_base::init(width * 2, height, fps);
}

bool camera_hal_ps5::read_frame(cv::Mat &left, cv::Mat &right) {
	if (!camera_hal_cv_base::read_frame()) {
		return false;
	}
	const unsigned int frame_width = frame.cols;
	right = frame.colRange(0, frame_width / 2);
	left = frame.colRange(frame_width / 2, frame_width);

	return true;
}

}


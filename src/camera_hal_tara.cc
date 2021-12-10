#include "camera_hal_tara.h"
#include "xunit_lib_tara.h"
#include "spdlog/spdlog.h"

#include <sys/ioctl.h>
#include <libv4l2.h>

namespace example {

const std::string DEV_FILE_PREFIX = "/dev/video";

camera_hal_tara::camera_hal_tara(const int camera_device_number) :
   camera_hal_cv_base(camera_device_number),
   dev_file(DEV_FILE_PREFIX + std::to_string(camera_device_number)) {
}

camera_hal_tara::~camera_hal_tara() {
}

bool camera_hal_tara::init(const unsigned int width, const unsigned int height, const double fps) {
	int dev_fd;

	if ((dev_fd = v4l2_open(dev_file.c_str(), O_RDWR | O_NONBLOCK, 0)) < 0) {
		return false;
	}

	if (ioctl(dev_fd, VIDIOC_QUERYCAP, &dev_cap) < 0) {
		return false;
	}

	struct v4l2_format dev_fmt = {};
	dev_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (ioctl(dev_fd, VIDIOC_G_FMT, &dev_fmt) < 0) {
		return false;
	}

	dev_fmt.fmt.pix.width = width;
	dev_fmt.fmt.pix.height = height;

	if (ioctl(dev_fd, VIDIOC_S_FMT, &dev_fmt) < 0) {
		return false;
	}

	if (v4l2_close(dev_fd) < 0) {
		return false;
	}

	if(!InitExtensionUnit((char *)dev_cap.bus_info)) {
		return false;
	}

	SetAutoExposureStereo();

	return camera_hal_cv_base::init(width, height, fps);
}


bool camera_hal_tara::read_frame(cv::Mat &left, cv::Mat &right) {
	if (!camera_hal_cv_base::read_frame()) {
		return false;
	}
	cv::Mat rgbchannel[3];
	cv::split(frame, rgbchannel);
	left = rgbchannel[1];
	right = rgbchannel[2];
	return true;
}

}


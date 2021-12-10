
#include <stdexcept>
#include "camera_hal.h"
#include "camera_hal_tara.h"
#include "camera_hal_ps5.h"
#include "spdlog/spdlog.h"

namespace example {

camera_hal_IF* create_hal(std::string camera_name,
		const int camera_device_number) {
	if (camera_name.find("SonyPS5") != std::string::npos) {
		return new camera_hal_ps5(camera_device_number);
	}
	if (camera_name.find("EConTara") != std::string::npos) {
		return new camera_hal_tara(camera_device_number);
	}
	throw std::runtime_error("Invalid camera_name: " + camera_name);
}

camera_hal::camera_hal(std::string camera_name, int camera_device_number) :
		pimpl(create_hal(camera_name, camera_device_number)) {
}

camera_hal::~camera_hal() {
}

bool camera_hal::init(const unsigned int width, const unsigned int height,
		const double fps) {
	return pimpl->init(width, height, fps);
}

bool camera_hal::read_frame(cv::Mat &left, cv::Mat &right) {
	return pimpl->read_frame(left, right);
}

camera_hal_cv_base::camera_hal_cv_base(const int camera_device_number) :
	camera_device_number(camera_device_number) {
}

camera_hal_cv_base::~camera_hal_cv_base() {}

bool camera_hal_cv_base::init(const unsigned int width, const unsigned int height, const double fps) {
	capture_device = cv::VideoCapture(camera_device_number);

	if (!capture_device.isOpened()) {
		spdlog::critical("Call to cv::VideoCapture() failed to open camera {}", camera_device_number);
		return false;
	}

	//Setting up FrameRate
	capture_device.set(cv::CAP_PROP_FPS, fps);

	//Setting width and height
	capture_device.set(cv::CAP_PROP_FRAME_WIDTH, width);
	capture_device.set(cv::CAP_PROP_FRAME_HEIGHT, height);

	return true;
}


bool camera_hal_cv_base::read_frame(cv::Mat &left, cv::Mat &right) {
	if (!read_frame()) {
		return false;
	}
	cv::Mat rgbchannel[3];
	cv::split(frame, rgbchannel);
	left = rgbchannel[1];
	right = rgbchannel[2];
	return true;
}

bool camera_hal_cv_base::read_frame() {
	return capture_device.read(frame) || frame.empty();
}

}


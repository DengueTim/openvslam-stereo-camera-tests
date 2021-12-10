#ifndef EXAMPLE_CAMERA_HAL_H_
#define EXAMPLE_CAMERA_HAL_H_

#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

namespace example {

class camera_hal_IF {
public:
	virtual ~camera_hal_IF() {};

	virtual bool init(const unsigned int width, const unsigned int height, const double fps) = 0;
	virtual bool read_frame(cv::Mat &left, cv::Mat &right) = 0;

	//Gets the exposure of the camera
//	virtual double get_exposure() = 0;
//
//	//Sets the exposure of the camera
//	virtual bool set_exposure(const double exposure) = 0;
//
//	//Sets the camera to auto exposure
//	virtual bool set_auto_exposure() = 0;
//
//	//Get the brightness from the camera
//	virtual double get_brightness() = 0;
//
//	//Sets the Brightness Val of the  camera
//	virtual bool set_brightness(const double brightness) = 0;
};

class camera_hal final : public camera_hal_IF {
public:
	camera_hal(const std::string camera_name, const int camera_device_number);
	~camera_hal();

	bool init(const unsigned int width, const unsigned int height, const double fps) override;

	bool read_frame(cv::Mat &left, cv::Mat &right) override;
private:
	const std::unique_ptr<camera_hal_IF> pimpl;
};

class camera_hal_cv_base : public camera_hal_IF {
public:
	camera_hal_cv_base(const int camera_device_number);

	virtual ~camera_hal_cv_base() ;

	virtual bool init(const unsigned int width, const unsigned int height, const double fps) override;
	virtual bool read_frame(cv::Mat &left, cv::Mat &right) = 0;

protected:
	cv::Mat frame;
	bool read_frame();
	cv::VideoCapture capture_device;
	const int camera_device_number;
};

}

#endif /* EXAMPLE_CAMERA_HAL_H_ */

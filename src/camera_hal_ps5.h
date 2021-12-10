#include "camera_hal.h"

namespace example {

class camera_hal_ps5 final : public camera_hal_cv_base {
public:
	camera_hal_ps5(const int camera_device_number);

	~camera_hal_ps5() ;

	bool init(const unsigned int width, const unsigned int height, const double fps) override;
	bool read_frame(cv::Mat &left, cv::Mat &right) override;
};

}


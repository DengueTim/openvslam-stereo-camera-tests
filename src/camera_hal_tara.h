#include "camera_hal.h"
#include <linux/videodev2.h>

namespace example {

class camera_hal_tara final : public camera_hal_cv_base {
public:
	camera_hal_tara(const int camera_device_number);

	~camera_hal_tara() ;

	bool init(const unsigned int width, const unsigned int height, const double fps) override;
	bool read_frame(cv::Mat &left, cv::Mat &right) override;

private:
	const std::string dev_file;
	struct v4l2_capability dev_cap;
};

}


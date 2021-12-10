cmake_minimum_required(VERSION 3.1)
project(stereo_camera LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

set(USE_PANGOLIN_VIEWER ON CACHE BOOL "Enable Pangolin Viewer")

find_package(Threads REQUIRED)
find_package(Eigen3 3.3 REQUIRED)
message(STATUS "Using Eigen ${Eigen3_VERSION}")

find_package(OpenCV 3.3 REQUIRED)
find_package(OpenMP REQUIRED)
find_package(openvslam REQUIRED)
message(STATUS "Found openvslam lib ${openvslam}")

find_library(UDEV NAMES libudev.so)
IF(UDEV)
	message(STATUS "Found UDEV lib ${UDEV}")
ELSE()
	message(FATAL_ERROR "LIB UDEV not found on your system. Try:\nsudo apt-get install libudev-dev")
ENDIF(UDEV)


find_library(V4L2 NAMES v4l2)
IF(V4L2)
	message(STATUS "Found v4l2 lib ${V4L2}")
ELSE()
	message(FATAL_ERROR "LIB v4l2 not found on your system...")
ENDIF(V4L2)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/3rdParty/tara-cpu-sdk/Source/common/xunit/libecon_xunit.so
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rdParty/tara-cpu-sdk/Source/common
	COMMAND make xunit
)

add_executable(stereo_camera src/stereo_camera.cc src/camera_hal.cc src/camera_hal_tara.cc src/camera_hal_ps5.cc)

target_compile_definitions(stereo_camera PRIVATE USE_PANGOLIN_VIEWER)

set_target_properties(stereo_camera PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

target_link_libraries(stereo_camera
	PRIVATE
	Eigen3::Eigen
	opencv_imgcodecs
	opencv_videoio
	openvslam::openvslam
	openvslam::pangolin_viewer
	${PROJECT_SOURCE_DIR}/3rdParty/tara-cpu-sdk/Source/common/xunit/libecon_xunit.so
	${UDEV}
	${V4L2})

target_include_directories(stereo_camera
	PRIVATE
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdParty/tara-cpu-sdk/Source/common/include>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdParty/popl/include>)

cmake_minimum_required(VERSION 2.8)
project( OpenCVTest )
find_package( OpenCV REQUIRED PATHS /usr/local/share/OpenCV NO_DEFAULT_PATH)
include_directories( include ${OpenCV_INCLUDE_DIRS} )

add_executable( canny src/videocapture.cpp src/canny/gpu_processing.cpp
			    src/canny/cpu_processing.cpp)

target_link_libraries( canny ${OpenCV_LIBS} )

add_executable( optical_flow src/videocapture.cpp src/optical_flow/gpu_processing.cpp
			    src/optical_flow/cpu_processing.cpp)

target_link_libraries( optical_flow ${OpenCV_LIBS} )

add_executable( orb src/videocapture.cpp src/orb/gpu_processing.cpp
			    src/orb/cpu_processing.cpp)

target_link_libraries( orb ${OpenCV_LIBS} )

#add_executable( lane_detector src/videocapture.cpp 
#			    src/lane_detector/LaneDetector.cpp 
#			    src/lane_detector/cpu_processing.cpp 
#			    src/lane_detector/gpu_processing.cpp)

#target_link_libraries( lane_detector ${OpenCV_LIBS} )

add_executable( dnn_object_detection src/dnn/object_detection.cpp)

target_link_libraries( dnn_object_detection ${OpenCV_LIBS} )
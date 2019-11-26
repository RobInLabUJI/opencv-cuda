cmake_minimum_required(VERSION 2.8)
project( OpenCVTest )
find_package( OpenCV REQUIRED )
include_directories( include ${OpenCV_INCLUDE_DIRS} )

#add_executable( canny src/videocapture.cpp 
#			    src/canny/cpu_processing.cpp 
#			    src/canny/gpu_processing.cpp)

#target_link_libraries( canny ${OpenCV_LIBS} )

#add_executable( stereo_bm src/stereocapture.cpp 
#			    src/stereo_bm/cpu_processing.cpp 
#			    src/stereo_bm/gpu_processing.cpp)

#target_link_libraries( stereo_bm ${OpenCV_LIBS} )

add_executable( optical_flow src/videocapture.cpp 
			    src/optical_flow/cpu_processing.cpp 
			    src/optical_flow/gpu_processing.cpp)

target_link_libraries( optical_flow ${OpenCV_LIBS} )

#add_executable( lane_detector src/videocapture.cpp 
#			    src/lane_detector/LaneDetector.cpp 
#			    src/lane_detector/cpu_processing.cpp 
#			    src/lane_detector/gpu_processing.cpp)

#target_link_libraries( lane_detector ${OpenCV_LIBS} )


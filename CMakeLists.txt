cmake_minimum_required(VERSION 3.5.1)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

project( OpenCVTest )
#find_package( OpenCV REQUIRED PATHS /usr/local/share/OpenCV NO_DEFAULT_PATH)
find_package( OpenCV REQUIRED )
include_directories( include ${OpenCV_INCLUDE_DIRS} )

add_executable( canny src/videocapture.cpp src/canny/gpu_processing.cpp
			    src/canny/cpu_processing.cpp)

target_link_libraries( canny ${OpenCV_LIBS} )

#add_executable( optical_flow src/videocapture.cpp src/optical_flow/gpu_processing.cpp
#			    src/optical_flow/cpu_processing.cpp)

#target_link_libraries( optical_flow ${OpenCV_LIBS} )

#add_executable( orb src/videocapture.cpp src/orb/gpu_processing.cpp
#			    src/orb/cpu_processing.cpp)

#target_link_libraries( orb ${OpenCV_LIBS} )

add_executable( dnn_object_detection src/dnn/object_detection.cpp)

target_link_libraries( dnn_object_detection ${OpenCV_LIBS} )

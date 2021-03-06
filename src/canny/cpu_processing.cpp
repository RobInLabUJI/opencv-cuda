#include <opencv2/opencv.hpp>

void cpu_processing(cv::Mat& src, cv::Mat& dst)
{
  const int lowThreshold = 20;
  const int ratio = 3;
  const int kernel_size = 3;
  cv::Mat src_gray, detected_edges;

  cv::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );
  cv::blur( src_gray, detected_edges, cv::Size(3,3) );
  cv::Canny( detected_edges, detected_edges, lowThreshold, 
             lowThreshold*ratio, kernel_size );
  dst = cv::Scalar::all(0);
  src.copyTo( dst, detected_edges );
}


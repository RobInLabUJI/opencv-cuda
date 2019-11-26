#include <opencv2/opencv.hpp>

void cpu_processing(cv::Mat& src, cv::Mat& dst)
{
  cv::Mat src_gray, descriptors;
  std::vector<cv::KeyPoint> keypoints;

  cv::cvtColor( src, src_gray, cv::COLOR_BGR2GRAY );
  cv::Ptr< cv::ORB > detector = cv::ORB::create();

  detector->detect(src_gray, keypoints);
  detector->compute(src_gray, keypoints, descriptors);

  //dst = cv::Scalar::all(0);
  cv::drawKeypoints( src, keypoints, dst, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
}


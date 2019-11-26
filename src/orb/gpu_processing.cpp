#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>

void gpu_processing(cv::Mat& src, cv::Mat& dst)
{
  cv::cuda::GpuMat gpuInImage, gpuOutImage;
  gpuInImage.upload(src);

  std::vector<cv::KeyPoint> keypoints;

  cv::cuda::GpuMat src_gray, descriptors;
  cv::cuda::cvtColor( gpuInImage, src_gray, cv::COLOR_BGR2GRAY );

  cv::Ptr< cv::cuda::ORB > detector = cv::cuda::ORB::create();

  detector->detect(src_gray, keypoints);
  detector->compute(src_gray, keypoints, descriptors);

  cv::drawKeypoints( src, keypoints, dst, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
  //gpuOutImage.download(dst);
}

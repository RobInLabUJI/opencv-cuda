#include <opencv2/opencv.hpp>

#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>

void gpu_processing(cv::Mat& src, cv::Mat& dst)
{
  const int lowThreshold = 20;
  const int ratio = 3;
  const int kernel_size = 3;

  cv::cuda::GpuMat gpuInImage, gpuOutImage;
  gpuInImage.upload(src);

  cv::cuda::GpuMat src_gray, blurred, edge;

  cv::Ptr<cv::cuda::Filter> blur = cv::cuda::createBoxFilter( CV_8UC1, CV_8UC1, cv::Size(3,3) );

  cv::Ptr<cv::cuda::CannyEdgeDetector> canny = cv::cuda::createCannyEdgeDetector(lowThreshold,lowThreshold*ratio,kernel_size);

  cv::cuda::cvtColor( gpuInImage, src_gray, cv::COLOR_BGR2GRAY );
  blur->apply( src_gray, blurred);
  canny->detect( blurred, edge );
  gpuInImage.copyTo( gpuOutImage, edge);

  gpuOutImage.download(dst);
}

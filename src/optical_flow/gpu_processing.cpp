#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>

bool gpu_first = true;
cv::cuda::GpuMat gpu_prvs;

void gpu_processing(cv::Mat& src, cv::Mat& dst)
{
  cv::cuda::GpuMat next;
  cv::cuda::GpuMat gpuInImage, gpuOutImage;

  gpuInImage.upload(src);

  if (!gpu_first) {
	cv::cuda::cvtColor( gpuInImage, next, cv::COLOR_BGR2GRAY );
	cv::cuda::GpuMat flow(gpu_prvs.size(), CV_32FC2);
    cv::Ptr<cv::cuda::FarnebackOpticalFlow> fof = cv::cuda::FarnebackOpticalFlow::create();
	fof->calc(gpu_prvs, next, flow);
    // visualization
    cv::Mat cpu_flow, flow_parts[2];
	flow.download(cpu_flow);
    cv::split(cpu_flow, flow_parts);
    cv::Mat magnitude, angle, magn_norm;
    cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    cv::normalize(magnitude, magn_norm, 0.0f, 1.0f, cv::NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));
    //build hsv image
    cv::Mat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    cv::merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cv::cvtColor(hsv8, dst, cv::COLOR_HSV2BGR);

    gpu_prvs = next;
  } else {
    gpu_first = false;
    cv::cuda::cvtColor( gpuInImage, gpu_prvs, cv::COLOR_BGR2GRAY );
	gpuInImage.copyTo( gpuOutImage );
    gpuOutImage.download(dst);
  }
}

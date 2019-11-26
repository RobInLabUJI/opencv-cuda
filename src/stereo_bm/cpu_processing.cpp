#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

void cpu_processing(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst)
{
  int max_disp = 64;
  int wsize = 15;
  double vis_mult = 1.0;
  double lambda = 8000.0;
  double sigma = 1.5;

  cv::Mat left_disp, right_disp, filtered_disp, filtered_disp_vis;

  cv::Ptr<cv::StereoBM> left_matcher = cv::StereoBM::create(max_disp,wsize);
  cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
  cv::Ptr<cv::StereoMatcher> right_matcher = cv::ximgproc::createRightMatcher(left_matcher);

  src1.convertTo(src1, CV_8U);
  cv::cvtColor(src1, src1,  cv::COLOR_BGR2GRAY);
  src2.convertTo(src2, CV_8U);
  cv::cvtColor(src2, src2,  cv::COLOR_BGR2GRAY);
  
  left_matcher->compute(src1, src2, left_disp);
  right_matcher->compute(src2, src1, right_disp);

  wls_filter->setLambda(lambda);
  wls_filter->setSigmaColor(sigma);
  wls_filter->filter(left_disp, src1, filtered_disp, right_disp);

  cv::ximgproc::getDisparityVis(left_disp, filtered_disp_vis, vis_mult);
  //cv::ximgproc::getDisparityVis(filtered_disp, filtered_disp_vis, vis_mult);
  cv::cvtColor(filtered_disp_vis, dst, cv::COLOR_GRAY2BGR);
}


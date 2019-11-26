#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

void cpu_processing(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst)
{
  int max_disp = 96;
  int wsize = 7;
  double vis_mult = 10.0;
  double lambda = 8000.0;
  double sigma = 1.5;

  cv::Mat src18, src18g, src28, src28g, dst_bw;
  cv::Mat left_disp, right_disp, filtered_disp, filtered_disp_vis;

  //cv::Ptr<cv::StereoBM> left_matcher = cv::StereoBM::create(max_disp,wsize);
  //cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);
  //cv::Ptr<cv::StereoMatcher> right_matcher = cv::ximgproc::createRightMatcher(left_matcher);

  cv::Ptr<cv::StereoSGBM> left_matcher = cv::StereoSGBM::create(0,    //int minDisparity
                                    128,     //int numDisparities
                                    3,      //int SADWindowSize
                                    3*3*4,    //int P1 = 0
                                    3*3*32,   //int P2 = 0
                                    1,     //int disp12MaxDiff = 0
                                    63,     //int preFilterCap = 0
                                    10,      //int uniquenessRatio = 0
                                    100,    //int speckleWindowSize = 0
                                    32,     //int speckleRange = 0
                                    true);  //bool fullDP = false

  src1.convertTo(src18, CV_8U);
  cv::cvtColor(src18, src18g,  cv::COLOR_BGR2GRAY);
  src2.convertTo(src28, CV_8U);
  cv::cvtColor(src28, src28g,  cv::COLOR_BGR2GRAY);
  
  left_matcher->compute(src18g, src28g, left_disp);
  double minVal; double maxVal;
  cv::minMaxLoc( left_disp, &minVal, &maxVal );
  left_disp.convertTo( dst_bw, CV_8UC1, 255/(maxVal - minVal));
  cv::cvtColor(dst_bw, dst, cv::COLOR_GRAY2BGR);
  //cv::applyColorMap(dst_bw, dst, cv::COLORMAP_JET);
  //right_matcher->compute(src28g, src18g, right_disp);

  //wls_filter->setLambda(lambda);
  //wls_filter->setSigmaColor(sigma);
  //wls_filter->filter(left_disp, src18g, filtered_disp, right_disp);

  //cv::ximgproc::getDisparityVis(left_disp, filtered_disp_vis, vis_mult);
  //cv::ximgproc::getDisparityVis(filtered_disp, filtered_disp_vis, vis_mult);
  //cv::cvtColor(filtered_disp_vis, dst, cv::COLOR_GRAY2BGR);
}


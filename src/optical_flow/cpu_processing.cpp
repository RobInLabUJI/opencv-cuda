#include <opencv2/opencv.hpp>

bool cpu_first = true;
cv::Mat cpu_prvs;

void cpu_processing(cv::Mat& src, cv::Mat& dst)
{
  cv::Mat next;

  if (!cpu_first) {
	cv::cvtColor( src, next, cv::COLOR_BGR2GRAY );
	cv::Mat flow(cpu_prvs.size(), CV_32FC2);
	cv::calcOpticalFlowFarneback(cpu_prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
    // visualization
    cv::Mat flow_parts[2];
    cv::split(flow, flow_parts);
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
    cpu_prvs = next;
  } else {
    cpu_first = false;
    ///cpu_prvs.create( src.size(), src.type() );
    cv::cvtColor( src, cpu_prvs, cv::COLOR_BGR2GRAY );
	src.copyTo( dst );
  }
}


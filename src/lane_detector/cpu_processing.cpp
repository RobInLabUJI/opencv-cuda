#include <opencv2/opencv.hpp>

#include "LaneDetector.hpp"

/*
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
  src.copyTo( dst, detected_edges);
}
*/

void cpu_processing(cv::Mat& src, cv::Mat& dst)
{
    LaneDetector lanedetector;  // Create the class object
    cv::Mat frame;
    cv::Mat img_denoise;
    cv::Mat img_edges;
    cv::Mat img_mask;
    cv::Mat img_lines;
    std::vector<cv::Vec4i> lines;
    std::vector<std::vector<cv::Vec4i> > left_right_lines;
    std::vector<cv::Point> lane;
    std::string turn;
    int flag_plot = -1;
    int i = 0;

      img_denoise = lanedetector.deNoise(src);
      img_edges = lanedetector.edgeDetector(img_denoise);
      img_mask = lanedetector.mask(img_edges);
	  dst = cv::Scalar::all(0);

      lines = lanedetector.houghLines(img_mask);
      if (!lines.empty()) {
        left_right_lines = lanedetector.lineSeparation(lines, img_edges);
        lane = lanedetector.regression(left_right_lines, frame);
        //turn = lanedetector.predictTurn();

		std::vector<cv::Point> poly_points;
		//src.copyTo(dst);
	  	poly_points.push_back(lane[2]);
	  	poly_points.push_back(lane[0]);
	  	poly_points.push_back(lane[1]);
	  	poly_points.push_back(lane[3]);
	  	cv::fillConvexPoly(dst, poly_points, cv::Scalar(0, 0, 255), CV_AA, 0);
	  	cv::addWeighted(dst, 0.3, src, 1.0 - 0.3, 0, dst);
		cv::line(dst, lane[0], lane[1], cv::Scalar(0, 255, 255), 5, CV_AA);
		cv::line(dst, lane[2], lane[3], cv::Scalar(0, 255, 255), 5, CV_AA);
	  } else {
        src.copyTo( dst, img_mask);
	  }

}


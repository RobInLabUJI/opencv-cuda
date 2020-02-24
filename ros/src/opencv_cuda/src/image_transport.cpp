#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/opencv.hpp>

void cpu_processing(cv_bridge::CvImagePtr src, cv_bridge::CvImagePtr dst)
{

  cv::Mat src_gray, descriptors;
  std::vector<cv::KeyPoint> keypoints;

  cv::cvtColor( src->image, src_gray, cv::COLOR_BGR2GRAY );
  cv::Ptr< cv::ORB > detector = cv::ORB::create();

  detector->detect(src_gray, keypoints);
  detector->compute(src_gray, keypoints, descriptors);

  //dst = cv::Scalar::all(0);

  cv::drawKeypoints( src->image, keypoints, src->image, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
}

//extern void cpu_processing(cv::Mat& in, cv::Mat& out);
//extern void gpu_processing(cv::Mat& in, cv::Mat& out);

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/output_video", 1);

    //cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    //cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr, dst;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Draw an example circle on the video stream
    //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    //  cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    //cv::waitKey(3);
    cpu_processing(cv_ptr, dst);
    
    // Output modified video stream
    //image_pub_.publish(cv_ptr->toImageMsg());
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

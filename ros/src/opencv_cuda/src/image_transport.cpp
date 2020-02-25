#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>

cv_bridge::CvImagePtr cpu_processing(cv_bridge::CvImagePtr src)
{
  cv::Mat src_gray, descriptors;
  std::vector<cv::KeyPoint> keypoints;

  cv::cvtColor( src->image, src_gray, cv::COLOR_BGR2GRAY );
  cv::Ptr< cv::ORB > detector = cv::ORB::create();

  detector->detect(src_gray, keypoints);
  detector->compute(src_gray, keypoints, descriptors);

  cv::drawKeypoints( src->image, keypoints, src->image, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );

  return src;
}

cv_bridge::CvImagePtr gpu_processing(cv_bridge::CvImagePtr src)
{
  cv::cuda::GpuMat gpuInImage, gpuOutImage;
  gpuInImage.upload(src->image);

  std::vector<cv::KeyPoint> keypoints;

  cv::cuda::GpuMat src_gray, descriptors;
  cv::cuda::cvtColor( gpuInImage, src_gray, cv::COLOR_BGR2GRAY );

  cv::Ptr< cv::cuda::ORB > detector = cv::cuda::ORB::create();

  detector->detect(src_gray, keypoints);
  detector->compute(src_gray, keypoints, descriptors);

  cv::drawKeypoints( src->image, keypoints, src->image, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT );
  return src;
}

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  bool use_gpu_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscribe to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/output_video", 1);
    ros::NodeHandle private_nh("~");
    private_nh.getParam("use_gpu", use_gpu_);
  }

  ~ImageConverter()
  {
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

    double ticks = (double)cv::getTickCount();
    if (use_gpu_) {
        dst = gpu_processing(cv_ptr);
    } else {
        dst = cpu_processing(cv_ptr);
    }
    ticks = ((double)cv::getTickCount() - ticks)/cv::getTickFrequency()*1000;
    cv::String message;
    if (use_gpu_) {
      message = cv::format("GPU processing time[ms]=%.3f", ticks);
    } else {
      message = cv::format("CPU processing time[ms]=%.3f", ticks);       
    }
    cv::putText(dst->image, message, cv::Point(30, 90), 
                cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0,0,255), 3);
    
    // Output modified video stream
    image_pub_.publish(dst->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "orb_demo");
  ImageConverter ic;
  ros::spin();
  return 0;
}

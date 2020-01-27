#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <iostream>
#include <fstream>

using namespace std;

extern void cpu_processing(cv::Mat& in, cv::Mat& out);
extern void gpu_processing(cv::Mat& in, cv::Mat& out);

int main(int, char**)
{
  cv::Mat frame, dst;
  cv::VideoCapture cap;
  bool use_gpu = false;
  int i, n;
  double t[30];
  for (i=0;i<30;i++) t[i]=0.0;
  ofstream out("result.txt");

    int CudaDevice;
    if(cv::cuda::getCudaEnabledDeviceCount()==0)
    {
        cerr<<endl<<"ERROR: NO CudaEnabledDevice"<<endl;
        exit(2);
    }
    else
    {
        CudaDevice = cv::cuda::getDevice();
        cv::cuda::setDevice(CudaDevice);
    }
    
  cap.open("project_video.mp4");
  if (!cap.isOpened()) {
    cerr << "ERROR! Unable to open video\n";
    return -1;
  }
  cout << "Start grabbing" << endl
       << "Press c to use CPU" << endl
       << "Press g to use GPU" << endl
       << "Press q to terminate" << endl;
  int fps = 25;
  int delay = 1000 / fps;
  i = 0;
  n = 0;
  while ( cap.isOpened() )
  {
    cap.read(frame);
    if (frame.empty()) {
      cerr << "ERROR! blank frame grabbed\n";
      break;
    }
    //cv::cvtColor( frame, frame, cv::COLOR_RGB2BGR );
    dst.create( frame.size(), frame.type() );

    double ticks = (double)cv::getTickCount();
    if (use_gpu) {
        gpu_processing(frame, dst);
    } else {
        cpu_processing(frame, dst);
    }
    ticks = ((double)cv::getTickCount() - ticks)/cv::getTickFrequency()*1000;
	t[i++] = ticks;
    if (n<30) n++;
    if (i==30) i=0;
    double m=0;
    for(int j=0;j<n;j++) m+=t[j];
    m /= n;
    out << m << endl;
    cv::String message;
    if (use_gpu) {
      message = cv::format("GPU processing time[ms]=%.3f", m);
    } else {
      message = cv::format("CPU processing time[ms]=%.3f", m);       
    }
    cv::putText(dst, message, cv::Point(30, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
    cv::imshow("Live", dst);
    int pause = delay-int(ticks);
    if (pause < 1) pause = 1;
    int key = cv::waitKey(pause); 
    if ( key == 'g') {
       use_gpu = true;
       n = 0;
 	   i = 0;
    }
    if ( key == 'c') {
       use_gpu = false;
       n = 0;
 	   i = 0;
    }
    if ( key == 'q')
      break;
  }
  out.close();
  return 0;
}


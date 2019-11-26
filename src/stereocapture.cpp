#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <iostream>
#include <chrono>

using namespace std;

extern void cpu_processing(cv::Mat& in1, cv::Mat& in2, cv::Mat& out);
extern void gpu_processing(cv::Mat& in1, cv::Mat& in2, cv::Mat& out);

int main(int, char**)
{
  cv::Mat frame1, frame2, dst;
  cv::VideoCapture cap1, cap2;
  bool use_gpu = false;
  int i, n;
  double t[30];
  for (i=0;i<30;i++) t[i]=0.0;

  cap1.open("I1_out.mp4");
  if (!cap1.isOpened()) {
    cerr << "ERROR! Unable to open video\n";
    return -1;
  }
  cap2.open("I1_out.mp4");
  if (!cap2.isOpened()) {
    cerr << "ERROR! Unable to open video\n";
    return -1;
  }
  cout << "Start grabbing" << endl
       << "Press c to use CPU" << endl
       << "Press g to use GPU" << endl
       << "Press q to terminate" << endl;
  int fps = cap1.get(CV_CAP_PROP_FPS);
  int delay = 1000 / fps;
  i = 0;
  while ( cap1.isOpened() )
  {
    clock_t startTime = clock();
    cap1.read(frame1);
    if (frame1.empty()) {
      cerr << "ERROR! blank frame grabbed\n";
      break;
    }
    cap2.read(frame2);
    if (frame2.empty()) {
      cerr << "ERROR! blank frame grabbed\n";
      break;
    }
    dst.create( frame1.size(), frame1.type() );

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    if (use_gpu) {
        gpu_processing(frame1, frame2, dst);
    } else {
        cpu_processing(frame1, frame2, dst);
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(end - begin).count()/1000.0;
	t[i++] = duration;
    if (i==30) i=0;
    double m=0;
    for(int j=0;j<30;j++) m+=t[j];
    m /= 30;
    cv::String message;
    if (use_gpu) {
      message = cv::format("GPU processing time[ms]=%.3f", m);
    } else {
      message = cv::format("CPU processing time[ms]=%.3f", m);       
    }
    cv::putText(dst, message, cv::Point(30, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255));
    cv::imshow("Live", dst);
    int pause = delay-int(duration);
    if (pause < 1) pause = 1;
    int key = cv::waitKey(pause); 
    if ( key == 'g')
       use_gpu = true;
    if ( key == 'c')
       use_gpu = false;
    if ( key == 'q')
      break;
  }
  return 0;
}



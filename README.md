# opencv-cuda

Docker image with OpenCV 4.2.0 compiled for CUDA 10.1.

## Prerequisites

* Linux PC with NVIDIA GPU
* [NVIDIA drivers (>= 418.39)](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html)
* [Docker (>=19.03) with GPU support](https://github.com/NVIDIA/nvidia-docker) <sup>1</sup>

Note that you do not need to install the CUDA toolkit on the host, but the driver needs to be installed.

## Quickstart

1. Clone this repository and select the `dnn`branch
```
git clone https://github.com/RobInLabUJI/opencv-cuda.git
cd opencv-cuda
git checkout dnn
```
2. Pull the Docker image
```
docker pull cuda4opencv/demos:opencv-4.2.0-cuda-10.1
```
3. Compile the demos
```
./compile_demos
```
4. Run the demos
```
./run_demo [canny | orb | optical_flow]
```
Press 'c' to use CPU, 'g' to use GPU, or 'q' to terminate.

5. Run the DNN example

Download the file [yolov3.weights](https://pjreddie.com/media/files/yolov3.weights) into to the `src/dnn` folder.
```
./run_bash
./src/dnn/run_object_detection
./src/dnn/run_object_detection_cuda
```
## Building the Docker image
This can take ~30 minutes or longer, depending on the PC hardware.
```
./build_image
```

## Acknowledgments

`project_video.mp4` courtesy of [Udacity's Advanced Line Finding Project](https://github.com/udacity/CarND-Advanced-Lane-Lines) ([MIT License](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/LICENSE))

`Dockerfile` heavily based on [Thomas Herbin's file](https://github.com/loitho/docker-opencv-cuda) ([MIT License](https://github.com/loitho/docker-opencv-cuda/blob/master/LICENSE))

<hr>

[1]: [nvidia-docker 2.0](https://github.com/NVIDIA/nvidia-docker/wiki/Installation-(version-2.0)) can be used, but you should replace `--gpus all` with `--runtime nvidia` in the `run_demo` script.

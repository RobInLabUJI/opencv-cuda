# opencv-cuda

Docker image with OpenCV 3.4.0 compiled for CUDA 8.0.

## Prerequisites

* Linux PC with NVIDIA GPU
* [NVIDIA drivers (>= 367.48)](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html)
* [Docker (>=19.03) with GPU support](https://github.com/NVIDIA/nvidia-docker) <sup>1</sup>

Note that you do not need to install the CUDA toolkit on the host, but the driver needs to be installed.

## Quickstart

1. Clone this repository
```
git clone -b ros --recurse-submodules https://github.com/RobInLabUJI/opencv-cuda.git
cd opencv-cuda
```
2. Pull the Docker image
```
docker pull cuda4opencv/demos:ros-kinetic
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

5. Run the ROS example
```
./launch_demo
```
or
```
./launch_demo_cuda
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

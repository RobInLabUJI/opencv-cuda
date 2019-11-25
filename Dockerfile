FROM nvidia/cuda:10.1-devel-ubuntu18.04

ADD opencv-3.4.0.tar.gz /usr

RUN apt-get update \
  && apt-get install -y \
    python3 cmake g++ \
    libjpeg-dev libwebp-dev libpng-dev \
    libtiff-dev libopenexr-dev \
  && rm -rf /var/lib/apt/lists/*

RUN apt-get update \
  && apt-get install -y \
    libgtk2.0-dev libcairo-dev libgdk-pixbuf2.0-dev \
    libdc1394-22-dev libavcodec-dev libavformat-dev libavutil-dev \
    libswscale.dev libgphoto2-dev \
  && rm -rf /var/lib/apt/lists/*

ADD include /root/include
ADD src /root/src

ADD README.txt CMakeLists.txt project_video.mp4 /root/

WORKDIR /root

RUN mkdir build && cd build && cmake .. && make

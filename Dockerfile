FROM nvidia/cuda:8.0-devel-ubuntu16.04

ARG OPENCV_VERSION=3.4.0

RUN apt-get update && \
        apt-get install -y \
        libcairo-dev libgdk-pixbuf2.0-dev \
        libdc1394-22-dev libavcodec-dev libavformat-dev libavutil-dev \
        libswscale.dev libgphoto2-dev \
        python3 cmake g++ \
        libwebp-dev libpng-dev \
        libtiff-dev libopenexr-dev \
        python-pip \
        build-essential \
        cmake \
        git \
        wget \
        unzip \
        yasm \
        pkg-config \
        libswscale-dev \
        libtbb2 \
        libtbb-dev \
        libjpeg-dev \
        libpng-dev \
        libtiff-dev \
        libjasper-dev \
        libavformat-dev \
        libpq-dev \
        libxine2-dev \
        libglew-dev \
        libtiff5-dev \
        zlib1g-dev \
        libjpeg-dev \
        libpng12-dev \
        libjasper-dev \
        libavcodec-dev \
        libavformat-dev \
        libavutil-dev \
        libpostproc-dev \
        libswscale-dev \
        libeigen3-dev \
        libtbb-dev \
        libgtk2.0-dev \
        pkg-config \
        python-dev \
        python-numpy \
        python3-dev \
        python3-numpy \
        libvtk6-dev \
        && rm -rf /var/lib/apt/lists/*

WORKDIR /

## Single command to reduce image size
## Build opencv
RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip \
    && unzip opencv.zip \
    && rm opencv.zip

RUN wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/$OPENCV_VERSION.zip \
    && unzip opencv_contrib.zip \
    && rm opencv_contrib.zip

RUN mkdir /opencv-$OPENCV_VERSION/cmake_binary \
    && cd /opencv-$OPENCV_VERSION/cmake_binary \
    && cmake -DBUILD_TIFF=ON \
       -DBUILD_opencv_java=OFF \
       -DBUILD_SHARED_LIBS=OFF \
       -DWITH_CUDA=ON \
       -DENABLE_FAST_MATH=1 \
       -DCUDA_FAST_MATH=1 \
       -DWITH_CUBLAS=1 \
       -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-8.0 \
       ## Should compile for most card
       ## 3.5 binary code for devices with compute capability 3.5 and 3.7,
       ## 5.0 binary code for devices with compute capability 5.0 and 5.2,
       ## 6.0 binary code for devices with compute capability 6.0 and 6.1,
       -DCUDA_ARCH_BIN='3.0 3.5 5.0 6.0 6.2' \
       -DCUDA_ARCH_PTX="" \
       ## AVX in dispatch because not all machines have it
       -DCPU_DISPATCH=AVX,AVX2 \
       -DENABLE_PRECOMPILED_HEADERS=OFF \
       -DWITH_OPENGL=OFF \
       -DWITH_OPENCL=OFF \
       -DWITH_QT=OFF \
       -DWITH_IPP=ON \
       -DWITH_TBB=ON \
       -DFORCE_VTK=ON \
       -DWITH_EIGEN=ON \
       -DWITH_V4L=ON \
       -DWITH_XINE=ON \
       -DWITH_GDAL=ON \
       -DWITH_1394=OFF \
       -DWITH_FFMPEG=OFF \
       -DBUILD_PROTOBUF=OFF \
       -DBUILD_TESTS=OFF \
       -DBUILD_PERF_TESTS=OFF \
       -DCMAKE_BUILD_TYPE=RELEASE \
       -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-$OPENCV_VERSION/modules \
    ..

RUN export NUMPROC=$(nproc --all) \
    && cd /opencv-$OPENCV_VERSION/cmake_binary \
    && make -j$NUMPROC install

RUN rm -r /opencv-$OPENCV_VERSION \
    && rm -r /opencv_contrib-$OPENCV_VERSION

RUN apt-get update \
    && apt-get install -y libxine2 \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /root/demos

WORKDIR /root/demos


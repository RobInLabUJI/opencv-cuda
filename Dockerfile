FROM nvidia/cuda:10.1-cudnn7-devel-ubuntu18.04

ARG OPENCV_VERSION=4.2.0

RUN add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"

RUN apt-get update && \
        apt-get install -y \
        cmake g++ build-essential git wget unzip yasm pkg-config \
        python3 python-pip python-dev python-numpy python3-dev python3-numpy \
        libcairo-dev libgdk-pixbuf2.0-dev libswscale.dev libgphoto2-dev \
        libdc1394-22-dev libavcodec-dev libavformat-dev libavutil-dev \
        libwebp-dev libpng-dev libtiff-dev libopenexr-dev \
        libswscale-dev libtbb2 libtbb-dev libjpeg8-dev libpng-dev \
        libtiff-dev libjasper1 libavformat-dev libpq-dev \
        libxine2-dev libglew-dev libtiff-dev zlib1g-dev \
        libavcodec-dev libavformat-dev \
        libavutil-dev libpostproc-dev libswscale-dev libeigen3-dev \
        libtbb-dev libgtk2.0-dev libvtk6-dev libxine2-dev \
        && rm -rf /var/lib/apt/lists/*

WORKDIR /

RUN wget -O opencv.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip \
    && unzip opencv.zip \
    && rm opencv.zip \
    && wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/$OPENCV_VERSION.zip \
    && unzip opencv_contrib.zip \
    && rm opencv_contrib.zip \
    && mkdir /opencv-$OPENCV_VERSION/cmake_binary \
    && cd /opencv-$OPENCV_VERSION/cmake_binary \
    && cmake -DBUILD_TIFF=ON \
       -DBUILD_opencv_java=OFF \
       -DBUILD_SHARED_LIBS=ON \
       -DWITH_CUDA=ON \
       -DWITH_CUBLAS=ON \
       -DWITH_CUDNN=ON \
       -DOPENCV_DNN_CUDA=ON \
       -DENABLE_FAST_MATH=1 \
       -DCUDA_FAST_MATH=1 \
       -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-9.0 \
       ## Should compile for most card
       ## 3.5 binary code for devices with compute capability 3.5 and 3.7,
       ## 5.0 binary code for devices with compute capability 5.0 and 5.2,
       ## 6.0 binary code for devices with compute capability 6.0 and 6.1,
       ##-DCUDA_ARCH_BIN='3.0 3.5 5.0 6.0 6.2' \
       -DCUDA_ARCH_BIN='6.0 6.2' \
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
       -DBUILD_PROTOBUF=ON \
       -DBUILD_EXAMPLES=ON \
       -DBUILD_TESTS=OFF \
       -DBUILD_PERF_TESTS=OFF \
       -DCMAKE_BUILD_TYPE=RELEASE \
       -DCMAKE_LIBRARY_PATH=/usr/local/cuda/lib64/stubs/ \
       -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-$OPENCV_VERSION/modules \
    .. \
    && export NUMPROC=$(nproc --all) \
    && cd /opencv-$OPENCV_VERSION/cmake_binary \
    && make -j$NUMPROC install \
    && rm -r /opencv-$OPENCV_VERSION \
    && rm -r /opencv_contrib-$OPENCV_VERSION

RUN mkdir /root/demos

WORKDIR /root/demos


# ADA [![Build Status](https://app.travis-ci.com/patriciogonzalezvivo/ada.svg?branch=main)](https://app.travis-ci.com/github/patriciogonzalezvivo/ada)


The fundation of any visual computational adventure. 
Ada is an easy cross platform OpenGL ES 2.0 library for creating apps in Windows, MacOS and Linux (with or without X11 support like in RaspberrPi)

## Dependencies

### MacOS

```bash
brew install glfw3 pkg-config
```

For video support (using FFMpeg library LIBAV), also do:

```bash
brew install ffmpeg --build-from-source
```

### Ubuntu 

```bash
sudo apt install git cmake xorg-dev libglu1-mesa-dev
```

For video support (using FFMpeg library LIBAV), also do:

```bash
sudo apt install ffmpeg libavcodec-dev libavcodec-extra libavfilter-dev libavfilter-extra libavdevice-dev libavformat-dev libavutil-dev libswscale-dev libv4l-dev libjpeg-dev libpng-dev libtiff-dev
```


### RaspberryPi OS (with X11)

```bash
sudo apt install git cmake xorg-dev libglu1-mesa-dev
```

### RaspberryPi OS (without X11)

```bash
sudo apt install git cmake libgbm-dev libdrm-dev libegl1-mesa-dev libgles2-mesa-dev
```

Make sure on your `/boot/config.txt` you have the following lines present and uncommented:

```bash
dtoverlay=vc4-fkms-v3d
max_framebuffers=2
hdmi_force_hotplug=1
```

### Fedora

```bash
sudo dnf install git gcc-c++ cmake mesa-libGLU-devel glfw-devel libXi-devel libXxf86vm-devel 
sudo yum install libXdamage-devel 

```

For video support (using FFMpeg library LIBAV), also do:
```bash
sudo dnf install ffmpeg ffmpeg-devel
```

### Arch 

```bash
sudo pacman -S glu glfw-x11
```

For video support (using FFMpeg library LIBAV), also do:

```bash
sudo pacman -S ffmpeg
```


## Compiling Ada projects

### For windows managers like MacOS, Windows or any Linux X11 enviroment like Gnome/KDE/etc (all through GLFW) 

```bash
git clone https://github.com/patriciogonzalezvivo/ada.git
cd ada
mkdir build
cd build
cmake ..
make
sudo make install
example/./hello_world
```

### For no X11 context (GBM by default or BROADCOM the older VC drivers on Raspberry PI < 4)

```bash
git clone https://github.com/patriciogonzalezvivo/ada.git
cd ada
mkdir build
cd build
cmake -DNO_X11=TRUE ..
make
sudo make install
example/./hello_world
```

**Note**: Newer RaspberryPi distribution do have BROADCOM drivers but work only on GBM for that use `-DFORCE_GBM=TRUE` instead

### As a emscripten WebAssembly project

1. [Install emscripten](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions) which already includes glfw: 
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ..
```

2. Then build the project
```bash
git clone https://github.com/patriciogonzalezvivo/ada.git
cd ada
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
make
```

3. Serve the file either using node or python
```bash
python3 -m http.server 
```
Then open http://localhost:8000/examples/hello_world.html


## Building your own app with ADA


Add the ADA header files you need

```c++
#include "ada/window.h"
#include "ada/gl/gl.h"

...

int main(int argc, char **argv) {
    // Initialize openGL context
    ada::initGL(argc, argv);

     // Render Loop
    while ( ada::isGL() ) {
        // Update
        ada::updateGL();

        // Do your thing
        // ...

        // Render the scene
        ada::renderGL();

        // Update the viewport
        ada::updateViewport();
    }

    // Close the GL context
    ada::closeGL();

    return 1;
}

```

Then on the `CMakeList.txt` you just need to do:

```cmake
add_executable (myApp myApp.cpp)

set_target_properties(myApp PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries (myApp PRIVATE ada)
```

# Acknowledgements

This library is highly inspired on [OpenFramewors](https://github.com/openframeworks/openFrameworks), a dear framework and community very close to my heart. The Mesh, Node and Camera clases are VERY similar. The reason to branch ADA was to minimize the amount of code, dependencies and be hable to add it through CMake.


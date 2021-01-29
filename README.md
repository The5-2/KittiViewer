## About The Project

Simple tool to view kitti datasets.

### Important

Change the variable "pathCalib" in main.cpp to the dataset folder e.g. 
```cpp
std::string pathCalib = "D:/Dev/KittiViewer/dataset/"
```


## Datasets

Datasets can be found here:
http://www.cvlibs.net/datasets/kitti/raw_data.php
The following files are required from the dataset to run the viewer:

| Name             |  File |
:-------------------------:|:-------------------------:
synced+rectified data | velodyne_points/data, oxts/data, image_0X/data
calibration | calib_cam_to_cam.txt, calib_velo_to_cam.txt
tracklets | tracklet_labels.xml

The image_0X folder needs to be renamed to image. The following folder structure is required for the dataset:

    .
    ├── tracklet_labels.xml
    ├── calib_cam_to_cam.txt
    ├── calib_velo_to_cam.txt
    ├── velodyne_points
    │   ├── timestamps.txt
    │   └── data
    │       └── ...
    ├── oxts
    │   ├── dataformat.txt
    │   ├── timestamps.txt
    │   └── data
    │       └── ...
    └── image
        ├── timestamps.txt
        ├── timestamps_end.txt
        ├── timestamps_start.txt
        └── data
            └── ...

### Installation

1. Clone the repo
```sh
git clone
```
2. Submodules
```sh
git submodule update --init --recursive
```
3. Build
```sh
cd path/to/kittiViewer  && mkdir build && cd build && cmake .. && make -j4
```

## Example

The shown example uses the dataset "2011_09_26_drive_0070"
![](images/KittiRender.gif)

## 3rd Party

| Project             |  License | Link|
:-------------------------:|:-------------------------:|:-------------------------:
imGui| MIT | https://github.com/ocornut/imgui/blob/master/LICENSE.txt
stb|  MIT | https://github.com/nothings/stb/blob/master/LICENSE
glm| MIT | https://github.com/g-truc/glm/blob/master/copying.txt
glfw| zlib | https://github.com/glfw/glfw/blob/master/LICENSE.md
glew-cmake| MIT | https://github.com/Perlmint/glew-cmake/blob/master/LICENSE.txt
dirent| MIT | https://github.com/tronkko/dirent/blob/master/LICENSE
tinyxml2| zlib | https://github.com/leethomason/tinyxml2/blob/master/LICENSE.txt


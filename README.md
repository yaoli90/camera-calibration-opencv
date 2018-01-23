# Camera Calibration Opencv

This project calibrate a camera base on so-called pinhole camera model. 
You can find the detail in [OpenCV - Camera Calibration and 3D Reconstruction](https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html).

## Getting Started

Take couple photos of the checkboard (usually 20-30 photos) by the camera. Rename the photos as "img1", "img2", ...

<img src="https://github.com/yaoli90/camera-calibration-opencv/blob/master/_calib_imgs/img1.jpg" width="400">

### Do calibration

```cpp
calibrateCamera cam(24, 11, 7, 20.625, "_calib_imgs\\");
cam.calibrate();
cam.generateCalibrationFile();
```

Initiate the calibration by setting calibration images information. 

```cpp
// nImages: number of calibration images
// nCornersWidth: number of interior corners in width
// nCornersHight: number of interior corners in height
// squareSizeMM: size of one square in millimeter

calibrateCamera::calibrateCamera(int nImages, int nCornersWidth, int nCornersHight, float squareSizeMM, 
  const std::string& imagesDir = "", const std::string& imagesFileName = "img", 
  const std::string& imagesExtension = "jpg");
```
The calibration process includes
- look for the interior corners for all images
- store the pixel locations of the interior corners (imagePoints)
- create a corresponding array to store the actual 3D loaction of the corners (objectPoints)
- calculate camera matrix (K) and distortion coefficient (D)
- project the "objectPoints" onto images based on "K" and "D" we just calculated
- calculate the mean square error between the projected points and "imagePoints"

```cpp
void calibrate(void);
```

Finally, write camera matrix (K), distortion coefficient (D) and some other imformation about this calibration to file

```cpp
void generateCalibrationFile(const std::string& fileName = "cam_.xml");
```

### Undistort Camera Frames

```cpp
calibrateCamera cam("cam_.xml");
cv::VideoCapture cap(0);
cv::Mat frame, undistortedFrame;
while (cap.isOpened()) {
  cap >> frame;
  cam.undistortFrame(frame, undistortedFrame);
  cv::imshow("Undistorted", undistortedFrame);
  cv::imshow("Original", frame);
  cv::waitKey(10);
}
```

Initiate the calibration by reading the calibration file we just generated. 

```cpp
void generateCalibrationFile(const std::string& fileName = "cam_.xml");
```

At the first time "undistortFrame" is called, transformation map will be calculated and stored. If the map already exists, it won't be calculated again.

```cpp
void undistortFrame(cv::Mat& origFrame,cv::Mat& undistortedFrame);
```

<img src="https://github.com/yaoli90/camera-calibration-opencv/blob/master/camCali.png" width="800">

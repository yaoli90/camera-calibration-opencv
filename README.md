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



A step by step series of examples that tell you have to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc

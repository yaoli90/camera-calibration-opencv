#ifndef CALIBCAMERA_H
#define CALIBCAMERA_H
#include <opencv2/aruco.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>


class calibrateCamera {
public:
	int nCornersWidth;
	int nCornersHight;
	int nImages;
	float squareSizeMM;
	std::string imagesDir;
	std::string imagesFileName;
	std::string calibrationFile;
	std::string imagesExtension;

	cv::Size imagesSize;
	cv::Mat K, D;

	calibrateCamera(int nImages, int nCornersWidth, int nCornersHight, float squareSizeMM,
		const std::string& imagesDir = "", const std::string& imagesFileName = "img", const std::string& imagesExtension = "jpg");
	calibrateCamera(const std::string& calibrationFile = "cam_.xml");
	void generateCalibrationFile(const std::string& fileName = "cam_.xml");
	void calibrate(void);
	void undistortFrame(cv::Mat& origFrame,cv::Mat& undistortedFrame);

private:
	std::vector< std::vector< cv::Point2f > > imagePoints;
	std::vector< std::vector< cv::Point3f > > objectPoints;
	std::vector< cv::Mat > rvecs, tvecs;
	cv::Mat map1, map2;
	void loadCalibrationImages();
	double reprojectionMSE();
};
#endif
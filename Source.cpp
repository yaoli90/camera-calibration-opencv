#include "calibCamera.h"

void main() {

	//calibrateCamera cam(24, 11, 7, 20.625, "_calib_imgs");
	//cam.calibrate();
	//cam.generateCalibrationFile();

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
	return;
}

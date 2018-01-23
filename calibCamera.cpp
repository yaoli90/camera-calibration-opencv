#include "calibCamera.h"

calibrateCamera::calibrateCamera(int nImages, int nCornersWidth, int nCornersHight, float squareSizeMM,
	const std::string& imagesDir, const std::string& imagesFileName, const std::string& imagesExtension) {
	this->nCornersWidth = nCornersWidth;
	this->nCornersHight = nCornersHight;
	this->nImages = nImages;
	this->squareSizeMM = squareSizeMM;
	this->imagesDir = imagesDir;
	this->imagesFileName = imagesFileName;
	this->imagesExtension = imagesExtension;
}

calibrateCamera::calibrateCamera(const std::string& calibrationFile) {
	cv::FileStorage cameraCalibrationStream(calibrationFile, cv::FileStorage::READ);
	if (!cameraCalibrationStream.isOpened()) {
		std::cout << "Cannot find calibration file " + calibrationFile + " .\n";
		return;
	}
	this->calibrationFile = calibrationFile;
	cameraCalibrationStream["K"] >> this->K;
	cameraCalibrationStream["D"] >> this->D;
	cameraCalibrationStream["nCornersWidth"] >> this->nCornersWidth;
	cameraCalibrationStream["nCornersHight"] >> this->nCornersHight;
	cameraCalibrationStream["squareSizeMM"] >> this->squareSizeMM;
	cameraCalibrationStream["imageSize"] >> this->imagesSize;
}

void calibrateCamera::loadCalibrationImages() {
	cv::Mat image, imageGray;
	cv::Size nCorners = cv::Size(this->nCornersWidth, this->nCornersHight);

	for (int k = 1; k <= this->nImages; k++) {
		std::string imageFile = this->imagesDir + this->imagesFileName + std::to_string(k) + "." + this->imagesExtension;
		image = cv::imread(imageFile, CV_LOAD_IMAGE_COLOR);
		if (image.empty()) {
			std::cout << "Uable to read" + imageFile + "\n";
			return;
		}
		cv::cvtColor(image, imageGray, CV_BGR2GRAY);
		std::vector< cv::Point2f > imageCorners;
		std::vector< cv::Point3f > objectCorners;
		bool cornersFound = false;
		cornersFound = cv::findChessboardCorners(image, nCorners, imageCorners,
			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if (!cornersFound) {
			std::cout << "Corners in " + imageFile + " NOT found. \n";
			continue;
		}
		else {
			std::cout << "Corners in " + imageFile + " found. \n";
			cv::cornerSubPix(imageGray, imageCorners, cv::Size(5, 5), cv::Size(-1, -1),
				cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			for (int i = 0; i < nCorners.height; i++)
				for (int j = 0; j < nCorners.width; j++)
					objectCorners.push_back(cv::Point3f((float)j * this->squareSizeMM, (float)i * this->squareSizeMM, 0));
			this->imagePoints.push_back(imageCorners);
			this->objectPoints.push_back(objectCorners);
		}
	}
	this->imagesSize = image.size();
	return;
}

double calibrateCamera::reprojectionMSE() {
	std::vector< cv::Point2f > reprojectedImagePoints;
	double totalErr = 0;
	int totalPoints = 0;

	for (int i = 0; i < objectPoints.size(); i++) {
		cv::projectPoints(cv::Mat(this->objectPoints[i]), this->rvecs[i], this->tvecs[i], K, D, reprojectedImagePoints);
		double err = cv::norm(cv::Mat(imagePoints[i]), cv::Mat(reprojectedImagePoints), CV_L2);
		totalErr += err*err;
		totalPoints += objectPoints[i].size();
	}
	return std::sqrt(totalErr / totalPoints);
}

void calibrateCamera::generateCalibrationFile(const std::string& fileName) {
	this->calibrationFile = fileName;

	cv::FileStorage fs(fileName, cv::FileStorage::WRITE);
	fs << "K" << this->K;
	fs << "D" << this->D;
	fs << "nCornersWidth" << this->nCornersWidth;
	fs << "nCornersHight" << this->nCornersHight;
	fs << "squareSizeMM" << this->squareSizeMM;
	fs << "imageSize" << this->imagesSize;
	std::cout << "Calibration file " + this->calibrationFile + " generated.\n";

	return;
}

void calibrateCamera::calibrate() {
	this->loadCalibrationImages();
	cv::calibrateCamera(this->objectPoints, this->imagePoints, this->imagesSize, 
		this->K, this->D, this->rvecs, this->tvecs, CV_CALIB_FIX_K4 + CV_CALIB_FIX_K5);
	double error = this->reprojectionMSE();
	std::cout << "Calibration error: " + std::to_string(error) + " pixels\n";
	return;
}

void calibrateCamera::undistortFrame(cv::Mat& origFrame, cv::Mat& undistortedFrame) {
	if (this->map1.empty() || this->map2.empty()) {
		cv::Mat optNewK = cv::getOptimalNewCameraMatrix(this->K, this->D, origFrame.size(), 1, origFrame.size(), 0);
		cv::initUndistortRectifyMap(this->K, this->D, cv::Mat(), optNewK, origFrame.size(), CV_16SC2, this->map1, this->map2);
	}
	cv::remap(origFrame, undistortedFrame, this->map1, this->map2, cv::INTER_LINEAR);
}




#include "ImageProcess.h"
using namespace std;

Mat extract(Mat img, int idx) {

	//1. Convert to gray and normalize
	Mat gray(img.rows, img.cols, CV_8UC1);

	cvtColor(img, gray, COLOR_BGR2GRAY);

	// anchor SE의 중심점
	// kernel이 커질수록 뚜렷해지지만 노이즈도 함께 증가
	Mat kernel = getStructuringElement(MORPH_CROSS, cv::Size(7, 7), cv::Point(3, 3));

	// step1 :: erosion
	Mat erosion;
	erode(gray, erosion, kernel);

	// step2 :: opening - dilation after erosion
	Mat opening;
	dilate(erosion, opening, kernel);

	Mat tophat;
	if (idx == 1) {
		cvtColor(opening, opening, COLOR_GRAY2BGR);
		tophat = img - opening;
	}
	else{
		// step3 :: closing - erosion after dilation
		Mat closing;
		erode(opening, closing, kernel);

		// - 연산을 위해 채널 맞추기
		cvtColor(closing, closing, COLOR_GRAY2BGR);
		tophat = img - closing;
	}

	return tophat;
}


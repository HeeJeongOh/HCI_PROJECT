#include "ImageProcess.h"
using namespace std;

Mat extract(Mat img) {
	//2. Convert to gray and normalize
	Mat gray(img.rows, img.cols, CV_8UC1);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	imshow("gray", gray);

	// anchor SE의 중심점
	// kernel이 커질수록 뚜렷해지지만 노이즈도 함께 증가
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, cv::Size(20, 20), cv::Point(1, 1));

	// step1 :: erosion
	Mat erosion;
	erode(gray, erosion, kernel);
	imshow("erosion", erosion);

	// step2 :: opening - dilation after erosion
	Mat opening;
	dilate(erosion, opening, kernel);
	imshow("opening", opening);
	cvtColor(opening, opening, COLOR_GRAY2BGR);

	// step3 :: tophat
	Mat tophat = img - opening;
	imshow("tophat", tophat);

	// img2_fg를 이진화 (4)
	Mat binary_img;
	threshold(tophat, binary_img, 240, 255, THRESH_BINARY_INV);
	imshow("binary_img", binary_img);

	// img2_fg bitwise_not으로 흑백 바꿈 (5) 
	Mat not_img;
	bitwise_not(binary_img, not_img);
	imshow("not_img", not_img);

	Mat black;
	// (2) + (4)를 and 연산 (6) 
	bitwise_and(tophat, binary_img, black); // -> 검정 배경에 이미지만 
	imshow("black", black);

	return 	black;
}

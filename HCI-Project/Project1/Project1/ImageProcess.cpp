#include "ImageProcess.h"
using namespace std;

Mat chooseImage() {
	int k;
	Mat res;
	string path;
	Mat example = imread("image/all.png");
	imshow("Example", example);
	waitKey(1);

	cout << "원하시는 이미지의 번호를 입력해주세요. (그 외 '0' 입력)" << endl;
	cin >> k;
	switch (k) {
	case 0:
		cout << "파일 경로를 입력해 주세요." << endl;
		cin >> path;
		res = imread(path);
		break;
	case 1:
		res = imread("image/1.jpg");
		break;
	case 2:
		res = imread("image/2.jpg");
		break;
	case 3:
		res = imread("image/3.jpg");
		break;
	case 4:
		res = imread("image/4.jpg");
		break;
	case 5:
		res = imread("image/5.jpg");
		break;
	case 6:
		res = imread("image/6.jpg");
		break;
	case 7:
		res = imread("image/7.jpg");
		break;
	case 8:
		res = imread("image/8.jpg");
		break;
	case 9:
		res = imread("image/9.jpg");
		break;
	case 10:
		res = imread("image/10.jpg");
		break;
	default:
		cout << "잘못된 입력입니다." << endl;
	}
	return res;
}


Mat extract(Mat img) {
	//2. Convert to gray and normalize
	Mat gray(img.rows, img.cols, CV_8UC1);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	normalize(gray, gray, 0, 255, NORM_MINMAX, CV_8UC1);
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

#include "ImageProcess.h"
using namespace std;

string chooseImage() {
	int k;
	string res;

	Mat example = imread("image/all.png");
	imshow("Example", example);

	cout << "원하시는 이미지의 번호를 입력해주세요. (그 외 '0' 입력)" << endl;
	cin >> k;
	switch (k) {
	case 0:
		cout << "파일 경로를 입력해 주세요." << endl;
		cin >> res;
	case 1:
		res = "image/1.jpg";
		break;
	case 2:
		res = "image/2.jpg";
		break;
	case 3:
		res = "image/3.jpg";
		break;
	case 4:
		res = "image/4.jpg";
		break;
	case 5:
		res = "image/5.jpg";
		break;
	case 6:
		res = "image/6.jpg";
		break;
	case 7:
		res = "image/7.jpg";
		break;
	case 8:
		res = "image/8.jpg";
		break;
	case 9:
		res = "image/9.jpg";
		break;
	case 10:
		res = "image/10.jpg";
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

	// step3 :: tophat
	Mat tophat = gray - opening;
	imshow("tophat", tophat);

	Mat result;
	cvtColor(tophat, result, COLOR_GRAY2BGR);

	return result;
}

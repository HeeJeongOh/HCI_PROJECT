﻿#include "ImageProcess.h"
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
	// imshow("2. Grayscale", gray);

	//3. Edge detector
	Mat blur;
	GaussianBlur(gray, blur, Size(3, 3), 0, 0, BORDER_DEFAULT);

	Mat edges;
	Canny(blur, edges, 250, 750, 5);
	// imshow("3. Edge Detector", edges);

	//4. Dilate
	Mat dilation;
	int dilateSize = 3;
	Mat elementDilate = getStructuringElement(MORPH_ELLIPSE, Size(2 * dilateSize + 1, 2 * dilateSize + 1), Point(dilateSize, dilateSize));
	dilate(edges, dilation, elementDilate);
	// imshow("4. Dilate", dilation);

	//5. Floodfill
	Mat floodFilled = cv::Mat::zeros(dilation.rows + 2, dilation.cols + 2, CV_8U);
	floodFill(dilation, floodFilled, cv::Point(0, 0), 0, 0, cv::Scalar(), cv::Scalar(), 4 + (255 << 8) + cv::FLOODFILL_MASK_ONLY);
	floodFilled = cv::Scalar::all(255) - floodFilled;
	Mat temp;
	floodFilled(Rect(1, 1, dilation.cols - 2, dilation.rows - 2)).copyTo(temp);
	floodFilled = temp;
	// imshow("5. Floodfill", floodFilled);

	//6. Erode
	int erosionSize = 4;
	Mat erosionElement = getStructuringElement(MORPH_ELLIPSE, Size(2 * erosionSize + 1, 2 * erosionSize + 1), Point(erosionSize, erosionSize));
	erode(floodFilled, floodFilled, erosionElement);
	// imshow("6. Erode", floodFilled);

	//7. Find largest contour
	int largestArea = 0;
	int largestContourIndex = 0;
	Rect boundingRectangle;
	Mat largestContour(img.rows, img.cols, CV_8UC1, Scalar::all(0));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(floodFilled, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		double a = contourArea(contours[i], false);
		if (a > largestArea)
		{
			largestArea = a;
			largestContourIndex = i;
			boundingRectangle = boundingRect(contours[i]);
		}
	}

	Scalar color(255, 255, 255);
	drawContours(largestContour, contours, largestContourIndex, color, FILLED, 8, hierarchy); //Draw the largest contour using previously stored index.
	rectangle(img, boundingRectangle, Scalar(0, 255, 0), 1, 8, 0);
	// imshow("7. Largest Contour", largestContour);

	//8. Mask original image
	Mat maskedSrc;
	img.copyTo(maskedSrc, largestContour);
	imshow("8. Masked Source", maskedSrc);

	Mat mask;
	threshold(gray, mask, 100, 255, THRESH_BINARY);
	imshow("thresh", mask);
	resize(mask, mask, Size(img.cols, img.rows), 0, 0, INTER_LINEAR);

	Mat object = gray - ~mask;
	imshow("object", object);

	Mat result;
	cvtColor(object, result, COLOR_GRAY2BGR);

	return result;
}

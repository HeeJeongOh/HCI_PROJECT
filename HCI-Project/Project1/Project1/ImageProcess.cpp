#include <iostream>
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
		cout << "파일 경로를 입력해 주세요" << endl;
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


Mat sobel(Mat gray);
Mat canny(Mat src);


Mat extract(Mat img) {
	//0. Source Image
	// imshow("img", img);

	//2. Convert to gray and normalize
	Mat gray(img.rows, img.cols, CV_8UC1);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	normalize(gray, gray, 0, 255, NORM_MINMAX, CV_8UC1);
	// imshow("2. Grayscale", gray);

	//3. Edge detector
	GaussianBlur(gray, gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
	Mat edges;
	bool useCanny = false;
	if (useCanny) {
		edges = canny(gray);
	}
	else {
		//Use Sobel filter and thresholding.
		edges = sobel(gray);
		//Automatic thresholding
		//threshold(edges, edges, 0, 255, cv::THRESH_OTSU);
		//Manual thresholding
		threshold(edges, edges, 25, 255, cv::THRESH_BINARY);
	}

	// imshow("3. Edge Detector", edges);

	//4. Dilate
	Mat dilateGrad = edges;
	int dilateType = MORPH_ELLIPSE;
	int dilateSize = 3;
	Mat elementDilate = getStructuringElement(dilateType,
		Size(2 * dilateSize + 1, 2 * dilateSize + 1),
		Point(dilateSize, dilateSize));
	dilate(edges, dilateGrad, elementDilate);
	// imshow("4. Dilate", dilateGrad);

	//5. Floodfill
	Mat floodFilled = cv::Mat::zeros(dilateGrad.rows + 2, dilateGrad.cols + 2, CV_8U);
	floodFill(dilateGrad, floodFilled, cv::Point(0, 0), 0, 0, cv::Scalar(), cv::Scalar(), 4 + (255 << 8) + cv::FLOODFILL_MASK_ONLY);
	floodFilled = cv::Scalar::all(255) - floodFilled;
	Mat temp;
	floodFilled(Rect(1, 1, dilateGrad.cols - 2, dilateGrad.rows - 2)).copyTo(temp);
	floodFilled = temp;
	// imshow("5. Floodfill", floodFilled);

	//6. Erode
	int erosionType = MORPH_ELLIPSE;
	int erosionSize = 4;
	Mat erosionElement = getStructuringElement(erosionType,
		Size(2 * erosionSize + 1, 2 * erosionSize + 1),
		Point(erosionSize, erosionSize));
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

	for (int i = 0; i < contours.size(); i++)
	{
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

	return maskedSrc;
}

Mat sobel(Mat gray) {
	Mat edges;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat edges_x, edges_y;
	Mat abs_edges_x, abs_edges_y;
	Sobel(gray, edges_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(edges_x, abs_edges_x);
	Sobel(gray, edges_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(edges_y, abs_edges_y);
	addWeighted(abs_edges_x, 0.5, abs_edges_y, 0.5, 0, edges);

	return edges;
}

Mat canny(Mat src)
{
	Mat detected_edges;

	int edgeThresh = 1;
	int lowThreshold = 250;
	int highThreshold = 750;
	int kernel_size = 5;
	Canny(src, detected_edges, lowThreshold, highThreshold, kernel_size);

	return detected_edges;
}

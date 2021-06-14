#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

void ROI(Mat background, Mat add_img, Point mid, int x, int y, int idx) {
	// 이미지의 높이, 너비, 채널 수 구하기 
	int width = add_img.cols;				// 크기 줄인 도안 
	int height = add_img.rows;
	
	int backWidth = mid.x;		// original frame 
	int backHeight = mid.y;

	Mat origin = add_img;

	// 배경 이미지의 Region Of Interest 구하기
	if (0 < backWidth
		&& 0 < width
		&& backWidth + width / 2+x <= background.cols
		&& 0 < backHeight
		&& 0 < height
		&& backHeight + height / 2-y <= background.rows) { // 피부가 인식 되었을 때 
		Mat roi = background(Rect(backWidth - width / 2+x, backHeight - height / 2-y, width, height));

		// 배경 이미지의 ROI영역과 워터마크 이미지의 워터마크 영역을 블렌딩
		if (idx == 0)
			addWeighted(roi, 1, origin, -1, 0, roi);
		else {
			// 대비 높이기

			addWeighted(roi, 1, origin, 0.7, 0.1, roi); // -> 컬러이미지일 때는 주석 풀기 
		}

		imshow("result", background);
	}
	else {	// 피부가 인식 되지 않았을 때 
		imshow("result", background);
	}

}

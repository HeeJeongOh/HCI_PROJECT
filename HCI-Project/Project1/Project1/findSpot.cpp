#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat ROI(Mat back, Mat img, Point p){
    // 이미지의 높이, 너비, 채널 수 구하기 
    int width = img.cols;            // 크기 줄인 도안 
    int height = img.rows;

    int backWidth = p.x;      // original frame 
    int backHeight = p.y;

    // 배경 이미지의 Region Of Interest 구하기
    if (0 < backWidth
        && 0 < width
        && backWidth + width / 2 <= back.cols
        && 0 < backHeight
        && 0 < height
        && backHeight + height / 2 <= back.rows) { // 피부가 인식 되었을 때 
        Mat roi = back(Rect(backWidth - width / 2, backHeight - height / 2, width, height));

        Mat gray_img; // (3)
        cvtColor(img, gray_img, COLOR_RGB2GRAY);

        // img2_fg를 이진화 (4)
        Mat binary_img;
        threshold(gray_img, binary_img, 50, 255, THRESH_BINARY_INV);
        imshow("binary_img", binary_img);

        // img bitwise_not으로 흑백 바꿈 (5) 
        Mat not_img;
        bitwise_not(binary_img, not_img);
        imshow("not_img", not_img);

        // gray_img & not_img -> gray_binary
        Mat gray_binary;
        bitwise_and(gray_img, not_img, gray_binary); // -> 검정 배경에 이미지만 
        imshow("gray_binary", gray_binary);

        cvtColor(gray_binary, gray_binary, COLOR_GRAY2BGR);
        addWeighted(roi, 1, gray_binary, 1, 0, roi);
        // add(roi, img2_fg, back); // 빵꾸뚫린 부분에 추출 이미지 add 
        // img2_fg.copyTo(roi);
        imshow("result11", back);
    }
    else {   // 피부가 인식 되지 않았을 때 

        imshow("result22", back);
    }

    return back;
}

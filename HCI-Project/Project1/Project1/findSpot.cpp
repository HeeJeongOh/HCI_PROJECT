#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat ROI(Mat back, Mat img, Point p) {
    // 이미지의 높이, 너비, 채널 수 구하기 
    int width = img.cols;            // 크기 줄인 도안 
    int height = img.rows;

    int backWidth = p.x;      // original frame 
    int backHeight = p.y;

    Mat origin = img;

    // 배경 이미지의 Region Of Interest 구하기
    if (0 < backWidth
        && 0 < width
        && backWidth + width / 2 <= back.cols
        && 0 < backHeight
        && 0 < height
        && backHeight + height / 2 <= back.rows) { // 피부가 인식 되었을 때 

        Mat roi = back(Rect(backWidth - width / 2, backHeight - height / 2, width, height));
        
        // 배경 이미지의 ROI영역과 워터마크 이미지의 워터마크 영역을 블렌딩

        // 그 이미지랑 original img 비트연산 
        //bitwise_and(roi, img2_fg, back);
        //bitwise_not(back,back);

        /*
        // img2_fg를 흑백이미지로 (2)
        Mat black = imread("image/black.png");
        resize(black, black, Size(back.cols, back.rows));
        Mat blackroi = black(Rect(backWidth - width / 2, backHeight - height / 2, width, height));
        addWeighted(blackroi, 1, img2_fg, 1, 0, blackroi);
        // add(blackroi, img2_fg, img2_fg);
        imshow("(0)", black);
        //img2_fg = black;

        Mat gray_img; // (3)
        cvtColor(img2_fg, gray_img, COLOR_RGB2GRAY);

        // img2_fg를 이진화 (4)
        Mat binary_img;
        threshold(gray_img, binary_img, 240, 255, THRESH_BINARY_INV);
        imshow("(4)",binary_img);

        // img2_fg bitwise_not으로 흑백 바꿈 (5) 
        Mat notimg;
        bitwise_not(binary_img, notimg);
        imshow("(5)", notimg);

        // (2) + (4)를 and 연산 (6) 
        bitwise_and(gray_img, binary_img, img2_fg); // -> 검정 배경에 이미지만 
        imshow("(6)", img2_fg);

        // roi과 (5)를 and연산 --> 빵꾸뚫린 부분  
        cvtColor(img2_fg, img2_fg, COLOR_GRAY2BGR);
        cvtColor(notimg, notimg, COLOR_GRAY2BGR);
        bitwise_and(roi, notimg, roi); // -> extract 이미지 빵꾸뚫린거 + 피부영역 
        imshow("roi", back);
        
        
        // roi만큼 이미지 빼기 ?? 
        */
        
        addWeighted(roi, 1, origin, -1, 0, roi);
        // addWeighted(roi, 1, origin, 1, 0, roi); // -> 컬러이미지일 때는 주석 풀기 
        

        imshow("result", back);
    }
    else {   // 피부가 인식 되지 않았을 때 
        imshow("result", back);
    }

    return back;
}

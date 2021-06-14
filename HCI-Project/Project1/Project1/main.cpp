#define CVUI_IMPLEMENTATION
#define CVUI_DISABLE_COMPILATION_NOTICES
#include "cvui.h"

#include <iostream>
#include "ImageProcess.h"
#include "findSpot.h"
#define FilePath "image/mask.png"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

#define CAM_WIDTH 600
#define CAM_HEIGHT 400

#define WINDOW_NAME "CVUI Test"
using namespace cv;

int main(int argc, char** argv)
{

    cvui::init(WINDOW_NAME);

    Mat frame = Mat(cv::Size(650, 700), CV_8UC3);
    double x_val = 0;
    double y_val = 0;
    double x_size = 0;
    double y_size = 0;
    double angle = 0;
    double num;
    int idx=0;
    Mat example1 = imread("image/1.jpg");
    resize(example1, example1, Size(100, 100));

    Mat example2 = imread("image/2.jpg");
    resize(example2, example2, Size(100, 100));

    Mat example3 = imread("image/3.jpg");
    resize(example3, example3, Size(100, 100));

    Mat example4 = imread("image/4.jpg");
    resize(example4, example4, Size(100, 100));

    Mat example5 = imread("image/5.jpg");
    resize(example5, example5, Size(100, 100));

    Mat example6 = imread("image/6.jpg");
    resize(example6, example6, Size(100, 100));

    Mat example7 = imread("image/7.jpg");
    resize(example7, example7, Size(100, 100));

    Mat example8 = imread("image/8.png");
    resize(example8, example8, Size(100, 100));

    Mat example9 = imread("image/9.jpg");
    resize(example9, example9, Size(100, 100));

    Mat example10 = imread("image/10.jpg");
    resize(example10, example10, Size(100, 100));

    //imshow("Example", example);
    waitKey(1);

    Mat blendImg = imread("image/1.jpg"); // 적용할 이미지 


    // 비디오 이미지 불러오기 
    VideoCapture cam(0);
    // 원래의 오리지널 이미지 
    cam.set(CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
    cam.set(CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
    Mat Origin_frame;
    Mat HAND_MASK = imread(FilePath, 1);

    if (!cam.isOpened()) { printf("--(!)Error opening video cam\n"); return -1; }

    while (cam.read(Origin_frame))
    {

        if (Origin_frame.empty()) {
            printf("--(!) No cam frame -- Break!");
            break;
        }


        // 크기 조정과 위치 조정을 진행할 프레임 
        frame = Scalar(150, 150, 150);

        cvui::text(frame, 50, 10, "Adjust X-axis center point");
        cvui::trackbar(frame, 100, 20, 400, &x_val, (double)-100, (double)100, 10, "%0.1f");

        cvui::text(frame, 50, 90, "Adjust Y-axis center point");
        cvui::trackbar(frame, 100, 100, 400, &y_val, (double)-100, (double)100, 10, "%0.1f");

        cvui::text(frame, 50, 170, "Adjust column size");
        cvui::trackbar(frame, 100, 180, 400, &x_size, (double)-100, (double)100, 10, "%0.Lf");

        cvui::text(frame, 50, 250, "Adjust row size");
        cvui::trackbar(frame, 100, 260, 400, &y_size, (double)-100, (double)100, 10, "%0.Lf");

        if (cvui::button(frame, 50, 410, example1, example1, example1)) {
            //blendImg = imread("image/1.jpg");
            blendImg = example1;
            idx = 0;
        }
        if (cvui::button(frame, 160, 410, example2, example2, example2)) {
            blendImg = example2;
            idx = 0;
        }
        if (cvui::button(frame, 270, 410, example3, example3, example3)) {
            blendImg = example3;
            idx = 0;
        }
        if (cvui::button(frame, 380, 410, example4, example4, example4)) {
            blendImg = example4;
            idx = 0;
        }
        if (cvui::button(frame, 490, 410, example5, example5, example5)) {
            blendImg = example5;
            idx = 0;
        }
        if (cvui::button(frame, 50, 540, example6, example6, example6)) {
            blendImg = example6;
            idx = 1;
        }
        if (cvui::button(frame, 160, 540, example7, example7, example7)) {
            blendImg = example7;
            idx = 1;
        }
        if (cvui::button(frame, 270, 540, example8, example8, example8)) {
            blendImg = example8;
            idx = 1;
        }
        if (cvui::button(frame, 380, 540, example9, example9, example9)) {
            blendImg = example9;
            idx = 1;
        }
        if (cvui::button(frame, 490, 540, example10, example10, example10)) {
            blendImg = example10;
            idx = 1;
        }
        cvui::imshow(WINDOW_NAME, frame);


        // 마스크를 이용한 전처리
        resize(HAND_MASK, HAND_MASK, Size(Origin_frame.cols, Origin_frame.rows), 0, 0, INTER_LINEAR);

        Mat Mask = Origin_frame + HAND_MASK;

        // 컬러 모델 적용 
        Mat YCrCb;
        cvtColor(Mask, YCrCb, COLOR_BGR2YCrCb);

        // 피부 부분 추출 
        Mat Skin_Area;
        inRange(YCrCb, Scalar(0, 130, 90), Scalar(255, 168, 130), Skin_Area);

        // 중심점 찾기 
        Moments m = moments(Skin_Area, true);
        Point p(m.m10 / m.m00, m.m01 / m.m00); // 피부 추출한 부분의 중심점 

        // 합성할 이미지의 크기를 200*200으로 초기화하기 
        Mat img_small;
        if (100 + x_size>0 && 100 + y_size > 0) {
            resize(blendImg, img_small, Size(100 + x_size, 100 + y_size));   // 이미지 크기 줄이기
        }
        else {
            resize(blendImg, img_small, Size(1, 1));
        }

     
        // 크기 줄인 도안과 그것의 워터마크 부분만 추출한 도안 반환 
        Mat add_img = extract(img_small, idx);   

        // 배경 이미지의 Region Of Interest 구하기
        ROI(Origin_frame, add_img, p, x_val, y_val, idx);


        if (cv::waitKey(20) == 27)
            break;
    }

    return 0;
}

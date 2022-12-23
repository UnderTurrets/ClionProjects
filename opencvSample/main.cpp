#include <iostream>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

class LightDescriptor
{	    //在识别以及匹配到灯条的功能中需要用到旋转矩形的长宽偏转角面积中心点坐标等
public:float width, length, angle, area;
    cv::Point2f center;
public:
    LightDescriptor() {};
    //让得到的灯条套上一个旋转矩形，以方便之后对角度这个特殊因素作为匹配标准
    LightDescriptor(const cv::RotatedRect& light)
    {
        width = light.size.width;
        length = light.size.height;
        center = light.center;
        angle = light.angle;
        area = light.size.area();
    }
};

int main()
{
    VideoCapture video(0); //VC类对象化
    // video.open("");
    //变量集中定义
    Mat frame, channels[3], binary, Gaussian, dilatee;
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    Rect boundRect;
    RotatedRect box;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Point2f> boxPts(4);
    //图像预处理
    for (;;) {
        Rect point_array[20];
        video >> frame;  //读取每帧
        if (frame.empty()) {
            break;
        }
        split(frame, channels); //通道分离
        threshold(channels[0], binary, 220, 255, 0);//二值化
        GaussianBlur(binary, Gaussian, Size(5, 5), 0);//滤波
        dilate(Gaussian, dilatee, element);
        // dilate(Gaussian, dilate, element, Point(-1, -1));//膨胀，把滤波得到的细灯条变宽
        findContours(dilatee, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);//轮廓检测
        vector<LightDescriptor> lightInfos;//创建一个灯条类的动态数组
        //筛选灯条
        for (int i = 0; i < contours.size(); i++) {
            // 求轮廓面积
            double area = contourArea(contours[i]);
            // 去除较小轮廓&fitEllipse的限制条件
            if (area < 5 || contours[i].size() <= 1)
                continue;//相当于就是把这段轮廓去除掉
            // 用椭圆拟合区域得到外接矩形（特殊的处理方式：因为灯条是椭圆型的，所以用椭圆去拟合轮廓，再直接获取旋转外接矩形即可）
            RotatedRect Light_Rec = fitEllipse(contours[i]);

            // 长宽比和轮廓面积比限制（由于要考虑灯条的远近都被识别到，所以只需要看比例即可）
            if (Light_Rec.size.width / Light_Rec.size.height > 4)
                continue;
            lightInfos.push_back(LightDescriptor(Light_Rec));
        }
        //二重循环多条件匹配灯条
        for (size_t i = 0; i < lightInfos.size(); i++) {
            for (size_t j = i + 1; (j < lightInfos.size()); j++) {
                LightDescriptor& leftLight = lightInfos[i];
                LightDescriptor& rightLight = lightInfos[j];
                float angleGap_ = abs(leftLight.angle - rightLight.angle);
                //由于灯条长度会因为远近而受到影响，所以按照比值去匹配灯条
                float LenGap_ratio = abs(leftLight.length - rightLight.length) / max(leftLight.length, rightLight.length);
                float dis = pow(pow((leftLight.center.x - rightLight.center.x), 2) + pow((leftLight.center.y - rightLight.center.y), 2), 0.5);
                //均长
                float meanLen = (leftLight.length + rightLight.length) / 2;
                float lengap_ratio = abs(leftLight.length - rightLight.length) / meanLen;
                float yGap = abs(leftLight.center.y - rightLight.center.y);
                float yGap_ratio = yGap / meanLen;
                float xGap = abs(leftLight.center.x - rightLight.center.x);
                float xGap_ratio = xGap / meanLen;
                float ratio = dis / meanLen;
                //匹配不通过的条件
                if (angleGap_ > 15 ||
                    LenGap_ratio > 1.0 ||
                    lengap_ratio > 0.8 ||
                    yGap_ratio > 1.5 ||
                    xGap_ratio > 2.2 ||
                    xGap_ratio < 0.8 ||
                    ratio > 3 ||
                    ratio < 0.8) {
                    continue;
                }
                //绘制矩形
                Point center = Point((leftLight.center.x + rightLight.center.x) / 2, (leftLight.center.y + rightLight.center.y) / 2);
                RotatedRect rect = RotatedRect(center, Size(dis, meanLen), (leftLight.angle + rightLight.angle) / 2);
                Point2f vertices[4];
                rect.points(vertices);
                for (int i = 0; i < 4; i++) {
                    line(frame, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2.2);
                }
            }
        }

        namedWindow("video", WINDOW_FREERATIO);
        imshow("video", frame);
        waitKey(1);
    }
    video.release();
    cv::destroyAllWindows();
    return 0;
}

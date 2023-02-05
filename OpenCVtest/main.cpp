#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void PrintMs ( ){
    static short i =1;static long long last =0;
    long long current=cv::getTickCount();
    if(i%2==1){
        last =0;
        cout<<"开始计时"<<endl;
    }else{
        i-=2;
        cout<<"结束计时 ";
    }
    if (last==0){
        last = current;
        i++;
        return;
    }
    double result = (double)((current-last)/cv::getTickFrequency())*1000;
    i++;
    cout<<"间隔："<<result<<"ms"<<endl;
}


void drawRotatedRect(InputOutputArray img, RotatedRect rRect,const Scalar& color, int thickness = 1,int lineType = LINE_8, int shift = 0){
    Point2f vertices[4];
    rRect.points(vertices);
    for(int i=0;i<4;i++){
        line(img,vertices[i],vertices[(i+1)%4],color,lineType,shift);
    }
}


class TrackbarUserdata{
public:
    Mat input;
    Mat output;
    int angle=0;
    string winname;
};


void RotateonChange(int,void *userdata) {

    TrackbarUserdata *data = (TrackbarUserdata *) userdata;
    int rows = data->input.rows;
    int cols = data->output.cols;
    Mat M = getRotationMatrix2D(Point2f(rows / 2, cols / 2), data->angle, 1);
    warpAffine(data->input,data->output,M,data->input.size());
    imshow(data->winname,data->output);
    waitKey(10);
}

void addSaltNoise(const Mat& src,Mat& dst,int num=1000){
    int total=src.cols*src.rows;
    if(num>total){
        cout<<"The inputarray doesn't have enough pixel."<<endl;
        return;
    }
    dst=src.clone();
    for (int k = 0; k < num; k++)
    {
        //随机取值行列，得到像素点(i,j)
        int i = rand() % dst.rows;
        int j = rand() % dst.cols;
        //修改像素点(i,j)的像素值
        for(int channel=0;channel<src.channels();channel++){
            dst.ptr(i,j)[channel]=255;
        }
    }
    for (int k = 0; k < num; k++)
    {
        //随机取值行列
        int i = rand() % dst.rows;
        int j = rand() % dst.cols;
        //修改像素点(i,j)的像素值
        for(int channel=0;channel<src.channels();channel++){
            dst.ptr(i,j)[channel]=0;
        }
    }
    return;
}

void addGaussianNoise(const Mat& src,Mat& dst,InputArray meanValue=10,InputArray std=36){
    dst=src.clone();

    //构造高斯噪声矩阵
    Mat noise(dst.size(),dst.type());
    RNG rng(time(NULL));
    rng.fill(noise, RNG::NORMAL, meanValue, std);

    //将高斯噪声矩阵与原图像叠加得到含噪图像
    dst+=noise;

    return ;
}


int main() {
    try {
        Mat xuenai = imread("xuenai.jpg");
        resize(xuenai,xuenai,Size(1000,1000));
        imshow("xuenai",xuenai);
        Mat xuenai_Salt;
        addSaltNoise(xuenai,xuenai_Salt,2000);
        imshow("xuenai_salt",xuenai_Salt);
        Mat xuenai_gauss;
        addGaussianNoise(xuenai,xuenai_gauss);
        imshow("xuenai_Gauss",xuenai_gauss);

//    Mat xuenai_canny(xuenai.size(),xuenai.type());
//    Canny(xuenai,xuenai_canny,60,150);
//    imshow("xuenai_canny",xuenai_canny);

//    Mat xuenai_rotate(xuenai.size(), xuenai.type());
//    TrackbarUserdata userdata;
//    userdata.input=xuenai;
//    userdata.output=xuenai_rotate;
//    userdata.winname="xuenai_rotate";
//    namedWindow(userdata.winname);
//    createTrackbar("angle",userdata.winname, &userdata.angle,360, RotateonChange,&userdata);

//    namedWindow("xuenai_rotate");
//    createTrackbar("angle","xuenai_rotate", nullptr,360);
//    while (1) {
//        int angle= getTrackbarPos("angle","xuenai_rotate");
//        Mat M = getRotationMatrix2D(Point2f(xuenai.cols / 2, xuenai.rows / 2), angle, 1);
//        warpAffine(xuenai, xuenai_rotate, M, xuenai.size());
//        imshow("xuenai_rotate",xuenai_rotate);
//        waitKey(20);
//    }

//    Mat xuenai_gray(xuenai.size(),xuenai.type());
//    cvtColor(xuenai,xuenai_gray,COLOR_BGR2GRAY);
//    Mat xuenai_threshold(xuenai.size(),xuenai.type());
//    threshold(xuenai_gray,xuenai_threshold,100,255,THRESH_BINARY);
//    imshow("xuenai_threshold",xuenai_threshold);


//    Mat kernal=getStructuringElement(MORPH_RECT,Size(3,3));
//
//    Mat xuenai_morphology(xuenai.size(),xuenai.type());
//    morphologyEx(xuenai_threshold,xuenai_morphology,MORPH_GRADIENT,kernal);
//    imshow("xuenai_morphology",xuenai_morphology);

//    Mat xuenai_dilate(xuenai.size(),xuenai.type());
//    dilate(xuenai_threshold,xuenai_dilate,kernal);
//    imshow("xuenai_dilate",xuenai_dilate);

//    Mat xuenai_bilateral(xuenai.size(),xuenai.type());
//    bilateralFilter(xuenai,xuenai_bilateral,-1,100,10);
//    imshow("xuenai_bilateral",xuenai_bilateral);

//    Mat xuenai_median(xuenai.size(),xuenai.type());
//    medianBlur(xuenai,xuenai_median,5);
//    imshow("xuenai_median",xuenai_median);


//    Mat xuenai_Gauss(xuenai.size(),xuenai.type());
//    GaussianBlur(xuenai,xuenai_Gauss,Size(-1,-1),10);
//    imshow("xuenai_Gauss",xuenai_Gauss);


//        Mat xuenai_flip(xuenai.size(), xuenai.type());
//        flip(xuenai, xuenai_flip, 0);
//        imshow("xuenai_flip", xuenai_flip);
//
//        Mat xuenai_rotate(xuenai.size(), xuenai.type());
//        rotate(xuenai, xuenai_rotate, ROTATE_180);
//        imshow("xuenai_rotate", xuenai_rotate);

//        Mat transformer=getRotationMatrix2D(Point2f (xuenai.cols/2,xuenai.rows/2),90,1);
//        Mat xuenai_flap;
//        warpAffine(xuenai,xuenai_flap,transformer,xuenai.size());
//        imshow("xuenai_flap",xuenai_flap);

//        Mat xuenai_sobel1(xuenai.size(),xuenai.type());
//        Sobel(xuenai_gray,xuenai_sobel1,CV_16S,1,1,3);
//        convertScaleAbs(xuenai_sobel1,xuenai_sobel1);
//        imshow("xuenai_sobel1",xuenai_sobel1);
//
//        Mat xuenai_xsobel(xuenai.size(),xuenai.type());Mat xuenai_ysobel(xuenai.size(),xuenai.type());Mat xuenai_sobel2(xuenai.size(),xuenai.type());
//        Sobel(xuenai_gray,xuenai_xsobel,CV_16S,1,0,3);
//        convertScaleAbs(xuenai_xsobel,xuenai_xsobel);
//        Sobel(xuenai_gray,xuenai_ysobel,CV_16S,0,1,3);
//        convertScaleAbs(xuenai_ysobel,xuenai_ysobel);
//        addWeighted(xuenai_xsobel,0.5,xuenai_ysobel,0.5,0,xuenai_sobel2);
//        convertScaleAbs(xuenai_sobel2,xuenai_sobel2);
//        imshow("xuenai_sobel2",xuenai_sobel2);


//  Mat a(1440,2560,CV_8UC3);
//
//  for(int i=0;i<a.rows;i++){
//      for(int j=0;j<a.cols;j++){
//          a.ptr(i,j)[0]=0;
//          a.ptr(i,j)[1]=0;
//          a.ptr(i,j)[2]=255;
//      }
//  }
//  for(auto iter=a.begin<Vec3b>();iter!=a.end<Vec3b>();iter++){
//      iter[0]=255;
//      iter[1]=0;
//      iter[2]=0;
//  }


//    for(int i=0;i<xuenai.rows;i++){
//        for(int j=0;j<xuenai.cols;j++){
//            for(int k=0;k<xuenai.channels();k++) {
//                xuenai.at<Vec3b>(i, j)[k] = saturate_cast<uchar>(xuenai.at<Vec3b>(i, j)[k] * 1.2 + 30);
//            }
//        }
//    }
//    xuenai.convertTo(xuenai,-1,1.2,30);
//
//    imshow("xuenai_convertTo",xuenai);



//       Mat tuanzi(xuenai,(Rect(0,0,500,1000)));
//        imshow("tuanzi",tuanzi);


//        Mat xuenai_pyrup(xuenai.size(),xuenai.type());
//        pyrUp(xuenai,xuenai_pyrup);
//        imshow("xuenai_pyrup",xuenai_pyrup);


//        Mat canvas(Size(1000,1000),CV_8U,Scalar(255));
//        ellipse(canvas,Point2f(500,500),Size(50,100),20,0,180,Scalar(0,0,0),5);
//        RotatedRect rREct(Point2f(100,100),Size(100,100),20);
//        drawRotatedRect(canvas,rREct,Scalar(0));


//        float  M_values[]={1,0,200,
//                          0,1,200};
//        Mat M(Size(3,2), CV_32F,M_values);
//        Mat xuenai_shift(xuenai.size(),xuenai.type());
//        warpAffine(xuenai,xuenai_shift,M,xuenai.size());
//        imshow("xuenai_shift",xuenai_shift);

//        Mat xuenai_blur(xuenai.size(),xuenai.type());
//        blur(xuenai,xuenai_blur,Size(3,5));
//        imshow("xuenai_blur",xuenai_blur);









    }catch (exception &error){
        cout<<error.what()<<endl;
    }
    if(waitKey(0)=='q')return 0;
}

#include <OpenCVLibrary.h>

int main() {
    try {
        Mat xuenai = imread("xuenai.jpg");
        Mat xuenai_rect = imread("xuenai_rect.jpg");
        Mat xuenai_rect_gray;
        cvtColor(xuenai_rect,xuenai_rect_gray,COLOR_BGR2GRAY);
        imshow("xuenai", xuenai);
        namedWindow("panel");
        createTrackbar("threshold","panel", nullptr,255);
        createTrackbar("angle","panel", nullptr,360);
        createTrackbar("width","panel", nullptr,1000);
        createTrackbar("height","panel", nullptr,1000);

        while (true) {
            int thres = getTrackbarPos("threshold", "panel");
            if(thres==0)thres=100;
            int width = getTrackbarPos("width", "panel");
            if(width==0)width=xuenai.cols;
            int height = getTrackbarPos("height", "panel");
            if(height==0)height=xuenai.rows;
            int angle = getTrackbarPos("angle","panel");

            Mat xuenai_transform=xuenai.clone();

            resize(xuenai_transform,xuenai_transform,Size(width,height));

            Mat M= getRotationMatrix2D(Point2f((float )xuenai_transform.cols/2,(float )xuenai_transform.rows/2),angle,1);
            warpAffine(xuenai_transform,xuenai_transform,M,xuenai_transform.size());

            Mat xuenai_gray(xuenai.size(),xuenai.type());
            cvtColor(xuenai_transform,xuenai_gray,COLOR_BGR2GRAY);

            //准备工作
            Ptr<ORB>obr=ORB::create();
            vector<KeyPoint>xuenai_ObrKp;
            Mat BFMmatch_result;Mat FLANNmatch_result;
            vector<KeyPoint>xuenai_rect_ObrKp;
            Mat xuenai_obr_descriptorsForBF;Mat xuenai_rect_obr_descriptorsForBF;Mat xuenai_obr_descriptorsForFLANN;Mat xuenai_rect_obr_descriptorsForFLANN;
            vector<vector<DMatch>>xuenai_BFMmatch_results;vector<vector<DMatch>>xuenai_FLANNmatch_results;
            obr->detectAndCompute(xuenai_gray,noArray(),xuenai_ObrKp,xuenai_obr_descriptorsForBF);
            obr->detectAndCompute(xuenai_rect_gray,noArray(),xuenai_rect_ObrKp,xuenai_rect_obr_descriptorsForBF);
            xuenai_obr_descriptorsForBF.convertTo(xuenai_obr_descriptorsForFLANN,CV_32F);
            xuenai_rect_obr_descriptorsForBF.convertTo(xuenai_rect_obr_descriptorsForFLANN,CV_32F);

            //进行匹配
            Ptr<BFMatcher>bfm=BFMatcher::create(NORM_HAMMING);
            Ptr<FlannBasedMatcher>flann=FlannBasedMatcher::create();
            bfm->knnMatch(xuenai_rect_obr_descriptorsForBF,xuenai_obr_descriptorsForBF,xuenai_BFMmatch_results,2);
            flann->knnMatch(xuenai_rect_obr_descriptorsForFLANN,xuenai_obr_descriptorsForFLANN,xuenai_FLANNmatch_results,2);

            //比率检验
            vector<DMatch>goodBFMresult,goodFLANNresult;
            for(auto match_result:xuenai_BFMmatch_results){
                if(match_result.size()>1 && match_result[0].distance<0.7*match_result[1].distance ){
                    goodBFMresult.push_back(match_result[0]);
                }
            }
            for(auto match_result:xuenai_FLANNmatch_results){
                if(match_result.size()>1 && match_result[0].distance<0.7*match_result[1].distance ){
                    goodFLANNresult.push_back(match_result[0]);
                }
            }

            //绘制匹配结果
            if(!goodBFMresult.empty()) {
                drawMatches(xuenai_rect, xuenai_rect_ObrKp,
                            xuenai_transform, xuenai_ObrKp,
                            goodBFMresult, BFMmatch_result,
                            Scalar::all(-1), Scalar::all(-1), vector<char>(),
                            DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                imshow("BFMmatch_result",BFMmatch_result);

            }
            if(!goodFLANNresult.empty()) {
                drawMatches(xuenai_rect, xuenai_rect_ObrKp,
                            xuenai_transform, xuenai_ObrKp,
                            goodFLANNresult, FLANNmatch_result,
                            Scalar::all(-1), Scalar::all(-1), vector<char>(),
                            DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
                imshow("FLANNmatch_result",FLANNmatch_result);
            }


//            Mat fast_result=xuenai_transform.clone(),obr_result=xuenai_transform.clone();
//            drawKeypoints(fast_result,fast_kp,fast_result,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//            drawKeypoints(obr_result,xuenai_ObrKp,obr_result,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//            imshow("fast_result",fast_result);
//            imshow("obr_result",obr_result);




//            Ptr<SIFT> sift=SIFT::create();
//            Ptr<SURF> surf=SURF::create();
//            vector<KeyPoint>sift_kp,surf_kp;
//            sift->detect(xuenai_gray,sift_kp);
//            surf->detect(xuenai_gray,surf_kp);
//
//            Mat sift_result=xuenai_transform.clone(),surf_result=xuenai_transform.clone();
//            drawKeypoints(sift_result,sift_kp,sift_result,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//            drawKeypoints(surf_result,surf_kp,surf_result,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//            imshow("sift_result",sift_result);
//            imshow("surf_result",surf_result);







//            vector<Point2f>xuenai_cornersSet;
//            goodFeaturesToTrack(xuenai_gray,xuenai_cornersSet,0,0.1,10);
//            for(auto corner:xuenai_cornersSet){
//                circle(xuenai_transform,corner,3,Scalar(0,0,255),-1);
//            }

            //cornerHarris(xuenai_gray,xuenai_corners,2,3,0.04);
//            normalize(xuenai_corners,xuenai_corners,0,255,NORM_MINMAX,-1);
//            convertScaleAbs(xuenai_corners,xuenai_corners);
//            Mat harris_result=xuenai_transform.clone();
//            for(int i=0;i<xuenai_corners.rows;i++){
//                uchar * ptr =xuenai_corners.ptr(i);
//                for(int j=0;j<xuenai_corners.cols;j++){
//                    int value=(int) *ptr;
//                    if(value>thres){
//                        circle(harris_result, Point(j,i), 3, Scalar(0, 0, 255));
//                    }
//                    ptr++;
//                }
//            }




//            imshow("xuenai_corners",xuenai_transform);
            if (waitKey(0) == 'q')break;
        }


//        for(int i=0;i<xuenai.cols;i++){
//            for(int j=0;j<xuenai.rows;j++){
//                if(xuenai_corners.ptr(i,j)[0]>100){
//                    circle(xuenai, Point(i,j), 3, Scalar(0, 0, 255));
//                }
//            }
//        }
//        imshow("xuenai_corners", xuenai);












//        rotate(xuenai,xuenai,ROTATE_90_CLOCKWISE);
//        resize(xuenai,xuenai,Size(500,500));
//        Mat templ= imread("xuenai_rect.jpg");
//        imshow("template",templ);
//        Mat match_result;
//        matchTemplate(xuenai,templ,match_result,TM_SQDIFF);
//        Point temLoc;
//        Point minLoc;
//        Point maxLoc;
//        double min,max;
//        minMaxLoc(match_result,&min,&max,&minLoc,&maxLoc);
//        temLoc=minLoc;
//        rectangle(xuenai,Rect(temLoc.x,temLoc.y,templ.cols,templ.rows),Scalar(0,0,255));
//        imshow("xuenai_match",xuenai);







//        Mat xuenai_rect(xuenai,Rect(Point(0,100),Point(400,600)));
//        if(imread("xuenai_rect.jpg").empty())imwrite("xuenai_rect.jpg",xuenai_rect);

//        Mat xuenai_Salt;
//        addSaltNoise(xuenai,xuenai_Salt,2000);
//        imshow("xuenai_salt",xuenai_Salt);
//        Mat xuenai_gauss;
//        addGaussianNoise(xuenai,xuenai_gauss);
//        imshow("xuenai_Gauss",xuenai_gauss);

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

//
// Created by 94097 on 2024/10/18.
//

#ifndef SEPARATION_H
#define SEPARATION_H

#endif //SEPARATION_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
#include "QuickDemo.h"

using namespace cv;
using namespace std;

class Separation_chessboard {
private:
    vector<String> input_file;
    vector<Mat> image_list;
    Mat max_value_matrix;
    Mat min_value_matrix;
public:
    bool separation(const string &folder,const int W, const int H,const int M,const double b,const bool show) {
        Mat img;
        Mat directionComponent = Mat::zeros(H,W,CV_8UC1);
        Mat globalComponent = Mat::zeros(H,W,CV_8UC1);
        glob(folder, input_file);
        for (auto &file: input_file) {
            imread2(file,img,0);
            resize(img,img,Size(W,H));
            image_list.push_back(img);
        }
        Mat max_value_matrix = Mat::zeros(H,W,CV_8UC1);
        Mat min_value_matrix = Mat::zeros(H,W,CV_8UC1);
        double Ld = 0 ,Lg = 0;
        for(int row=0;row<H;row++){
            for(int col=0;col<W;col++) {
                int max_value = 0;
                int min_value = 255;
                for (int i = 0;i < M; i++) {
                    if(image_list[i].at<uchar>(row,col)>max_value) {
                        max_value= image_list[i].at<uchar>(row,col);
                    }
                    if (image_list[i].at<uchar>(row,col)<min_value) {
                        min_value= image_list[i].at<uchar>(row,col);
                    }
                    max_value_matrix.at<uchar>(row,col) = max_value;
                    min_value_matrix.at<uchar>(row,col) = min_value;
                }
                Ld = (max_value - min_value)/(1-b);
                Lg = 2*(max_value - Ld)/(1+b);
                directionComponent.at<uchar>(row,col) = Ld;
                globalComponent.at<uchar>(row,col) = Lg;
            }
        }
        imwrite("./data/output/chessboard_sep/max_value_matrix.bmp",max_value_matrix);
        imwrite("./data/output/chessboard_sep/min_value_matrix.bmp",min_value_matrix);
        imwrite("./data/output/chessboard_sep/directionComponent.bmp",directionComponent);
        imwrite("./data/output/chessboard_sep/globalComponent.bmp",globalComponent);
        if(show) {
            imshow("max_value_matrix",max_value_matrix);
            imshow("min_value_matrix",min_value_matrix);
            imshow("directionComponent",directionComponent);
            imshow("globalComponent",globalComponent);
            waitKey(0);
            destroyAllWindows();
        }
        return true;
    }

    bool separationByEnergy(const string &folder,const int W, const int H,const int M,const int N,const double b) {
        vector<float> coeffientsList;
        Mat global_fusion = Mat::zeros(H,W,CV_8UC1);
        Mat img;
        Mat directionComponent = Mat::zeros(H,W,CV_8UC1);
        Mat globalComponent = Mat::zeros(H,W,CV_8UC1);
        // 打开文件进行读取
        std::ifstream inFile("./data/pattern_ideal/compound/ratio_list.txt");
        // 检查文件是否成功打开
        if (!inFile.is_open()) {
            std::cerr << "无法打开文件进行读取" << std::endl;
        }
        // 逐行读取文件内容并打印到控制台
        std::string line;
        while (getline(inFile, line)) {
            std::cout << line << std::endl;
            float num = stof(line);
            coeffientsList.push_back(num);
        }
        // 关闭文件
        inFile.close();

        // for (int y = 12;y < 20;y++) {
        //     string img_index = "D:/LIUYUFEI/code/PhaseShiftComplementary/data/24-12-17/" + to_string(y)+".bmp";
        //     imread2(img_index,img,0);
        //     resize(img,img,Size(W,H));
        //     imwrite("./data/output/chessboard_sep/multicompound/diret/"+to_string(y)+".bmp",img);
        // }

        for(int x = 0;x < N+8;x++) {
            image_list.clear();
            for (int y = 0;y < M;y++) {
                string img_index = folder + "/" + to_string(x)+ "/" + to_string(y)+".bmp";
                imread2(img_index,img,0);
                resize(img,img,Size(W,H));
                image_list.push_back(img);
            }

            Mat max_value_matrix = Mat::zeros(H,W,CV_8UC1);
            Mat min_value_matrix = Mat::zeros(H,W,CV_8UC1);
            double Ld = 0 ,Lg = 0;
            for(int row=0;row<H;row++){
                for(int col=0;col<W;col++) {
                    int max_value = 0;
                    int min_value = 255;
                    int index_max = 0;
                    int index_min = 0;
                    for (int i = 0;i < M; i++) {
                        if(image_list[i].at<uchar>(row,col)>max_value) {
                            max_value= image_list[i].at<uchar>(row,col);
                            index_max = i;
                        }
                        if (image_list[i].at<uchar>(row,col)<min_value) {
                            min_value= image_list[i].at<uchar>(row,col);
                            index_min = i;
                        }
                        max_value_matrix.at<uchar>(row,col) = max_value;
                        min_value_matrix.at<uchar>(row,col) = min_value;
                    }
                    Ld = (max_value - min_value)/(1-b);
                    Lg = 2*(max_value - Ld)/(1+b);
                    directionComponent.at<uchar>(row,col) = Ld;
                    globalComponent.at<uchar>(row,col) = Lg;
                }
            }
            // imwrite("./data/output/chessboard_sep/max_value_matrix.bmp",max_value_matrix);
            // imwrite("./data/output/chessboard_sep/min_value_matrix.bmp",min_value_matrix);
            imwrite("./data/output/chessboard_sep/multicompound/diret/"+to_string(x)+".bmp",directionComponent);
            imwrite("./data/output/chessboard_sep/multicompound/global/"+to_string(x)+".bmp",globalComponent);
            global_fusion = global_fusion + globalComponent;
        }
        imwrite("./data/output/chessboard_sep/multicompound/global/global_fusion.bmp",global_fusion);
        return true;
    }

    void separation_phase_shift(const string &folder,const int W, const int H,const int M,const double b,const bool show) {
        for (int x = 0;x < 12;x++) {
            image_list.clear();
            string subflolder = folder+"./24-11-20/"+to_string(x);
            Mat img;
            Mat directionComponent = Mat::zeros(H,W,CV_8UC1);
            Mat globalComponent = Mat::zeros(H,W,CV_8UC1);
            glob(subflolder, input_file);
            for (auto &file: input_file) {
                imread2(file,img,0);
                resize(img,img,Size(W,H));
                image_list.push_back(img);
            }
            Mat max_value_matrix = Mat::zeros(H,W,CV_8UC1);
            Mat min_value_matrix = Mat::zeros(H,W,CV_8UC1);
            double Ld = 0 ,Lg = 0;
            for(int row=0;row<H;row++){
                for(int col=0;col<W;col++) {
                    int max_value = 0;
                    int min_value = 255;
                    for (int i = 0;i < M; i++) {
                        if(image_list[i].at<uchar>(row,col)>max_value) {
                            max_value= image_list[i].at<uchar>(row,col);
                        }
                        if (image_list[i].at<uchar>(row,col)<min_value) {
                            min_value= image_list[i].at<uchar>(row,col);
                        }
                        max_value_matrix.at<uchar>(row,col) = max_value;
                        min_value_matrix.at<uchar>(row,col) = min_value;
                    }
                    Ld = (max_value - min_value)/(1-b);
                    Lg = 2*(max_value - Ld)/(1+b);
                    directionComponent.at<uchar>(row,col) = Ld;
                    globalComponent.at<uchar>(row,col) = Lg;
                }
            }
            // imwrite("./data/output/chessboard_sep/max_value_matrix.bmp",max_value_matrix);
            // imwrite("./data/output/chessboard_sep/min_value_matrix.bmp",min_value_matrix);
            imwrite("./data/output/PhaseShiftRemoval/"+to_string(x)+".bmp",directionComponent);
            // imwrite("./data/output/chessboard_sep/globalComponent.bmp",globalComponent);
            if(show) {
                imshow("max_value_matrix",max_value_matrix);
                imshow("min_value_matrix",min_value_matrix);
                imshow("directionComponent",directionComponent);
                imshow("globalComponent",globalComponent);
                waitKey(0);
                destroyAllWindows();
            }
        }
    }
};

class Separation {
private:
    vector<String> input_file;
    vector<Mat> image_list;
    vector<float> cos_list;
    vector<float> sin_list;
    Mat fusion_image;
public:
    bool separation(const string &folder,const int W, const int H,const int N,const bool show) {
        Mat img;
        Mat directionComponent = Mat::zeros(H,W,CV_32FC1);
        Mat globalComponent = Mat::zeros(H,W,CV_32FC1);
        glob(folder, input_file);
        for (auto &file: input_file) {
            imread2(file,img,0);
            resize(img,img,Size(W,H));
            image_list.push_back(img);
        }
        // for (int i = 0;i < image_list.size();i++) {
        //     imshow("img",image_list[i]);
        //     waitKey(0);
        // }
        for (int i = 0;i < N;i++) {
            float cos_value = cos(2*i*M_PI/N);
            float sin_value = sin(2*i*M_PI/N);
            cos_list.push_back(cos_value);
            sin_list.push_back(sin_value);
        }

        float L1 , L2 , L3;
        float temp = 4.0/N;
        float Ld = 0,Lg = 0;
        for(int row=0;row<H;row++){
                for(int col=0;col<W;col++) {
                    L1 = 0,L2 = 0,L3 = 0,Ld = 0,Lg = 0;
                    for (int i = 0;i < N; i++) {
                        L1 = cos_list[i]*image_list[i].at<uchar>(row,col)+L1;
                        L2 = sin_list[i]*image_list[i].at<uchar>(row,col)+L2;
                        L3 = image_list[i].at<uchar>(row,col)+L3;
                    }
                    Ld = sqrt((temp*L1)*(temp*L1)+(temp*L2)*(temp*L2));
                    Lg = 2*L3/N-Ld;
                    directionComponent.at<float>(row,col) = Ld;
                    globalComponent.at<float>(row,col) = Lg;
                }
        }
        Mat fusion_image = Mat::zeros(H,W,CV_32FC1);
        fusion_image = directionComponent+globalComponent/2;
        fusion_image.convertTo(fusion_image,CV_8UC1);
        imshow("fusion_image",fusion_image);
        waitKey(0);

        imwrite("./data/output/fringe_sep/directionComponent.bmp",directionComponent);
        imwrite("./data/output/fringe_sep/globalComponent.bmp",globalComponent);
        imwrite("./data/output/fringe_sep/fusion.bmp",fusion_image);

        if (show) {
            namedWindow("directionComponent",WINDOW_FREERATIO);
            imshow("directionComponent",directionComponent);
            namedWindow("globalComponent",WINDOW_FREERATIO);
            imshow("globalComponent",globalComponent);
            waitKey(0);
            destroyAllWindows();
        }
        return show;
    };


};

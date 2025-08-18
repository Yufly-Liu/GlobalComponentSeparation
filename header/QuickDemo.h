//
// Created by 94097 on 2024/10/17.
//

#ifndef QUICKDEMO_H
#define QUICKDEMO_H

#endif //QUICKDEMO_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

namespace std::filesystem::__cxx11 {
    class path;
}

using namespace std;

cv::Mat inline image_shift(cv::Mat &image_orginal, int pixel_shift,int direction);

void inline imread2(const string filename, cv::Mat &img,int color_model) {
    img = cv::imread(filename,color_model);
    if(img.empty()) {
        cerr<<"read image failed"<<endl;assert(0);
    }
};

void inline make_pattern_fringe(int W,int H,int Step,int D,int stripeHeight,int stripeWidth) {
    vector<cv::Mat> fringe_list;
    // 创建一个全黑的图像
    cv::Mat image(H, W, CV_8UC1, cv::Scalar(0));
    // 设置条纹的颜色为白色
    cv::Scalar white(255);
    // 在指定的高度处画出白色条纹
    if (stripeHeight > 0) {
        for (int y = 0; y < H; y += stripeHeight * 2) {
            for (int h = 0; h < stripeHeight && y + h < H; ++h) {
                for (int x = 0; x < W; ++x) {
                    image.at<uchar>(y + h, x) = 255;
                }
            }
        }
    }
    if (stripeWidth > 0) {
        for (int x = 0; x < W; x += stripeWidth * 2) {
            for (int w = 0; w < stripeWidth && x + w < W; ++w) {
                for (int y = 0; y < H; ++y) {
                    image.at<uchar>(y, x + w) = 255;
                }
            }
        }
    }
    // 显示图像
    cv::imshow("Black and White Stripes", image);
    cv::waitKey(0);

    fringe_list.push_back(image);
    if(stripeWidth > 0) {
        for (int step = 0;step<Step; step++) {
            cv::Mat fringe_shift =image_shift(fringe_list[step],D,1);
            fringe_list.push_back(fringe_shift);
        }
        for (int i = 0; i < Step; i++) {
            cv::imwrite("./data/pattern_ideal/fringe/fringe_"+to_string(2*stripeWidth)+"_"+to_string(i)+".bmp", fringe_list[i]);
        }
    }
    if(stripeHeight > 0) {
        for (int step = 0;step<Step; step++) {
            cv::Mat fringe_shift =image_shift(fringe_list[step],D,0);
            fringe_list.push_back(fringe_shift);
        }
        for (int i = 0; i < Step; i++) {
            cv::imwrite("./data/pattern_ideal/fringe/fringe_"+to_string(2*stripeHeight)+"_"+to_string(i)+".bmp", fringe_list[i]);
        }
    }
}



void make_pattern_chessboard(int W,int H,int Step,int D,int squareSize) {
    vector<cv::Mat> chessboard_list;
    cv::Mat chessboard0(H, W, CV_8UC1, cv::Scalar(255)); // 初始为白色
    cv::Mat chessboard1(H, W, CV_8UC1, cv::Scalar(255)); // 初始为白色
    // 计算有多少行和列
    int rows = H / squareSize;
    int cols = W / squareSize;
    // 绘制棋盘格
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            cv::Rect rect(c * squareSize, r * squareSize, squareSize, squareSize);
            if ((r + c) % 2 == 0) {
                // 填充黑色方块
                cv::rectangle(chessboard0, rect, cv::Scalar(0), -1);
            }
        }
    }
    chessboard_list.push_back(chessboard0);
    for (int step = 0;step<Step; step++) {
        cv::Mat chessboard_shift =image_shift(chessboard_list[step],D,1);
        chessboard_list.push_back(chessboard_shift);
    }
    // 保存图像
    for (int i = 0; i < Step; i++) {
        cv::imwrite("./data/pattern_ideal/chessboard/chessboard_"+to_string(D)+"_"+to_string(i)+".bmp", chessboard_list[i]);
    }
}

cv::Mat inline image_shift(cv::Mat &image_orginal,const int pixel_shift,int direction) {
    // 读取图像
    cv::Mat img = image_orginal;
    if (img.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        assert(0);
    }
    // 获取图像的宽度和高度
    int cols = img.cols;
    int rows = img.rows;
    // 创建一个新的图像
    cv::Mat shiftedImg(rows, cols, img.type(), cv::Scalar(0, 0, 0));

    // 如果N大于图像宽度，则不执行任何操作
    if (pixel_shift >= cols) {
        std::cout << "N is too large to shift the image." << std::endl;
        assert(0);
    }
    // 进行图像的右移操作
    if(direction == 1) {
        img(cv::Rect(pixel_shift, 0, cols - pixel_shift, rows)).copyTo(shiftedImg(cv::Rect(0, 0, cols - pixel_shift, rows)));
        img(cv::Rect(0, 0, pixel_shift, rows)).copyTo(shiftedImg(cv::Rect(cols-pixel_shift, 0, pixel_shift, rows)));
        cv::imshow("Shifted Image", shiftedImg);
        cv::waitKey(0);
    }
    //进行图像的上下平移
    if(direction == 0) {
        img(cv::Rect(0, 0, cols, rows - pixel_shift)).copyTo(shiftedImg(cv::Rect(0, pixel_shift, cols, rows - pixel_shift)));
        img(cv::Rect(0, rows - pixel_shift, cols, pixel_shift)).copyTo(shiftedImg(cv::Rect(0, 0, cols, pixel_shift)));
        cv::imshow("Shifted Image", shiftedImg);
        cv::waitKey(0);
    }
    // 保存结果
    return shiftedImg;
}

void  inline calc_warppred_phase(const string &folder,const int W, const int H,const int N,const bool show) {
    vector<cv::Mat> image_list;
    vector<cv::String> input_file;
    cv::Mat img;
    cv::Mat sin_sum = cv::Mat::zeros(H,W,CV_32F);
    cv::Mat cos_sum = cv::Mat::zeros(H,W,CV_32F);
    cv::Mat image = cv::Mat::zeros(H,W,CV_32F);
    cv::Mat warppred_phase_image = cv::Mat::zeros(H,W,CV_32F);
    cv::glob(folder, input_file);
    for (auto &file: input_file) {
        imread2(file,img,0);
        resize(img,img,cv::Size(W,H));
        image_list.push_back(img);
    }
    for (const auto & i : image_list) {
        imshow("img",i);
        cv::waitKey(0);
    }

    for(int i=0;i<N;i++) {
        image_list[i].convertTo(image,CV_32F);
        sin_sum = image*sin(2 * i * M_PI / N)+sin_sum;
        cos_sum = image*cos(2 * i * M_PI / N)+cos_sum;
    }
    for(int i=0;i<H;i++) {
        for(int j=0;j<W;j++) {
            warppred_phase_image.at<float>(i,j) = atan2(sin_sum.at<float>(i,j),cos_sum.at<float>(i,j));
        }
    }
    ofstream outfile("./data/warppred_phase_image.txt");
    for(int i=0;i<H;i++) {
        for(int j=0;j<W;j++) {
            outfile<<warppred_phase_image.at<float>(i,j)<<" ";
        }
        outfile<<endl;
    }

    cv::imshow("warppred_phase_image",warppred_phase_image);
    cv::waitKey(0);
}

// 矩阵减法函数
void subtractMatrices(cv::Mat mat1,cv::Mat mat2,cv::Mat result) {
    for (int i = 0; i < mat1.rows; ++i) {
        for (int j = 0; j < mat1.cols; ++j) {
            result.at<uchar>(i,j) = mat1.at<uchar>(i,j) - mat2.at<uchar>(i,j);
        }
    }
}

void substruct_interrection(const string &folder,const int W, const int H,const int N){
    vector<cv::String> input_file;
    cv::Mat img;
    cv::Mat img_BW = cv::Mat::zeros(H,W,CV_8UC1);
    cv::Mat img_SIN = cv::Mat::zeros(H,W,CV_8UC1);
    vector<cv::Mat> image_list;
    cv::glob(folder, input_file);
    for (auto &file: input_file) {
        imread2(file,img,0);
        resize(img,img,cv::Size(W,H));
        image_list.push_back(img);
    }
    cv::Mat globalcomponent_BW = cv::imread("./data/output/chessboard_sep/globalComponent.bmp",0);
    cv::Mat globalcomponent_SIN = cv::imread("./data/output/fringe_sep/globalComponent.bmp",0);
    for (int i = 0; i < N; i++) {
        img_BW = image_list[i] - globalcomponent_BW/2;
        img_SIN = image_list[i] - globalcomponent_SIN/2;

        cv::imwrite("./data/input/fringe/"+to_string(i)+"-BW.bmp", img_BW);
        cv::imwrite("./data/input/fringe/"+to_string(i)+"-SIN.bmp", img_SIN);
    }
}

void binarizeMatrix(const cv::Mat &input, cv::Mat &output, int thresholdValue) {
    // 确保输入矩阵是单通道的
    if (input.channels() != 1) {
        std::cerr << "Input matrix must be single channel." << std::endl;
        assert(0);
    }
    // 使用阈值进行二值化
    cv::threshold(input, output, thresholdValue, 1, cv::THRESH_BINARY);
    // 显示二值化后的图像
}



 void make_patterns_compound(const string &folder_sin,const string &folder_mask,const int W, const int H,const int N,const bool show) {
    vector<cv::Mat> image_list_sinusoidal;
    vector<cv::Mat> image_list_mask;
    vector<string> input_file_sin;
    vector<cv::String> input_file_mask;
    vector<float> ratio_list;
    cv::Mat img;
    for(int i =0;i<N+8;i++) {
        input_file_sin.push_back(folder_sin+"/"+to_string(i)+".bmp");
    }
    for (auto &file: input_file_sin) {
        imread2(file,img,0);
        resize(img,img,cv::Size(W,H));
        // cv::imshow("img",img);
        // cv::waitKey(0);
        image_list_sinusoidal.push_back(img);

    }
    float E_sum = 0;
    vector<float> rows = img(cv::Range(0,1), cv::Range::all());
    for(float element : rows) {
        E_sum += element * element;
    }
    cout<<"E_original = "<<E_sum<<endl;

    cv::glob(folder_mask, input_file_mask);
    for (auto &file: input_file_mask) {
        imread2(file,img,0);
        resize(img,img,cv::Size(W,H));
        image_list_mask.push_back(img);
    }
    for(int i = 0;i<image_list_mask.size();i++) {
        binarizeMatrix(image_list_mask[i],image_list_mask[i],127);
    }
    cv::Mat pattern_compound = cv::Mat::zeros(H,W,CV_8UC1);
    for(int i =0;i<image_list_sinusoidal.size();i++) {
        for(int j = 0;j<image_list_mask.size();j++) {
            pattern_compound = image_list_sinusoidal[i].mul(image_list_mask[j]);
            float sum = 0;
            float ratio = 0;
            vector<float> row = pattern_compound(cv::Range(0,1), cv::Range::all());
            for(float element : row) {
                sum += element * element;
            }
            ratio = sum/E_sum;
            ratio_list.push_back(ratio);
            cout<<ratio_list.size()<<endl;
            cout<<"ratio = "<<ratio<<endl;
            cv::imwrite("./data/pattern_ideal/compound/"+to_string(i)+"_"+to_string(j)+".bmp",pattern_compound);
        }
    }
    // 打开一个文件用于写入
    std::ofstream outFile("./data/pattern_ideal/compound/ratio_list.txt");
    // 检查文件是否成功打开
    if (!outFile.is_open()) {
        std::cerr << "无法打开文件进行写入" << std::endl;
    }
    // 将 ratio_list 中的每个元素写入文件
    for (const auto& ratio : ratio_list) {
        outFile << ratio << "\n";
    }
    // 关闭文件
    outFile.close();
}

void renameBmpFilesInDirectory(const std::string& directoryPath,const int N,const int M,const int W,const int H) {
    std::vector<string> bmpFiles;
    vector<cv::Mat> image_list;
    vector<cv::String> input_file;
    for(int x = 0;x < N+8;x++) {
        cv::Mat img = cv::Mat::zeros(H,W,CV_8UC1);
        image_list.clear();
        for (int y = 0;y < M;y++) {
            string dir_index = directoryPath + "/" + to_string(x);
            glob(dir_index, input_file);
            for (auto &file: input_file) {
                imread2(file,img,0);
                resize(img,img,cv::Size(W,H));
                image_list.push_back(img);
            }
            cv::imwrite("./data/input/chessboard/rename/"+to_string(x)+"/"+"0.bmp",image_list[0]);
            cv::imwrite("./data/input/chessboard/rename/"+to_string(x)+"/"+"1.bmp",image_list[1]);
            cv::imwrite("./data/input/chessboard/rename/"+to_string(x)+"/"+"2.bmp",image_list[2]);
            cv::imwrite("./data/input/chessboard/rename/"+to_string(x)+"/"+"3.bmp",image_list[3]);
            cv::imwrite("./data/input/chessboard/rename/"+to_string(x)+"/"+"4.bmp",image_list[4]);
        }
    }
}

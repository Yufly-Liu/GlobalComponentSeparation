#include <iostream>
#include <opencv2/opencv.hpp>
#include "config.h"
#include "separation.h"
#include <vector>

using namespace std;
using namespace cv;
clock_t start_time, end_time;

int main()
{
    start_time = clock();
    system("chcp 65001");
    vector<String> image_file;
    PATH path;
    PARA para;
    // Mat img = imread("D:/LIUYUFEI/code/GlobalComponentSeparation/data/output/chessboard_sep/1/globalComponent.bmp", IMREAD_GRAYSCALE);
    // binarizeMatrix(img,img,13);
    // imshow("img",img);
    // waitKey(0);

    // calc_warppred_phase(path.fringe_dir,para.W_DLP,para.H_DLP,para.N,para.show);
    // make_pattern_chessboard(para.W_DLP,para.H_DLP,para.step,para.D,para.square_size);
    // make_pattern_fringe(para.W_DLP,para.H_DLP,para.step,para.D,para.stripeHeight,para.stripeWidth);
    // make_patterns_compound(path.sinusoid_dir,path.mask_dir,para.W_DLP,para.H_DLP,para.N,para.show);
    // renameBmpFilesInDirectory("D:/LIUYUFEI/code/GlobalComponentSeparation/data/compound",para.N,para.M,para.W,para.H);
    Separation_chessboard chessboard;
    // chessboard.separation(path.chessboard_dir,para.W,para.H,para.M,para.b,para.show);
    chessboard.separationByEnergy(path.multicompound_dir,para.W,para.H,para.M,para.N,para.b);
    // chessboard.separation_phase_shift(path.input_dir,para.W,para.H,para.M,para.b,para.show);
    // Separation separation;
    // separation.separation(path.fringe_dir,para.W,para.H,para.N,para.show);
    // substruct_interrection(path.fringe_dir,para.W,para.H,para.N);
    //计算时间
    end_time = clock();
    double t = double(end_time - start_time)/CLOCKS_PER_SEC;
    cout << "程序耗时: " << t << "s"<< endl;
    return 0;
}

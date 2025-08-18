
#ifndef CONFIG_H
#define CONFIG_H

#endif //CONFIG_H

#include <iostream>
using  namespace std;

struct PATH {
    string sep = "/";
    string root_dir = R"(./)";
    string data_dir = root_dir + sep + "data";
    string input_dir = data_dir + sep + "input";
    string pattern_ideal_dir = data_dir + sep + "pattern_ideal";
    string compound_dir = pattern_ideal_dir + sep + "compound";
    string sinusoid_dir = compound_dir + sep + "sinusoidal";
    string mask_dir = compound_dir + sep + "mask";
    string chessboard_dir = input_dir + sep + "chessboard";
    string multicompound_dir =chessboard_dir + sep + "rename";
    string fringe_dir = input_dir + sep + "fringe";
    string output_dir = data_dir + sep + "output";
};

struct PARA {
    int W = 1624;
    int H = 1240;
    int N = 12;//相移步数
    int M = 5;//棋盘格张数
    double b = 0;//投影未激活是的亮度分数
    bool show = false;

    //生成图像时候的参数
    int W_DLP = 1280;
    int H_DLP = 720;
    int square_size = 40;
    int step = 3;
    int D = 8;
    //生成条纹时候的参数
    int stripeHeight = 10;//水平，不生成时请设置为0
    int stripeWidth = 0;//竖直

};
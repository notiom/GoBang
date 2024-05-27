#include<iostream>
#include<graphics.h> //easyx图形库的头文件
#include"ChessGame.h"

#define GRADE_SIZE_13 0 //设置为1选择13*13的棋盘，设置为0选择19*19的棋盘
#define CHESS_BLACK_PLAYERFLAG 1 //设置为1选择黑棋先走，设置为0选择白棋先走
#define WHOPK 1 
//设置为1人机对战，设置为2人人对战

int main(int argc, char* argv[]) 
{
    int gradeSize = 0, margin_x = 0, margin_y = 0;
    float chessSize = 0.0f;
    if (GRADE_SIZE_13) {
        gradeSize = 13;
        margin_x = 44;
        margin_y = 43;
        chessSize = 67.3f;
    }
    else {
        gradeSize = 19;
        margin_x = 52;
        margin_y = 50.5;
        chessSize = 43.2f;
    }
	Man man{};
	AI ai{};
    Chess chess = { gradeSize,margin_x, margin_y,chessSize };
	ChessGame game(&man, &ai, &chess);

    int playerFlayBlack = CHESS_BLACK_PLAYERFLAG;
    int whoFighting = WHOPK;
	game.play(playerFlayBlack, whoFighting);


	return 0;
}

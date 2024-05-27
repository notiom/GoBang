#include "Man.h"

void Man::init(Chess* chess)
{
	this->chess = chess;
}

void Man::go(bool isDoubleMan)
{
	MOUSEMSG msg;
	ChessPos pos;
	
	//通过chess对象，判断落子是否有效
	while (1)
	{
		//获取鼠标消息
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBoard(msg.x, msg.y, &pos))
		{
			break;
		}

	}
	printf("您所落位置->:(%d,%d)\n", pos.row, pos.col);
	//落子
	if (isDoubleMan)  //并未设置人人对战
	{
		chess->ChessDown(&pos, CHESS_BLACK);
	}
	else
	{
		if (chess->playerFlag) //这里通过playerFlag切换落子颜色
		{
			chess->ChessDown(&pos, CHESS_BLACK);
		}
		else
		{
			chess->ChessDown(&pos, CHESS_WHITE);
		}
	}
	
}

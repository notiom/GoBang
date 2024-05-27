#include "Chess.h"
#include<mmsystem.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}
void Chess::init(int playerFlayBlack)
{
	// 创建我们的游戏窗口
	initgraph(897, 895, EX_SHOWCONSOLE);

	//显示我们的棋盘图片
	if (gradeSize == 13)
	{
		loadimage(0, "../res/棋盘2.jpg");
	}
	else if(gradeSize == 19)
		loadimage(0, "../res/棋盘3.png");
	
	//播放开始提示音
	mciSendString("play ../res/start.wav", 0, 0, 0);
	//加载黑棋白棋图片
	loadimage(&chessBlackImg, "../res/black.png", chessSize,chessSize,true);
	loadimage(&chessWhiteImg, "../res/white.png", chessSize, chessSize, true);

	//清理棋盘
	for (int i = 0; i < gradeSize; i++)
	{
		for (int j = 0; j < gradeSize; j++)
		{
			chessMap[i][j] = 0; 
		}
	}

	//判断操作者使那个子先走
	if (playerFlayBlack)
	{
		playerFlag = true;
	}
	else
	{
		playerFlag = false;
	}
}

bool Chess::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;   //几行几列
	int row = (y - margin_y) / chessSize;
	bool ret = false;
	int offset = chessSize * 0.4;
	do
	{
		//判断左上坐标
		int distanceLen;
		int leftTopPosX = margin_x + chessSize * col;   //左上坐标的位置，x则表示目前鼠标的位置
		int leftTopPosY = margin_y + chessSize * row;
		distanceLen = sqrt(pow(x - leftTopPosX, 2) + pow(y - leftTopPosY, 2));
		if (distanceLen < offset)
		{
			pos->row = row;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}
		//判断右上坐标
		int rightTopPosX = leftTopPosX + chessSize;
		int rightTopPosY = leftTopPosY;
		distanceLen = sqrt(pow(x - rightTopPosX, 2) + pow(y - rightTopPosY, 2));
		if (distanceLen < offset)
		{
			pos->row = row;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}
		//判断左下坐标
		int leftBelowPosX = leftTopPosX;
		int leftBelowPosY = leftTopPosY + chessSize;
		distanceLen = sqrt(pow(x - leftBelowPosX, 2) + pow(y - leftBelowPosY, 2));
		if (distanceLen < offset)
		{
			pos->row = row + 1;
			pos->col = col;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}
		//判断右下坐标
		int rightBelowPosX = leftTopPosX + chessSize;
		int rightBelowPosY = leftTopPosY + chessSize;
		distanceLen = sqrt(pow(x - rightBelowPosX, 2) + pow(y - rightBelowPosY,2));
		if (distanceLen < offset)
		{
			pos->row = row + 1;
			pos->col = col + 1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				ret = true;
			}
			break;
		}
	} while (0);
	

	return ret;
}

void Chess::ChessDown(ChessPos* pos, chess_kind_t kind)
{
	int x = margin_x + chessSize * pos->col - 0.5 * chessSize;
	int y = margin_y + chessSize * pos->row - 0.5 * chessSize;

	if (kind == CHESS_WHITE)
	{
		putimagePNG(x, y, &chessWhiteImg);
	}
	else
	{
		putimagePNG(x, y, &chessBlackImg);
	}
	updateGameMap(pos);
}

int Chess::getGradeSize()
{
	return gradeSize;
}

bool Chess::checkOver()
{
	if (checkWin())
	{
		if (playerFlag == false) 
		{
			//刚才走棋的为黑棋(棋手方)
			mciSendString("play ../res/不错.mp3", 0, 0, 0);
			loadimage(0, "../res/胜利.jpg");

		}
		else
		{
			mciSendString("play ../res/失败.mp3", 0, 0, 0);
			loadimage(0, "../res/失败.jpg");
		}
		_getch();  //暂停等待按下任意键
		return true;
	}
	return false;
}

int Chess::getChessData(int row,int col)
{
	return chessMap[row][col];
}

void Chess::updateGameMap(ChessPos* pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag; //黑白方交换行棋
}

bool Chess::checkWin()
{
	//最近落子点的位置
	int row = lastPos.row;
	int col = lastPos.col;

	//落子点的水平方向
	for (int i = 0; i < 5; i++)
	{
		//对于第i次，五子连珠的最左侧位置
		//i = 0 (row,col),(row,col+1),(row,col+2),(row,col+3),(row,col+4)
		//i = 1 (row,col-1),(row,col),(row,col+1),(row,col+2),(row,col+3)
		//依次类推
		if (col - i >= 0 && col - i + 4 < gradeSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
		{
			return true;
		}
		
	}

	//落子点的垂直方向
	for (int i = 0; i < 5; i++)
	{
		//依次类推
		if (row - i >= 0 && row - i + 4 < gradeSize &&
			chessMap[row-i][col] == chessMap[row - i + 1][col] &&
			chessMap[row-i][col] == chessMap[row - i + 2][col] &&
			chessMap[row-i][col] == chessMap[row - i + 3][col] &&
			chessMap[row-i][col] == chessMap[row - i + 4][col])
		{
			return true;
		}

	}

	//落子点的//方向
	for (int i = 0; i < 5; i++)
	{
		//依次类推
		if (row + i < gradeSize && row + i - 4 >=0 &&
			col - i >= 0 && col - i + 4 <gradeSize &&
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
		{
			return true;
		}

	}

	//落子点的\\方向
	for (int i = 0; i < 5; i++)
	{
		//依次类推
		if (row - i >= 0 && row - i + 4 < gradeSize &&
			col + i <gradeSize && col + i - 4 >=0 &&
			chessMap[row - i][col + i] == chessMap[row - i + 1][col + i - 1] &&
			chessMap[row - i][col + i] == chessMap[row - i + 2][col + i - 2] &&
			chessMap[row - i][col + i] == chessMap[row - i + 3][col + i - 3] &&
			chessMap[row - i][col + i] == chessMap[row - i + 4][col + i - 4])
		{
			return true;
		}

	}
	return false;

}

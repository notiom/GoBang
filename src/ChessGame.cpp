#include "ChessGame.h"

ChessGame::ChessGame(Man* man, AI* ai, Chess* chess)
{
	this->man = man;
	this->ai = ai;
	this->chess = chess;
	man->init(chess);
	ai->init(chess);
}
//对局(开始五子棋游戏)
void ChessGame::play(int playerFlayBlack,int whoFighting)
{
	chess->init(playerFlayBlack); //棋盘初始化
	if(whoFighting==1&& playerFlayBlack) //人机黑子先走
	{
		while (1)
		{
			//先由棋手走
			man->go();
			if (chess->checkOver())
			{
				chess->init(playerFlayBlack);  //走棋已经结束
				continue;
			}
			//由AI走
			ai->go();
			if (chess->checkOver())
			{
				chess->init(playerFlayBlack);
				continue;
			}
		}
	}
	else if(whoFighting == 1 && !playerFlayBlack)  //人机白子先走
	{ 
		while (1)
		{
			//先由AI走
			ai->go();
			if (chess->checkOver())
			{
				chess->init(playerFlayBlack);  //走棋已经结束
				continue;
			}
			//由棋手走
			man->go();
			if (chess->checkOver())
			{
				chess->init(playerFlayBlack);
				continue;
			}
		}
	}
	else if (whoFighting == 2)  //人人走
	{
		while (1)
		{
			//先由棋手1走
			man->go(false);
			if (chess->checkOver())
			{
				chess->init(playerFlayBlack);  //走棋已经结束
				continue;
			}
			//再由棋手2走
			man->go(false);
			if (chess->checkOver())
			{
				chess->init(playerFlayBlack);
				continue;
			}
		}
	}

}

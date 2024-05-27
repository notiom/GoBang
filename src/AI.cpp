#include "AI.h"
#include<iostream>
void AI::init(Chess* chess)
{
	this->chess = chess;

	int size = chess->getGradeSize();
	for (int i = 0; i < size; i++)
	{
		vector<int> row;
		for (int j = 0; j < size; j++)
		{
			row.push_back(0);
		}
		scoreMap.push_back(row);
	}
}

void AI::go()
{
	
	ChessPos pos = think();
	printf("AI所落位置->:(%d,%d)\n", pos.row, pos.col);
	Sleep(1000);  //假装思考,已表示尊重
	chess->ChessDown(&pos, CHESS_WHITE);
}

void AI::calculateScore()
{
	int personNum = 0; //棋手方连续有多少个连续的棋子
	int aiNum = 0;//AI方有多少个连续的棋子
	int emptyNum = 0; //该方向上空白位的个数
	//评分向量数组清零
	for (int i = 0; i < scoreMap.size(); i++)
	{
		for (int j = 0; j < scoreMap[i].size(); j++)
		{
			scoreMap[i][j] = 0;
		}
	}
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
		
			if (chess->getChessData(row,col)) //只要不为0，就忽略计算该点代价，即为1和-1均会忽略
			{
				continue;
			}
			for (int y = -1; y <= 0; y++)
			{
				for (int x = -1; x <= 1; x++) //考虑上下左右以及斜边8种情况
				{
					if (y == 0 && x != 1) continue;   //忽略(0,0)和(-1,0)位置  依次考虑左上，上，右上，右
					//假设黑棋在该位置落子，会构成什么棋形
					//代价清零
					personNum = 0;
					aiNum = 0;
					emptyNum = 0;
					for (int i = 1; i <= 4; i++)
					{
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == 1)
						{
							personNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}
					}

					//反向继续计算
					for (int i = 1; i <= 4; i++)  //右下，下，左下，左
					{
						int curRow = row - i * y;
						int curCol = col - i * x;

						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == 1)
						{
							personNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}

					}

					if (personNum == 1) //连2
					{
						scoreMap[row][col] += 10;
					}
					else if (personNum == 2) //连3
					{
						if (emptyNum == 1)  //死3
						{
							scoreMap[row][col] += 30;
						}
						else if(emptyNum ==2) //活3
						{
							scoreMap[row][col] += 40; 
						}
					}
					else if (personNum == 3)
					{
						if (emptyNum == 1)
						{
							scoreMap[row][col] += 60;
						}
						else if (emptyNum == 2)
						{
							scoreMap[row][col] += 200;  //堵住它的死4
						}
					}
					else if (personNum == 4)
					{
						scoreMap[row][col] += 20000;
					}
					////假设白棋在该位置落子，会构成什么棋形
					emptyNum = 0;
					for (int i = 1; i <= 4; i++)
					{
						int curRow = row + i * y;
						int curCol = col + i * x;

						if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == -1)
						{
							aiNum++;
						}
						else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == 0)
						{
							emptyNum++;
							break;
						}
						else
						{
							break;
						}
					}
					//反向继续计算
					for (int i = 1; i <= 4; i++)
						{
							int curRow = row - i * y;
							int curCol = col - i * x;

							if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == -1)
							{
								aiNum++;
							}
							else if (curRow >= 0 && curRow < size && curCol >= 0 && curCol < size && chess->getChessData(curRow, curCol) == 0)
							{
								emptyNum++;
								break;
							}
							else
							{
								break;
							}
					}
					if (aiNum == 0)
					{
						scoreMap[row][col] += 5;
					}
					else if (aiNum == 1)
					{
						scoreMap[row][col] += 10;
					}
					else if (aiNum == 2)
					{
						if (emptyNum == 1)
						{
							// scoreMap[row][col] += 30;
							scoreMap[row][col] += 25;
						}
						else if (emptyNum == 2)
						{
							scoreMap[row][col] += 50;
						}
					}
					else if (aiNum == 3)
					{
						if (emptyNum == 1)
						{
							// scoreMap[row][col] += 60;
							scoreMap[row][col] += 55;
						}
						else if (emptyNum == 2)
						{
							scoreMap[row][col] += 10000;
						}
					}
					else if (aiNum >= 4)
					{
						scoreMap[row][col] += 30000;
					}

				}
			}
		}
	}
	for (int i = 0; i < scoreMap.size(); i++) //打印代价
	{
		if (i == 0)
		{
			std::cout << "代价地图矩阵:" << std::endl << "{";
		}
		std::cout << "[";
		for (int j = 0; j < scoreMap[i].size()-1; j++)
		{

			std::cout << scoreMap[i][j] << ",";
		}
		std::cout << scoreMap[i][scoreMap[i].size() - 1] << "]";
		if (i == scoreMap.size() - 1)
		{
			std::cout << "}" << std::endl;
		}
		else
		{
		std:cout << std::endl;
		}
	}
}

ChessPos AI::think()
{
	calculateScore();
	vector<ChessPos> maxPoints;
	int maxScore = 0;
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			if (chess->getChessData(row, col) == 0)  //判断此处是否可以走棋
			{
				if (scoreMap[row][col] > maxScore)
				{
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if (scoreMap[row][col] == maxScore)
				{
					maxPoints.push_back(ChessPos(row,col));  //此时直接创建一个结构体对象
				}
			}
		}
	}
	if (chess->getChessData(9, 9) == 0 &&chess->getGradeSize()==19)
	{
		return ChessPos(9, 9);
	}
	else if (chess->getChessData(6,6) == 0 && chess->getGradeSize() == 13)
	{
		return ChessPos(6, 6);
	}
	int index = rand() % maxPoints.size();
	return maxPoints[index];
}

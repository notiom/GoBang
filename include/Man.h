#pragma once
#include"Chess.h"
class Man
{
public:
	void init(Chess* pos);
	void go(bool isDoubleMan = true);
private:
	Chess *chess;
};

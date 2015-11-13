#ifndef LEARNMUDUO_CHAPTER6_SUDOKU_H
#define LEARNMUDUO_CHAPTER6_SUDOKU_H

#include <muduo/base/Types.h>
#include <muduo/base/StringPiece.h>

muduo::string solveSudoku(const muduo::StringPiece& puzzle);
const int kCells = 81;
extern const char kNoSolution[];

#endif

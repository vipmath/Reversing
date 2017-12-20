#pragma once

#include "Common.h"

#include <vector>

class Board
{
public:
    Board();

    Board Copy() const;

    bool IsOnBoard(const Point& pt) const;
    bool IsValidMove(const Point& pt) const;
    bool IsEnd() const;

    StoneType GetCurrentPlayer() const;
    StoneType GetOpponentPlayer() const;
    StoneType GetStoneColor(const Point& pt) const;
    PointSet GetLegalMoves() const;
    StoneType GetWinner() const;
    PointArr GetHistory() const;

    void Clear();
    void DoMove(const Point& pt);

    void ShowBoard() const;

private:
    PointSet flip_stone(const Point& pt) const;
    int flip_count(const Point& pt) const;

private:
    std::vector<StoneType> board_;
    StoneType current_player_;
    bool is_end_;

    int black_stones_;
    int white_stones_;

    std::vector<Point> history_;
};
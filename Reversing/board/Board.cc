#include "Board.h"

#include <tuple>
#include <stdio.h>

Board::Board()
{
    board_.resize(BOARD_SIZE2);

    Clear();
}

Board Board::Copy() const
{
    Board ret;

    std::copy(board_.begin(), board_.end(), ret.board_.begin());
    ret.current_player_ = current_player_;
    ret.is_end_ = is_end_;
    ret.black_stones_ = black_stones_;
    ret.white_stones_ = white_stones_;
    std::copy(history_.begin(), history_.end(), ret.history_.begin());

    return ret;
}

bool Board::IsOnBoard(const Point& pt) const
{
    return (pt.X >= 0 && pt.X < BOARD_SIZE) && (pt.Y >= 0 && pt.Y < BOARD_SIZE);
}

bool Board::IsValidMove(const Point& pt) const
{
    if (pt == Pass || pt == Resign) return true;

    return (IsOnBoard(pt) &&
            (flip_count(pt) > 0));
}

bool Board::IsEnd() const
{
    return is_end_;
}

StoneType Board::GetCurrentPlayer() const
{
    return current_player_;
}

StoneType Board::GetOpponentPlayer() const
{
    return GetOpponent(current_player_);
}

StoneType Board::GetStoneColor(const Point& pt) const
{
    return board_[POS(pt)];
}

PointSet Board::GetLegalMoves() const
{
    PointSet ret;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            Point pt(i, j);

            if (IsValidMove(pt))
                ret.insert(pt);
        }
    }

    return ret;
}

StoneType Board::GetWinner() const
{
	if (is_end_)
	{
		if (black_stones_ > white_stones_) return StoneType::BLACK;
		else if (black_stones_ < white_stones_) return StoneType::WHITE;
	}

	return StoneType::EMPTY;
}

PointArr Board::GetHistory() const
{
    return history_;
}

void Board::Clear()
{
    current_player_ = StoneType::BLACK;
    is_end_ = false;

    std::fill(board_.begin(), board_.end(), StoneType::EMPTY);
    board_[POS(3, 3)] = StoneType::WHITE;
    board_[POS(4, 3)] = StoneType::BLACK;
    board_[POS(3, 4)] = StoneType::BLACK;
    board_[POS(4, 4)] = StoneType::WHITE;

    black_stones_ = 2;
    white_stones_ = 2;
}

void Board::DoMove(const Point& pt)
{
    if (is_end_)
        throw std::runtime_error("Already ended");

    if (pt != Pass && pt != Resign)
    {
        auto flip = flip_stone(pt);

        if (flip.size() == 0)
            throw std::runtime_error("Invalid move");

        board_[POS(pt)] = current_player_;
        for (auto& p : flip)
            board_[POS(p)] = current_player_;

        if (current_player_ == StoneType::BLACK)
        {
			black_stones_ += flip.size();
			white_stones_ -= flip.size();
		}
		else
		{
			white_stones_ += flip.size();
			black_stones_ -= flip.size();
		}
    }

    current_player_ = GetOpponent(current_player_);
    history_.push_back(pt);

    if (pt == Resign)
    {
        is_end_ = true;
    }

    if (GetLegalMoves().size() == 0)
    {
        DoMove(Pass);
    }

    if (history_.back() == Pass)
    {
        is_end_ = true;
    }
}

void Board::ShowBoard() const
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board_[POS(j, i)] == StoneType::EMPTY)
                printf("·");
            else if (board_[POS(j, i)] == StoneType::BLACK)
                printf("B");
            else if (board_[POS(j ,i)] == StoneType::WHITE)
                printf("W");
        }

        printf("\n");
    }
}

PointSet Board::flip_stone(const Point& pt) const
{
    PointSet total;

    static std::vector<std::tuple<int, int>> move = {
        std::make_tuple(1, 0),
        std::make_tuple(0, 1),
        std::make_tuple(-1, 0),
        std::make_tuple(0, -1),
        std::make_tuple(1, 1),
        std::make_tuple(1, -1),
        std::make_tuple(-1, 1),
        std::make_tuple(-1, -1)
    };

    for (auto& m : move)
    {
        Point new_pos = pt;
        PointSet stone;

        for (int i = 0; i < 7; ++i)
        {
            new_pos.X += std::get<0>(m);
            new_pos.Y += std::get<1>(m);

            if (!IsOnBoard(new_pos)) break;

            if (board_[POS(new_pos)] == current_player_)
                total.insert(stone.begin(), stone.end());

            if (board_[POS(new_pos)] != GetOpponent(current_player_)) break;
            stone.insert(new_pos);
        }
    }

    return total;
}

int Board::flip_count(const Point& pt) const
{
    return flip_stone(pt).size();
}

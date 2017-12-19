#pragma once

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <set>

constexpr int BOARD_SIZE = 8;
constexpr int BOARD_SIZE2 = BOARD_SIZE * BOARD_SIZE;

enum class StoneType
{
    EMPTY,
    BLACK,
    WHITE
};

constexpr StoneType GetOpponent(StoneType color)
{
    switch (color)
    {
    case StoneType::BLACK: return StoneType::WHITE;
    case StoneType::WHITE: return StoneType::BLACK;
    default: return StoneType::EMPTY;
    }
}

struct Point
{
    Point() : X(0), Y(0) { }
    Point(int x, int y) : X(x), Y(y) { }

    bool operator==(const Point& pt) const
    {
        return (X == pt.X) && (Y == pt.Y);
    }

    bool operator!=(const Point& pt) const
    {
        return (X != pt.X) || (Y != pt.Y);
    }

    bool operator>(const Point& pt) const
    {
        return (X + Y * BOARD_SIZE) > (pt.X + pt.Y * BOARD_SIZE);
    }

    bool operator<(const Point& pt) const
    {
        return (X + Y * BOARD_SIZE) < (pt.X + pt.Y * BOARD_SIZE);
    }

    int X, Y;
};

const Point Pass = Point(-100, -100);
const Point Resign = Point(-200, -200);

using PointArr = std::vector<Point>;
using PointSet = std::set<Point>;

constexpr int POS(int x, int y)
{
    return x + y * BOARD_SIZE;
}

constexpr int POS(const Point& pt)
{
    return POS(pt.X, pt.Y);
}

inline Point IDX2PT(int idx)
{
    return Point(idx % BOARD_SIZE, idx / BOARD_SIZE);
}

inline Point operator""_pt(const char* str, size_t len)
{
    if (len > 3 || len < 2)
        throw std::runtime_error("Invalid coordinate");

        static std::string coord = "abcdefghjklmnopqrstuvwxyz";
        std::string tmp = str;
        boost::to_lower(tmp);

        return Point((int)coord.find_first_of(tmp[0]), atoi(tmp.substr(1).c_str()) - 1);
}
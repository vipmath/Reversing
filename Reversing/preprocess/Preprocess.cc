#include "Preprocess.h"

Tensor StateToTensor(const Board& board)
{
    Tensor ret { boost::extents[11][BOARD_SIZE][BOARD_SIZE] };

    Board tmp;

    int turns = board.GetHistory().size();

    for (int i = 0; i < turns - 5; ++i)
    {
        tmp.DoMove(board.GetHistory()[i]);
    }

    for (int i = std::max(turns - 5, 0); i < turns; ++i)
    {
        tmp.DoMove(board.GetHistory()[i]);

        std::cout << std::endl;
        tmp.ShowBoard();

        for (int x = 0; x < BOARD_SIZE; ++x)
        {
            for (int y = 0; y < BOARD_SIZE; ++y)
            {
                if (tmp.GetStoneColor(Point(x, y)) == board.GetCurrentPlayer())
                    ret[2 * (turns - i - 1) + 0][y][x] = 1;
                else if (tmp.GetStoneColor(Point(x, y)) == board.GetOpponentPlayer())
                    ret[2 * (turns - i - 1) + 1][y][x] = 1;
            }
        }
    }

    if (board.GetCurrentPlayer() == StoneType::BLACK)
        for (int x = 0; x < BOARD_SIZE; ++x)
            for (int y = 0; y < BOARD_SIZE; ++y)
                ret[10][y][x] = 1;

    return ret;
}
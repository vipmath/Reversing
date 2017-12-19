#include "board/Board.h"
#include "net/Network.h"

#include <iostream>

int main()
{
    Board board;
    Network net;

    board.DoMove("C4"_pt);
    board.DoMove("C3"_pt);

    auto ret = net.EvalState(board);

    std::cout << std::get<1>(ret);

    std::cin.get();
}
#pragma once

#include "../board/Board.h"

#include <boost/multi_array.hpp>

using Tensor = boost::multi_array<float, 3>;

Tensor StateToTensor(const Board& board);
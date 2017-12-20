#pragma once

#include "helper.h"
#include "../board/Board.h"

#include <map>
#include <memory>
#include <tuple>

using ActionProb = std::tuple<Point, float>;
using ActionProbs = std::vector<ActionProb>;

class Network
{
public:
    using Ptr = std::shared_ptr<Network>;

public:
    Network();
    ~Network();

    std::tuple<ActionProbs, float> EvalState(const Board& board);

private:
    mxnet::cpp::Symbol net_;
    mxnet::cpp::Executor* exec_;
    std::map<std::string, mxnet::cpp::NDArray> args_;
};
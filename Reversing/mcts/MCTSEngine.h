#pragma once

#include "../net/Network.h"
#include "TreeNode.h"

struct NetworkParams
{
    Board board;
    TreeNode::Ptr node;

    NetworkParams(const Board& b, TreeNode::Ptr n) : board(b), node(n) { }
};

class MCTSEngine
{
public:
    MCTSEngine();

    Point GenMove(const Board& board);
    void DoMove(const Point& pt);

    void Clear();

private:
    void playout(const Board& board);
    void evaluate();

    void enqueue_eval(const Board& board, TreeNode::Ptr node);

private:
    Network::Ptr net_;
    TreeNode::Ptr root_;

    std::mutex mutex_;
    
    std::atomic<bool> stop_think_;

    std::deque<NetworkParams> network_que_;
    std::atomic<int> network_que_cnt_;
};

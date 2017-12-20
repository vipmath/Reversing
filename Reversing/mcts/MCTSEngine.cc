#include "MCTSEngine.h"

#include <thread>

MCTSEngine::MCTSEngine()
{
    net_ = std::make_shared<Network>();

    root_ = std::make_shared<TreeNode>();
}

Point MCTSEngine::GenMove(const Board& board)
{
    const int Threads = std::thread::hardware_concurrency();

    if (!root_->HasChild())
    {
        auto result = net_->EvalState(board);

        root_->Expand(std::get<0>(result));
    }

    stop_think_ = false;

    std::vector<std::thread> threads(Threads);
    threads[0] = std::thread(&MCTSEngine::evaluate, this);

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 1; j < Threads; ++j)
            threads[j] = std::thread(&MCTSEngine::playout, this, std::ref(board));

        for (int j = 1; j < Threads; ++j)
            threads[j].join();
    }
    stop_think_ = true;

    for (int j = 0; j < Threads; ++j)
        if (threads[j].joinable())
            threads[j].join();

    float temperature = 1.f;
    root_ = root_->Play(temperature);

    return root_->GetAction();
}

void MCTSEngine::DoMove(const Point& pt)
{
    auto it = std::find_if(root_->GetChildren().begin(), root_->GetChildren().end(), [&](TreeNode::Ptr node) {
        return node->GetAction() == pt;
    });

    if (it != root_->GetChildren().end())
    {
        root_ = *it;
        root_->SetParent(nullptr);
    }
    else
    {
        root_ = std::make_shared<TreeNode>(pt);
    }
}

void MCTSEngine::Clear()
{
    root_ = std::make_shared<TreeNode>();
}

void MCTSEngine::playout(const Board& board)
{
    auto node = root_;
    Board cpy = board.Copy();

    while (node->HasChild())
    {
        if (cpy.IsEnd()) break;

        node = node->Select();
        cpy.DoMove(node->GetAction());
    }

    node->Update(0);
    enqueue_eval(cpy, node);
}

void MCTSEngine::evaluate()
{
    while (1)
    {
        if (network_que_cnt_ > 0)
        {
            std::lock_guard<std::mutex> lock(mutex_);

            for (int i = 0; i < network_que_.size(); ++i)
            {
                auto result = net_->EvalState(network_que_[i].board);

                network_que_[i].node->Expand(std::get<0>(result));
                network_que_[i].node->SetV(std::get<1>(result));
            }

            network_que_.clear();
            network_que_cnt_ = 0;
        }

        if (stop_think_) break;
    }
}


void MCTSEngine::enqueue_eval(const Board& board, TreeNode::Ptr node)
{
    std::lock_guard<std::mutex> lock(mutex_);

    network_que_.emplace_back(board, node);
    atomic_fetch_add(&network_que_cnt_, 1);
}
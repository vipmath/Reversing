#pragma once

#include "../net/Network.h"

#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <tuple>

class TreeNode : public std::enable_shared_from_this<TreeNode>
{
public:
    using Ptr = std::shared_ptr<TreeNode>;

    static constexpr float Vl = 3.f;
    static constexpr float puct = 5;

public:
    TreeNode(Point action = Pass, float P = 1.f, Ptr parent = nullptr);

    TreeNode::Ptr Select();
    TreeNode::Ptr Play(float temperature);

    void Expand(const ActionProbs& probs);

    const Point& GetAction() const;
    float GetPrior() const;

    void SetParent(Ptr parent);
    Ptr GetParent();
    std::deque<Ptr>& GetChildren();

    bool IsRoot() const;
    bool HasChild() const;
    
    int GetVisits() const;
    float GetQValue() const;

    void Update(int w, bool own = true);

private:
    Ptr parent_;
    std::deque<Ptr> children_;

    Point action_;

    std::atomic<float> P_;
    std::atomic<float> W_;
    std::atomic<int> N_;

    bool is_expanded_;
    std::mutex mutex_;
};
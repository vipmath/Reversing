#include "Network.h"
#include "../board/Common.h"
#include "../preprocess/Preprocess.h"

#include <vector>

Network::Network()
{
    using namespace mxnet::cpp;

    Symbol data = Symbol::Variable("data");
    Symbol data_label = Symbol::Variable("softmax_label");
    Symbol value_label = Symbol::Variable("tanh_label");

    std::vector<Symbol> layers;
    layers.emplace_back(ConvFactory(data, 256, Shape(5, 5), Shape(1, 1), Shape(2, 2), "0"));

    for (int i = 1; i < 5; ++i)
        layers.emplace_back(ConvFactory(layers.back(), 256, Shape(3, 3), Shape(1, 1), Shape(1, 1), std::to_string(i)));

    Symbol p_fc_w("p_fc_w"), p_fc_b("p_fc_b");
    Symbol v_fc_w("v_fc_w"), v_fc_b("v_fc_b");
    Symbol v_fc2_w("v_fc2_w"), v_fc2_b("v_fc2_b");

    auto p_conv = ConvFactory(layers.back(), 2, Shape(1, 1), Shape(1, 1), Shape(0, 0), "5");
    auto p_fc = FullyConnected(p_conv, p_fc_w, p_fc_b, BOARD_SIZE2);
    auto p_head = SoftmaxOutput("softmax", p_fc, data_label);

    auto v_conv = ConvFactory(layers.back(), 1, Shape(1, 1), Shape(1, 1), Shape(0, 0), "5-1");
    auto v_fc = FullyConnected(v_conv, v_fc_w, v_fc_b, 256);
    auto v_relu = Activation(v_fc, ActivationActType::kRelu);
    auto v_fc2 = FullyConnected(v_relu, v_fc2_w, v_fc2_b, 1);
    auto v_tanh = Activation(v_fc2, ActivationActType::kTanh);
    auto v_head = LinearRegressionOutput(v_tanh, value_label);

    net_ = Symbol::Group({ p_head, v_head });

    args_["data"] = NDArray(Shape(1, 11, BOARD_SIZE, BOARD_SIZE), Context::cpu());
    args_["softmax_label"] = NDArray(Shape(1), Context::cpu());
    args_["tanh_label"] = NDArray(Shape(1), Context::cpu());
    net_.InferArgsMap(Context::cpu(), &args_, args_);

    exec_ = net_.SimpleBind(Context::cpu(), args_);
}

Network::~Network()
{
    if (exec_)
        delete exec_;
}

std::tuple<ActionProbs, float> Network::EvalState(const Board& board)
{
    using namespace mxnet::cpp;

    auto input = StateToTensor(board);

    args_["data"].SyncCopyFromCPU(input.data(), input.num_elements());

    exec_->Forward(false);
    NDArray::WaitAll();

    Tensor output { boost::extents[1][BOARD_SIZE][BOARD_SIZE] };
    exec_->outputs[0].SyncCopyToCPU(output.data(), BOARD_SIZE2);

    std::vector<ActionProb> ret;

    for (int x = 0; x < BOARD_SIZE; ++x)
    {
        for (int y = 0; y < BOARD_SIZE; ++y)
        {
            auto pt = Point(x, y);

            if (board.IsValidMove(pt))
                ret.emplace_back(std::make_tuple(pt, output[0][y][x]));
        }
    }

    float value = 0;
    exec_->outputs[1].SyncCopyToCPU(&value, 1);

    return std::make_tuple(ret, value);
}

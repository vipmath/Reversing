#include "helper.h"

namespace mxnet
{
    namespace cpp
    {
        inline Symbol BatchNorm(const std::string& symbol_name,
			Symbol data,
			mx_float eps = 0.001,
			mx_float momentum = 0.9,
			bool fix_gamma = true,
			bool use_global_stats = false) {
            return Operator("BatchNorm")
                    .SetParam("eps", eps)
                    .SetParam("momentum", momentum)
                    .SetParam("fix_gamma", fix_gamma)
                    .SetParam("use_global_stats", use_global_stats)
                    .SetInput("data", data)
                    .CreateSymbol(symbol_name);
        }
    }
}

mxnet::cpp::Symbol ConvFactory(mxnet::cpp::Symbol data, int num_filter,
		mxnet::cpp::Shape kernel, mxnet::cpp::Shape stride, mxnet::cpp::Shape pad,
		const std::string & name) {
    mxnet::cpp::Symbol conv_w("convolution" + name + "_weight"), conv_b("convolution" + name + "_bias");

    mxnet::cpp::Symbol conv = mxnet::cpp::Convolution("conv_" + name, data,
        conv_w, conv_b, kernel,
        num_filter, stride, mxnet::cpp::Shape(1, 1), pad);
    std::string name_suffix = name;
    mxnet::cpp::Symbol bn = mxnet::cpp::BatchNorm("batchnorm" + name, conv);

    return Activation("acivation" + name, bn, "relu");
}
#pragma once

#include <mxnet-cpp/MxNetCpp.h>

mxnet::cpp::Symbol ConvFactory(mxnet::cpp::Symbol data, int num_filter,
		mxnet::cpp::Shape kernel, mxnet::cpp::Shape stride, mxnet::cpp::Shape pad,
		const std::string & name);
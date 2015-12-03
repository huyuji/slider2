#pragma once
#ifndef id5218FD12_57AD_4116_913B9B30B1312C1D
#define id5218FD12_57AD_4116_913B9B30B1312C1D

#include <opencv2/core.hpp>

namespace ImageProcessor
{
    class Operation
    {
    public:
        virtual cv::Mat operator()(const cv::Mat& image) = 0;
        virtual ~Operation() {};
    };
}

#endif // header

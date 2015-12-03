#pragma once
#ifndef id4DC27CD2_32C5_4CBE_B611753DB21189B4
#define id4DC27CD2_32C5_4CBE_B611753DB21189B4

#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class Contrast : public Operation
    {
    public:
        Contrast(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_histogram;
    };
}

#endif // header

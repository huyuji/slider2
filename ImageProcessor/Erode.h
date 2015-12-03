#pragma once
#ifndef idDA13D0B1_25B2_4DA7_BDE39E8C758B4636
#define idDA13D0B1_25B2_4DA7_BDE39E8C758B4636

#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class Erode : public Operation
    {
    public:
        Erode(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_erode;
    };
}

#endif // header

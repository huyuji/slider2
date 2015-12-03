#pragma once
#ifndef id4BA7BA0F_7774_4EC5_BAD74B519AD86A2D
#define id4BA7BA0F_7774_4EC5_BAD74B519AD86A2D

#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class Convert : public Operation
    {
    public:
        Convert(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_bright;
        double m_contrast;
    };
}

#endif // header

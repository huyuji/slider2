#pragma once
#ifndef id29E657F0_6995_404C_85EA1DAE1796FCF9
#define id29E657F0_6995_404C_85EA1DAE1796FCF9

#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class Threshold : public Operation
    {
    public:
        Threshold(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_thresh;
    };
}

#endif // header

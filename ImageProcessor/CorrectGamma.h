#pragma once
#ifndef idD84F5E35_2CE5_4163_850EA7F5077F9869
#define idD84F5E35_2CE5_4163_850EA7F5077F9869

#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class CorrectGamma : public Operation
    {
    public:
        CorrectGamma(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_gamma;
    };
}

#endif // header

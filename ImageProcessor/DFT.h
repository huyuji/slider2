#pragma once
#ifndef idD6B78F05_A6C4_42C9_93ACD735718FA861
#define idD6B78F05_A6C4_42C9_93ACD735718FA861

#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class DFT : public Operation
    {
    public:
        DFT(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        cv::Mat getMask(unsigned int rows, unsigned int cols);
        void applyMask(cv::Mat& mat, const cv::Mat& mask);

        void denormalizeParams();

        double m_kmask;
        double m_rmax;
        double m_rmin;
    };
}

#endif // header

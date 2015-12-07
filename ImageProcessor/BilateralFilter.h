#pragma once
#ifndef id6D2FD6FC_95D1_44DA_871AE489BE486224
#define id6D2FD6FC_95D1_44DA_871AE489BE486224

#include <string>
#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class BilateralFilter : public Operation
    {
    public:
        BilateralFilter(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_ksize;
        double m_sigc;
        double m_sigx;
    };
}

#endif // header

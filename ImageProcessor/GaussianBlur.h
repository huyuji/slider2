#pragma once
#ifndef idBA2E79BA_A355_49E8_8FE8D7A44594D3B3
#define idBA2E79BA_A355_49E8_8FE8D7A44594D3B3

#include <string>
#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class GaussianBlur : public Operation
    {
    public:
        GaussianBlur(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_alpha;
        double m_beta;
        double m_ksize;
        double m_sigc;
        double m_sigx;
    };
}

#endif // header

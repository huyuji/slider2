#pragma once
#ifndef idB9570046_FA43_434F_8A48AEFCA62BECC1
#define idB9570046_FA43_434F_8A48AEFCA62BECC1

#include <string>
#include <boost/property_tree/ptree.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class Sharpen : public Operation
    {
    public:
        Sharpen(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        void denormalizeParams();

        double m_alpha;
        double m_beta;
        double m_ksize;
        double m_sigc;
        double m_sigx;
        std::string m_blur;
    };
}

#endif // header

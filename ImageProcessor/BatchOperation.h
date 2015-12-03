#pragma once
#ifndef id3796B6D8_A114_49C8_8073C0CC9CC73FC4
#define id3796B6D8_A114_49C8_8073C0CC9CC73FC4

#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>

#include "Operation.h"

namespace ImageProcessor
{
    class BatchOperation : public Operation
    {
    public:
        BatchOperation(const boost::property_tree::ptree& params);
        virtual cv::Mat operator()(const cv::Mat& image);

    private:
        std::vector<boost::shared_ptr<Operation> > m_operations;
    };
}

#endif // header

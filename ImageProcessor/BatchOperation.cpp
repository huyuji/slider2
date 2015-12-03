#include <boost/algorithm/string.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "BatchOperation.h"
#include "CorrectGamma.h"
#include "Contrast.h"
#include "Convert.h"
#include "Erode.h"
#include "DFT.h"
#include "Sharpen.h"
#include "Threshold.h"

using namespace ImageProcessor;

static boost::shared_ptr<Operation> CreateOperation(const std::string& name, const boost::property_tree::ptree& params)
{
    if(boost::iequals(name, "Contrast"))
    {
        return boost::shared_ptr<Operation>(new Contrast(params));
    }
    else if(boost::iequals(name, "Convert"))
    {
        return boost::shared_ptr<Operation>(new Convert(params));
    }
    else if(boost::iequals(name, "CorrectGamma"))
    {
        return boost::shared_ptr<Operation>(new CorrectGamma(params));
    }
    else if(boost::iequals(name, "Erode"))
    {
        return boost::shared_ptr<Operation>(new Erode(params));
    }
    else if(boost::iequals(name, "DFT"))
    {
        return boost::shared_ptr<Operation>(new DFT(params));
    }
    else if(boost::iequals(name, "Sharpen"))
    {
        return boost::shared_ptr<Operation>(new Sharpen(params));
    }
    else if(boost::iequals(name, "Threshold"))
    {
        return boost::shared_ptr<Operation>(new Threshold(params));
    }
    else
    {
        throw std::runtime_error("unknown image processing operation " + name);
    }
}

BatchOperation::BatchOperation(const boost::property_tree::ptree& params)
{
    for(auto it = params.begin(); it != params.end(); ++it)
    {
        m_operations.push_back(CreateOperation(it->first, it->second));
    }
}

cv::Mat BatchOperation::operator()(const cv::Mat& image)
{
    cv::Mat image_out = image;
    long long i = 0;
    for(auto it = m_operations.begin(); it != m_operations.end(); ++it)
    {
        image_out = (**it)(image_out);
    }

    return image_out;
}

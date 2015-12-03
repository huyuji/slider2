#include <opencv2/imgproc.hpp>

#include "Threshold.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

Threshold::Threshold(const boost::property_tree::ptree& params)
    : m_thresh(params.get<double>(Const::thresh))
{
    denormalizeParams();
}

void Threshold::denormalizeParams()
{

}

cv::Mat Threshold::operator()(const cv::Mat& image)
{
    Mat img_out;
    threshold(image, img_out, m_thresh, 255, THRESH_TOZERO);
    return img_out;
}

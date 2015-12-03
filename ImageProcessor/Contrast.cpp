#include "opencv2/imgproc/imgproc.hpp"

#include "Contrast.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

Contrast::Contrast(const boost::property_tree::ptree& params)
    : m_histogram(params.get<double>(Const::histogram))
{
    denormalizeParams();
}

void Contrast::denormalizeParams()
{
    m_histogram = m_histogram / 20 + 1e-10;
}

cv::Mat Contrast::operator()(const cv::Mat& image)
{
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(m_histogram);

    Mat img_out;
    clahe->apply( image, img_out);

    return img_out;
}

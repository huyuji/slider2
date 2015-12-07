#include <opencv2/imgproc.hpp>

#include "BilateralFilter.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

BilateralFilter::BilateralFilter(const boost::property_tree::ptree& params):
      m_ksize(params.get<double>(Const::ksize)),
      m_sigc(params.get<double>(Const::sigc)),
      m_sigx(params.get<double>(Const::sigx))
{
    denormalizeParams();
}

void BilateralFilter::denormalizeParams()
{
}

cv::Mat BilateralFilter::operator()(const cv::Mat& image)
{
    Mat img_out;
    bilateralFilter(image, img_out, m_ksize, m_sigc, m_sigx);

    return img_out;
}

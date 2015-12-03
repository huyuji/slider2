#include "Convert.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

Convert::Convert(const boost::property_tree::ptree& params)
    : m_bright(params.get<double>(Const::bright)),
      m_contrast(params.get<double>(Const::contrast))
{
    denormalizeParams();
}

void Convert::denormalizeParams()
{
    m_contrast /= 50;
    m_bright -= 50;
}

cv::Mat Convert::operator()(const cv::Mat& image)
{

    Mat image_out;
    image.convertTo(image_out, -1, m_contrast, m_bright);

    return image_out;
}

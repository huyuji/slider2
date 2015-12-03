#include "opencv2/imgproc/imgproc.hpp"

#include "Erode.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

Erode::Erode(const boost::property_tree::ptree& params)
    : m_erode(params.get<double>(Const::erode))
{
    denormalizeParams();
}

void Erode::denormalizeParams()
{
    m_erode /= 10;
}

cv::Mat Erode::operator()(const cv::Mat& image)
{
    Mat element = getStructuringElement(
        MORPH_CROSS,cv::Size(2 * m_erode + 1, 2 * m_erode + 1),
        Point(m_erode, m_erode));

    Mat img_out;
    cv::erode(image, img_out, element);

    return img_out;
}

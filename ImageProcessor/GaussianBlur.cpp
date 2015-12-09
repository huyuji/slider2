#include <opencv2/imgproc.hpp>

#include "GaussianBlur.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

GaussianBlur::GaussianBlur(const boost::property_tree::ptree& params)
    : m_alpha(params.get<double>(Const::alpha)),
      m_beta(params.get<double>(Const::beta)),
      m_ksize(params.get<double>(Const::ksize)),
      m_sigc(params.get<double>(Const::sigc)),
      m_sigx(params.get<double>(Const::sigx))
{
    denormalizeParams();
}

void GaussianBlur::denormalizeParams()
{
    m_alpha = m_alpha / 10.0 + 1;
    m_beta = -1 * m_beta / 20.0;
    m_ksize = m_ksize * 2 + 1;
}

cv::Mat GaussianBlur::operator()(const cv::Mat& image)
{
    Mat img_out;
    Mat img_blur;
    cv::GaussianBlur(image, img_blur, Size(m_ksize, m_ksize), m_sigc, m_sigx, BORDER_DEFAULT);
    addWeighted(image, m_alpha, img_blur, m_beta, 0.0, img_out);
    img_blur.release();

    return img_out;
}

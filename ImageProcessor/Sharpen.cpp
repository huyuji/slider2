#include <opencv2/imgproc.hpp>

#include "Sharpen.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

Sharpen::Sharpen(const boost::property_tree::ptree& params)
    : m_alpha(params.get<double>(Const::alpha)),
      m_beta(params.get<double>(Const::beta)),
      m_ksize(params.get<double>(Const::ksize)),
      m_sigc(params.get<double>(Const::sigc)),
      m_sigx(params.get<double>(Const::sigx)),
      m_blur(params.get<std::string>(Const::blur))
{
    denormalizeParams();
}

void Sharpen::denormalizeParams()
{
    m_alpha = m_alpha / 10.0 + 1;
    m_beta = -1 * m_beta / 10.0;
}

cv::Mat Sharpen::operator()(const cv::Mat& image)
{
    Mat img_out;
    if(m_blur == Const::GaussianBlur)
    {
        Mat img_blur;
        GaussianBlur(image, img_blur, Size(m_ksize, m_ksize), m_sigc, m_sigx, BORDER_DEFAULT);
        addWeighted(image, m_alpha, img_blur, m_beta, 0.0, img_out);
        img_blur.release();

    }
    else if(m_blur == Const::BilateralFilter)
    {
        bilateralFilter(image, img_out, m_ksize, m_sigc, m_sigx);
    }

    return img_out;
}

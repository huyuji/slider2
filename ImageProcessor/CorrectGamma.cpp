#include "CorrectGamma.h"
#include "Const.h"

using namespace ImageProcessor;
using namespace cv;

CorrectGamma::CorrectGamma(const boost::property_tree::ptree& params)
    : m_gamma(params.get<double>(Const::gamma))
{
    denormalizeParams();
}

void CorrectGamma::denormalizeParams()
{
    m_gamma /= 50;
}

cv::Mat CorrectGamma::operator()(const cv::Mat& image)
{
    double inverse_gamma = 1 / (m_gamma + 1e-20);

    Mat lut_matrix(1, 256, CV_8UC1 );
    uchar * ptr = lut_matrix.ptr();
    for(int i = 0; i < 256; i++)
    {
        ptr[i] = (int)(pow((double)i / 255.0, inverse_gamma) * 255.0);
    }

    Mat img_out;
    LUT(image, lut_matrix, img_out);

    return img_out;
}

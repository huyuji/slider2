#pragma once
#ifndef id04BE696B_A111_482B_A8CC2673956D1EA1
#define id04BE696B_A111_482B_A8CC2673956D1EA1

#define ImageProcessor_API

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <opencv2/core.hpp>

extern "C"
{
    ImageProcessor_API cv::Mat ProcessImage(
        const cv::Mat& image,
        const boost::property_tree::ptree& operations);

    ImageProcessor_API void getOperations(
        const std::string& configFolder,
        const std::string& deviceName,
        const std::string& configName,
        boost::property_tree::ptree& operations);

    ImageProcessor_API cv::Mat OpenImage(const std::string& imageFile);

    ImageProcessor_API void SaveImage(const cv::Mat& image, const std::string& outputPath);
}

#endif // header

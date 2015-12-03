// ImageProcessor.cpp : Defines the exported functions for the DLL application.
//

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ImageProcessor.h"
#include "BatchOperation.h"
#include "ImageConfig.h"

using namespace ImageProcessor;

ImageProcessor_API cv::Mat ProcessImage(
    const cv::Mat& image,
    const boost::property_tree::ptree& operations)
{
    return BatchOperation(operations)(image);
}

ImageProcessor_API void getOperations(
    const std::string& configFolder,
    const std::string& deviceName,
    const std::string& configName,
    boost::property_tree::ptree& operations)
{
    operations = *ImageConfig(configFolder).getOperations(deviceName, configName);
}

ImageProcessor_API cv::Mat OpenImage(const std::string& imageFile)
{
    return cv::imread(imageFile, CV_LOAD_IMAGE_GRAYSCALE);
}

ImageProcessor_API void SaveImage(const cv::Mat& image, const std::string& outputPath)
{
    cv::imwrite(outputPath, image);
}

#pragma once
#ifndef id36092F60_1AF5_40FC_9D2BB4AAFF616AE9
#define id36092F60_1AF5_40FC_9D2BB4AAFF616AE9

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/shared_ptr.hpp>

namespace ImageProcessor
{
    class ImageConfig
    {
    public:
        ImageConfig(const std::string& configFolder) : m_configFolder(configFolder) {};
        boost::shared_ptr<boost::property_tree::ptree> getOperations(const std::string& deviceName, const std::string& configName);

    private:
        std::string getConfigFilePath(const std::string& deviceName);
        boost::shared_ptr<boost::property_tree::ptree> getOperationsFromFile(const std::string& configFile, const std::string& configName);

        const std::string m_configFolder;
    };
}

#endif // header

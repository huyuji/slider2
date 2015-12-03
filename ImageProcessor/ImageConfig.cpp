#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "ImageConfig.h"
#include "Const.h"

using namespace ImageProcessor;

std::string ImageConfig::getConfigFilePath(const std::string& deviceName)
{
    try
    {
        boost::filesystem::path dir(m_configFolder);
        boost::filesystem::recursive_directory_iterator itr(dir), itr_end;
        std::string targetName = deviceName + Const::CONFIGFILEEXT;

        while (itr != itr_end) {
            if (boost::filesystem::is_regular_file(*itr))
            {
                const std::string fileName = itr->path().filename().string();
                if (boost::iequals(fileName, targetName))
                {
                    return itr->path().string();
                }
            }
            itr++;
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("exception caught when finding configuration file for " + deviceName + ". " + e.what());
    }

    return std::string();
}

boost::shared_ptr<boost::property_tree::ptree> ImageConfig::getOperationsFromFile(
    const std::string& configFile, const std::string& configName)
{
    using namespace boost::property_tree;

    try
    {
        ptree configurations;
        read_json(configFile, configurations);
        configurations = configurations.get_child(Const::CONFIG_CONFIGURATIONS);

        // use the default if configuration name is not specified
        if(configName.empty())
        {
            for(auto it = configurations.begin(); it != configurations.end(); ++it)
            {
                const ptree& config = it->second;
                if(config.get<bool>(Const::CONFIG_ISDEFAULT))
                {
                    return boost::shared_ptr<ptree>(new ptree(std::move(config.get_child(Const::CONFIG_OPERATIONS))));
                }
            }
        }
        else
        {
            for(auto it = configurations.begin(); it != configurations.end(); ++it)
            {
                const ptree& config = it->second;
                const std::string name = config.get<std::string>(Const::CONFIG_NAME);
                if(boost::iequals(name, configName))
                {
                    return boost::shared_ptr<ptree>(new ptree(std::move(config.get_child(Const::CONFIG_OPERATIONS))));
                }
            }
        }
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error("exception caught when finding configuration " + configName + ". " + e.what());
    }

    return boost::shared_ptr<ptree>();
}

boost::shared_ptr<boost::property_tree::ptree> ImageConfig::getOperations(
    const std::string& deviceName, const std::string& configName)
{
    const std::string configFile = getConfigFilePath(deviceName);
    if(configFile.empty())
    {
        throw std::runtime_error("no configuration file for " + deviceName + " in " + m_configFolder);
    }

    boost::shared_ptr<boost::property_tree::ptree> operations = getOperationsFromFile(configFile, configName);
    if(!operations)
    {
        throw std::runtime_error("failed to find configuration " + configName + " for " + deviceName + " in " + configFile );
    }

    return operations;
}

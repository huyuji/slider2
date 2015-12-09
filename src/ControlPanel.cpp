#include "ControlPanel.h"

#include <fstream>
#include <vector>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm//string.hpp>
#include <boost/filesystem.hpp>
#include "Const.h"
#include "OperationControl.h"
#include "util.h"

using boost::property_tree::ptree;

ControlPanel::ControlPanel(ptree& operations)
    : m_changeSaved(true), m_configurations(nullptr), m_operations(nullptr)

{
    m_buttonLoad = new QPushButton("load");
    connect(m_buttonLoad, SIGNAL(clicked()), this, SLOT(loadConfigFile()));

    m_configurationList = new QComboBox();
    m_configurationList->setInsertPolicy(QComboBox::InsertAlphabetically);

    m_buttonSave = new QPushButton("save");
    m_buttonSave->setDisabled(m_changeSaved);
    connect(m_buttonSave, SIGNAL(clicked()), this, SLOT(save()));

    m_buttonNew = new QPushButton("new");
    connect(m_buttonNew, SIGNAL(clicked()), this, SLOT(newConfig()));

    m_operationList = new QComboBox();
    m_operationList->setInsertPolicy(QComboBox::InsertAlphabetically);
    m_operationList->addItem("Contrast");
    m_operationList->addItem("Convert");
    m_operationList->addItem("CorrectGamma");
    m_operationList->addItem("DFT");
    m_operationList->addItem("Erode");
    m_operationList->addItem("GaussianBlur");
    m_operationList->addItem("BilateralFilter");
    m_operationList->addItem("Threshold");

    m_buttonAddOperation = new QPushButton("add processing");
    connect( m_buttonAddOperation, SIGNAL( clicked() ), this, SLOT(addOperation()) );
    m_buttonAddOperation->setDisabled(true);

    m_buttonLine1 = new QHBoxLayout();
    m_buttonLine1->addWidget(m_buttonLoad, 0, Qt::AlignLeft);
    m_buttonLine1->addWidget(m_configurationList, 1);
    m_buttonLine1->addWidget(m_buttonSave, 0);
    m_buttonLine1->addWidget(m_buttonNew, 0);

    m_buttonLine2 = new QHBoxLayout();
    m_buttonLine2->addWidget(m_operationList, 1);
    m_buttonLine2->addWidget(m_buttonAddOperation, 0);

    m_operationLayout = new QVBoxLayout();

    m_layout = new QVBoxLayout();
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->addLayout(m_buttonLine1);
    m_layout->addLayout(m_buttonLine2);
    m_layout->addLayout(m_operationLayout);
    setLayout(m_layout);
}

void ControlPanel::addOperation()
{
    std::string opName = m_operationList->currentText().toStdString();
    m_operations->add_child(opName, ptree());
    addOperation(opName, m_operations->back().second);
    operationValueChanged();
}

void ControlPanel::save()
{
    saveToFile();
}

bool ControlPanel::saveToFile()
{
    if(m_changeSaved)
    {
        return true;
    }

    if(!m_configFilePath.empty())
    {
        QMessageBox msgBox;
        msgBox.setText("Overwrite?");
        msgBox.setInformativeText("Do you want to overwrite current config file?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        const int ret = msgBox.exec();
        if(ret == QMessageBox::Cancel)
        {
            return false;
        }
        else if(ret == QMessageBox::Yes)
        {
            saveToFile(m_configFilePath);
            return true;
        }
    }

    QFileDialog dialog(this, tr("Save to Config File"), QString(), tr("Json File (*.json)"));
    if(dialog.exec() == QDialog::Accepted)
    {
        m_configFilePath = dialog.selectedFiles().first().toStdString();
        if(boost::filesystem::path(m_configFilePath).extension() != ".json")
        {
            m_configFilePath += ".json";
        }
        saveToFile(m_configFilePath);
        return true;
    }

    return false;
}

void ControlPanel::saveToFile(const std::string& file)
{
    std::ofstream json(file);
    boost::property_tree::write_json(json, m_root, true);
    m_changeSaved = true;
    m_buttonSave->setDisabled(m_changeSaved);
}

void ControlPanel::clear()
{
    m_configFilePath.clear();
    m_changeSaved = true;
    m_buttonSave->setDisabled(m_changeSaved);
    m_root.clear();
    m_configurations = nullptr;
    m_operations = nullptr;
    ClearLayout(m_operationLayout);
}

bool ControlPanel::saveChange()
{
    if(m_changeSaved)
    {
        return true;
    }

    QMessageBox msgBox;
    msgBox.setInformativeText("You have unsaved changes. Do you want to keep them?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    const int ret = msgBox.exec();
    if(ret == QMessageBox::Save)
    {
        return saveToFile();
    }
    else if(ret == QMessageBox::Discard)
    {
        clear();
        return true;
    }
    else if(ret == QMessageBox::Cancel)
    {
        return false;
    }
    else
    {
        throw std::runtime_error("program should not reach here");
    }
}

void ControlPanel::initRoot()
{
    m_root.clear();
    m_root.add_child(ImageProcessor::Const::CONFIG_CONFIGURATIONS, ptree());
    m_configurations = &m_root.get_child(ImageProcessor::Const::CONFIG_CONFIGURATIONS);
}

void ControlPanel::newConfiguration(const std::string& configName)
{
    for(auto it = m_configurations->begin(); it != m_configurations->end(); ++it)
    {
        const ptree& config = it->second;
        std::string name = config.get<std::string>(ImageProcessor::Const::CONFIG_NAME);
        if(boost::iequals(name, configName))
        {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                        tr("There is already a configuration named %1.").arg(QDir::toNativeSeparators(configName.c_str())));
            return;
        }
    }

    m_configurations->push_back(ptree::value_type("", ptree()));
    ptree& config = m_configurations->back().second;
    config.put(ImageProcessor::Const::CONFIG_NAME, configName);
    config.put(ImageProcessor::Const::CONFIG_ISDEFAULT, false);
    config.add_child(ImageProcessor::Const::CONFIG_OPERATIONS, ptree());
    m_operations = &config.get_child(ImageProcessor::Const::CONFIG_OPERATIONS);
    ClearLayout(m_operationLayout);
    m_buttonAddOperation->setDisabled(false);
}

void ControlPanel::newConfig()
{
    bool newConfigFile = true;

    if(!m_configFilePath.empty())
    {
        QMessageBox msgBox;
        msgBox.setInformativeText("Create new configuration in current config file?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        const int ret = msgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            newConfigFile = false;
        }
        else if(ret == QMessageBox::No)
        {
            if(!saveChange())
            {
                return;
            }

            newConfigFile = true;
        }
        else if(ret == QMessageBox::Cancel)
        {
            return;
        }
    }

    bool ok;
    QString text = QInputDialog::getText(this, tr(""), tr("Name the new configuration:"), QLineEdit::Normal, QString(), &ok);
    if (ok && !text.isEmpty())
    {
        if(newConfigFile)
        {
            clear();
            initRoot();
        }
        
        newConfiguration(text.toStdString());
        m_configurationList->addItem(text);
        m_configurationList->setCurrentText(text);
    }
}

void ControlPanel::operationValueChanged()
{
    m_changeSaved = false;
    m_buttonSave->setDisabled(m_changeSaved);
    emit operationChanged();
}

void ControlPanel::loadConfigFile()
{
    if(!saveChange())
    {
        return;
    }

    QFileDialog dialog(this, tr("Open Config File"), QString(), tr("Json File (*.json)"));
    while (dialog.exec() == QDialog::Accepted && !readConfigFile(dialog.selectedFiles().first().toStdString())) {}
}

bool ControlPanel::readConfigFile(const std::string& configFilePath)
{
    try
    {
        read_json(configFilePath, m_root);
        m_configurations = &m_root.get_child(ImageProcessor::Const::CONFIG_CONFIGURATIONS);
        if(m_configurations->empty())
        {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("No configuration found in %1.").arg(QDir::toNativeSeparators(configFilePath.c_str())));
            clear();
            return false;
        }

        m_configFilePath = configFilePath;
        loadConfigurations();
    }
    catch(std::exception& e)
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(configFilePath.c_str())));
        clear();
        return false;
    }

    return true;
}

void ControlPanel::loadConfigurations()
{
    std::string defaultConfigName;
    std::vector<std::string> configNames;
    for(auto it = m_configurations->begin(); it != m_configurations->end(); ++it)
    {
        const ptree& config = it->second;
        std::string name = config.get<std::string>(ImageProcessor::Const::CONFIG_NAME);
        bool isDefault = config.get<bool>(ImageProcessor::Const::CONFIG_ISDEFAULT);
        if(isDefault)
        {
            defaultConfigName = name;
        }
        configNames.push_back(name);
    }

    refreshConfiguraionList(configNames, defaultConfigName);
    loadConfiguration(m_configurationList->currentText());
}

void ControlPanel::refreshConfiguraionList(const std::vector<std::string>& configNames, const std::string& currentConfig)
{
    disconnect(m_configurationList, SIGNAL(currentTextChanged(const QString &)));

    m_configurationList->clear();
    for(auto it = configNames.begin(); it != configNames.end(); ++it)
    {
        m_configurationList->addItem(it->c_str());
    }
    if(!currentConfig.empty())
    {
        m_configurationList->setCurrentText(currentConfig.c_str());
    }

    connect(m_configurationList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(loadConfiguration(const QString &)));
}

void ControlPanel::loadConfiguration(const QString& configName)
{
    for(auto it = m_configurations->begin(); it != m_configurations->end(); ++it)
    {
        ptree& config = it->second;
        const std::string name = config.get<std::string>(ImageProcessor::Const::CONFIG_NAME);
        if(boost::iequals(name, configName.toStdString()))
        {
            ClearLayout(m_operationLayout);
            m_operations = &config.get_child(ImageProcessor::Const::CONFIG_OPERATIONS);
            loadOperations();
            m_buttonAddOperation->setDisabled(false);
            return;
        }
    }

    throw std::runtime_error(("configuration " + configName.toStdString() + " not found"));
}

void ControlPanel::loadOperations()
{
    for(auto it = m_operations->begin(); it != m_operations->end(); ++it)
    {
        addOperation(it->first, it->second);
    }
}

void ControlPanel::addOperation(const std::string& operationName, boost::property_tree::ptree& parameters)
{
    OperationControl* operationControl = OperationControl::CreateOperationControl(operationName, parameters);
    connect(operationControl, SIGNAL(valueChanged()), this, SLOT(operationValueChanged()));
    m_operationLayout->addWidget(operationControl);
}

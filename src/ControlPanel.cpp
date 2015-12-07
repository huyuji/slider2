#include "ControlPanel.h"

#include <fstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm//string.hpp>
#include <boost/filesystem.hpp>
#include "Const.h"
#include "OperationControl.h"
#include "util.h"

using boost::property_tree::ptree;

ControlPanel::ControlPanel(ptree& operations)
    : m_operations(operations), m_operationLayout(nullptr), m_changeSaved(true)
{
    m_buttonLoad = new QPushButton("load");
    connect(m_buttonLoad, SIGNAL(clicked()), this, SLOT(loadConfigFile()));

    m_configurationList = new QComboBox();
    m_configurationList->setInsertPolicy(QComboBox::InsertAlphabetically);

    m_buttonSave = new QPushButton("save");
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

    m_buttonLine1 = new QHBoxLayout();
    m_buttonLine1->addWidget(m_buttonLoad, 0, Qt::AlignLeft);
    m_buttonLine1->addWidget(m_configurationList, 1);
    m_buttonLine1->addWidget(m_buttonSave, 0);
    m_buttonLine1->addWidget(m_buttonNew, 0);

    m_buttonLine2 = new QHBoxLayout();
    m_buttonLine2->addWidget(m_operationList, 1);
    m_buttonLine2->addWidget(m_buttonAddOperation, 0);

    m_layout = new QVBoxLayout();
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->addLayout(m_buttonLine1);
    m_layout->addLayout(m_buttonLine2);
    setLayout(m_layout);
}

void ControlPanel::addOperation()
{
    //addOperation(m_operationLayout, m_operationList->currentText());
}

void ControlPanel::addOperation(QVBoxLayout* layout, const QString& operationName, boost::property_tree::ptree& parameters)
{
    OperationControl* operationControl = OperationControl::CreateOperationControl(operationName, parameters);
    connect(operationControl, SIGNAL(valueChanged()), this, SLOT(operationValueChanged()));
    layout->addWidget(operationControl);
}

void ControlPanel::save()
{
    if(m_changeSaved)
    {
        return;
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
            return;
        }
        else if(ret == QMessageBox::Yes)
        {
            saveToFile(m_configFilePath);
            return;
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
    }
}

void ControlPanel::saveToFile(const std::string& file)
{
    std::ofstream json(file);
    boost::property_tree::write_json(json, m_configurations, true);
    m_changeSaved = true;
}

void ControlPanel::newConfig()
{
    bool newConfigFile = false;

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
            newConfigFile = true;
        }
        else if(ret == QMessageBox::Cancel)
        {
            return;
        }
    }

    if(newConfigFile)
    {

    }

    //bool ok;
    //QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
    //                                     tr("User name:"), QLineEdit::Normal,
    //                                     QDir::home().dirName(), &ok);
    //if (ok && !text.isEmpty())
    //    textLabel->setText(text);


    clearOperations();
    m_operationLayout = new QVBoxLayout();
}

void ControlPanel::operationValueChanged()
{
    m_changeSaved = false;
    emit valueChanged();
}

void ControlPanel::loadConfigFile()
{
    QFileDialog dialog(this, tr("Open Config File"), QString(), tr("Json File (*.json)"));
    ptree configurations;
    while (dialog.exec() == QDialog::Accepted && !readConfigFile(dialog.selectedFiles().first(), configurations)) {}
}

bool ControlPanel::readConfigFile(const QString &fileName, ptree& configurations)
{
    try
    {
        read_json(fileName.toStdString(), configurations);
        configurations = configurations.get_child(ImageProcessor::Const::CONFIG_CONFIGURATIONS);
        if(configurations.empty())
        {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("No configuration found in %1.").arg(QDir::toNativeSeparators(fileName)));
            return false;
        }
        else
        {
            loadConfigurations(configurations);
            m_configFilePath = fileName.toStdString();
        }
    }
    catch(std::exception& e)
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        return false;
    }

    return true;
}

void ControlPanel::loadConfigurations(const boost::property_tree::ptree& configurations)
{
    disconnect(m_configurationList, SIGNAL(currentTextChanged(const QString &)));
    m_configurationList->clear();
    std::string defaultConfigName;
    for(auto it = configurations.begin(); it != configurations.end(); ++it)
    {
        const ptree& config = it->second;
        std::string name = config.get<std::string>(ImageProcessor::Const::CONFIG_NAME);
        bool isDefault = config.get<bool>(ImageProcessor::Const::CONFIG_ISDEFAULT);
        if(isDefault)
        {
            defaultConfigName = name;
        }
        m_configurationList->addItem(name.c_str());
    }
    if(!defaultConfigName.empty())
    {
        m_configurationList->setCurrentText(defaultConfigName.c_str());
    }

    m_configurations = configurations;
    loadConfiguration(m_configurationList->currentText());
    connect(m_configurationList, SIGNAL(currentTextChanged(const QString &)), this, SLOT(loadConfiguration(const QString &)));
}

void ControlPanel::loadConfiguration(const QString& operationName)
{
    for(auto it = m_configurations.begin(); it != m_configurations.end(); ++it)
    {
        ptree& config = it->second;
        const std::string name = config.get<std::string>(ImageProcessor::Const::CONFIG_NAME);
        if(boost::iequals(name, operationName.toStdString()))
        {
            loadOperations(config.get_child(ImageProcessor::Const::CONFIG_OPERATIONS));
            return;
        }
    }

    throw std::runtime_error(("configuration " + operationName + " not found").toStdString());
}

void ControlPanel::loadOperations(boost::property_tree::ptree& operations)
{
    QVBoxLayout* layout = new QVBoxLayout();
    for(auto it = operations.begin(); it != operations.end(); ++it)
    {
        addOperation(layout, it->first.c_str(), it->second);
    }

    clearOperations();
    m_operationLayout = layout;
    m_layout->addLayout(m_operationLayout);
    m_operations = operations;
    emit valueChanged();
}

void ControlPanel::clearOperations()
{
    if(m_operationLayout)
    {
        DeleteLayout(m_operationLayout);
        m_layout->removeItem(m_operationLayout);
    }
    m_operationLayout = nullptr;
}
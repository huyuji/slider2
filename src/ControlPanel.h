#pragma once
#ifndef idC81AB1C5_FCD9_43D0_8F949A86835F4865
#define idC81AB1C5_FCD9_43D0_8F949A86835F4865

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    ControlPanel(const boost::property_tree::ptree *& operations);
    ~ControlPanel();

signals:
    void configFileChanged();
    void configSelectionChanged();
    void configChanged();
    void operationChanged();

private slots:
    void addOperation();
    void save();
    void newConfig();
    void operationValueChanged();
    void loadConfigFile();
    void loadConfiguration(const QString& configName);
    void deleteOperation(QWidget*);

private:
    QVBoxLayout* m_layout;
        QHBoxLayout* m_buttonLine1;
            QPushButton* m_buttonLoad;
            QComboBox* m_configurationList;
            QPushButton* m_buttonSave;
            QPushButton* m_buttonNew;
        QHBoxLayout* m_buttonLine2;
            QComboBox* m_operationList;
            QPushButton* m_buttonAddOperation;
        QVBoxLayout* m_operationLayout;

    std::string m_configFilePath;
    bool m_changeSaved;
    boost::property_tree::ptree m_root;
    boost::property_tree::ptree* m_configurations;
    boost::property_tree::ptree* m_operations;
    const boost::property_tree::ptree *& m_output;

    bool readConfigFile(const std::string& configFilePath);
    void loadConfigurations();
    void refreshConfiguraionList(const std::vector<std::string>& configNames, const std::string& currentConfig);
    void loadOperations();
    void addOperation(const std::string& operationName, boost::property_tree::ptree& parameters);

    bool saveToFile();
    void saveToFile(const std::string& file);
    bool saveChange();
    void clear();
    void initRoot();
    void newConfiguration(const std::string& configName);
};

#endif // header

#pragma once
#ifndef idC81AB1C5_FCD9_43D0_8F949A86835F4865
#define idC81AB1C5_FCD9_43D0_8F949A86835F4865

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    ControlPanel(boost::property_tree::ptree& operations);

signals:
    void valueChanged();

private slots:
    void addOperation();
    void save();
    void newConfig();
    void operationValueChanged();
    void loadConfigFile();
    void loadConfiguration(const QString& operationName);

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
    boost::property_tree::ptree m_configurations;
    boost::property_tree::ptree& m_operations;

    void addOperation(const QString& operationName);
    void addOperation(QVBoxLayout* layout, const QString& operationName, boost::property_tree::ptree& parameters);

    bool readConfigFile(const QString &fileName, boost::property_tree::ptree& configurations);
    void loadConfigurations(const boost::property_tree::ptree& configurations);
    void loadOperations(boost::property_tree::ptree& operations);
    void clearOperations();

    void saveToFile(const std::string& file);
};

#endif // header

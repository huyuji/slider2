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
    void saveOperations();
    void operationValueChanged();
    void loadConfigFile();

private:
    QPushButton* m_buttonLoad;
    QPushButton* m_buttonSave;

    QComboBox* m_operationList;
    QPushButton* m_buttonAddOperation;

    QGridLayout* m_gridLayout;

    boost::property_tree::ptree& m_operations;

    void addOperation(const QString& operationName);
    bool readConfigFile(const QString &fileName, boost::property_tree::ptree& configurations);
};

#endif // header

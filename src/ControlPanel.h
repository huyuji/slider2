#pragma once
#ifndef idC81AB1C5_FCD9_43D0_8F949A86835F4865
#define idC81AB1C5_FCD9_43D0_8F949A86835F4865

#include <QtWidgets>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    ControlPanel();

private slots:
    void addOperation();

private:
    QPushButton* m_buttonLoad;
    QPushButton* m_buttonSave;

    QComboBox* m_operationList;
    QPushButton* m_buttonAddOperation;

    QGridLayout* m_gridLayout;

    void addOperation(const QString& operationName);
};

#endif // header

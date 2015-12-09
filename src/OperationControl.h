#pragma once
#ifndef idB6191E8A_5691_4BBF_B66B745335976433
#define idB6191E8A_5691_4BBF_B66B745335976433

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>
#include "SliderControl.h"

class OperationControl : public QWidget
{
    Q_OBJECT

public:
    static OperationControl* CreateOperationControl(const std::string& operationName, boost::property_tree::ptree& parameters);
    ~OperationControl();

signals:
    void valueChanged();
    void operationDeleted(QWidget*);

protected:
    OperationControl(const std::string& name);

    void addSlider(const std::string& parameterName, boost::property_tree::ptree& parameters, int min = Min, int max = Max, unsigned int step = Step, unsigned int page = Page);

private slots:
    void sliderValueChanged();
    void close();

private:
    static const int Min = 0;
    static const int Max = 100;
    static const unsigned int Step = 1;
    static const unsigned int Page = 5;

    static const unsigned int ButtonWidth = 20;

    QHBoxLayout* m_layout;
        QVBoxLayout* m_buttonLayout;
            QPushButton* m_buttonClose;
            QPushButton* m_buttonDrag;
        QGroupBox* m_box;
            QVBoxLayout* m_boxLayout;

    const std::string m_name;
};

//class ContrastControl : public OperationControl
//{
//    Q_OBJECT
//
//public:
//    ContrastControl();
//};

#endif // header

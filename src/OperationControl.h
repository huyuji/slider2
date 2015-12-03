#pragma once
#ifndef idB6191E8A_5691_4BBF_B66B745335976433
#define idB6191E8A_5691_4BBF_B66B745335976433

#include <QtWidgets>
#include "SliderControl.h"

class OperationControl : public QGroupBox
{
    Q_OBJECT

public:
    static OperationControl* CreateOperationControl(const QString& operationName);

protected:
    OperationControl(const QString& name);

    void addSlider(const QString& parameterName, int min = Min, int max = Max, unsigned int step = Step);

private:
    static const int Min = 0;
    static const int Max = 100;
    static const unsigned int Step = 1;

    std::vector<SliderControl*> m_sliderControls;
    QVBoxLayout* m_vbox;
    const QString m_name;
};

//class ContrastControl : public OperationControl
//{
//    Q_OBJECT
//
//public:
//    ContrastControl();
//};

#endif // header

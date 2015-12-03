#include "OperationControl.h"
#include <exception>

OperationControl* OperationControl::CreateOperationControl(const QString& operationName)
{
    OperationControl* operationControl = new OperationControl(operationName);

    if(operationName == "Contrast")
    {
        operationControl->addSlider("histogram");
    }
    else if(operationName == "Convert")
    {
        operationControl->addSlider("bright");
        operationControl->addSlider("contrast");
    }
    else if(operationName == "CorrectGamma")
    {
        operationControl->addSlider("gamma");
    }
    else if(operationName == "DFT")
    {
        operationControl->addSlider("rmax");
    }
    else if(operationName == "Erode")
    {
        operationControl->addSlider("erode");
    }
    else if(operationName == "Sharpen")
    {

    }
    else if(operationName == "Threshold")
    {
        operationControl->addSlider("thresh");
    }
    else
    {
        throw std::runtime_error(("unknown operation " + operationName).toStdString());
    }

    return operationControl;
}

OperationControl::OperationControl(const QString& name)
    : m_vbox(new QVBoxLayout()), m_name(name)
{
    setTitle(name);
}

void OperationControl::addSlider(const QString& parameterName, int min, int max, unsigned int step)
{
    SliderControl* sliderControl = new SliderControl(parameterName, min, max, step);
    m_vbox->addWidget(sliderControl);
    m_sliderControls.push_back(sliderControl);
    setLayout(m_vbox);
}
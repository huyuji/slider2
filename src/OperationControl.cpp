#include "OperationControl.h"
#include <exception>

using boost::property_tree::ptree;

OperationControl* OperationControl::CreateOperationControl(const QString& operationName, ptree& operations)
{
    OperationControl* operationControl = new OperationControl(operationName);
    operations.push_back(ptree::value_type(operationName.toStdString(), ptree()));
    ptree& operation = operations.back().second;

    if(operationName == "Contrast")
    {
        operationControl->addSlider("histogram", operation);
    }
    else if(operationName == "Convert")
    {
        operationControl->addSlider("bright", operation);
        operationControl->addSlider("contrast", operation);
    }
    else if(operationName == "CorrectGamma")
    {
        operationControl->addSlider("gamma", operation);
    }
    else if(operationName == "DFT")
    {
        operationControl->addSlider("rmax", operation);
    }
    else if(operationName == "Erode")
    {
        operationControl->addSlider("erode", operation);
    }
    else if(operationName == "Sharpen")
    {

    }
    else if(operationName == "Threshold")
    {
        operationControl->addSlider("thresh", operation);
    }
    else
    {
        throw std::runtime_error("unknown operation " + operationName.toStdString());
    }

    return operationControl;
}

OperationControl::OperationControl(const QString& name)
    : m_vbox(new QVBoxLayout()), m_name(name)
{
    setTitle(name);
}

void OperationControl::addSlider(const QString& parameterName, ptree& operation, int min, int max, unsigned int step)
{
    operation.push_back(ptree::value_type(parameterName.toStdString(), ptree("0")));
    ptree& parameter = operation.back().second;

    SliderControl* sliderControl = new SliderControl(parameterName, parameter, min, max, step);
    connect(sliderControl, SIGNAL(valueChanged()), this, SLOT(sliderValueChanged()));
    m_vbox->addWidget(sliderControl);
    m_sliderControls.push_back(sliderControl);
    setLayout(m_vbox);
}

void OperationControl::sliderValueChanged()
{
    emit valueChanged();
}
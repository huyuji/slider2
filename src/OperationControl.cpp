#include "OperationControl.h"
#include <exception>

using boost::property_tree::ptree;

OperationControl* OperationControl::CreateOperationControl(const QString& operationName, ptree& parameters)
{
    OperationControl* operationControl = new OperationControl(operationName);

    if(operationName == "Contrast")
    {
        operationControl->addSlider("histogram", parameters);
    }
    else if(operationName == "Convert")
    {
        operationControl->addSlider("bright", parameters);
        operationControl->addSlider("contrast", parameters);
    }
    else if(operationName == "CorrectGamma")
    {
        operationControl->addSlider("gamma", parameters);
    }
    else if(operationName == "DFT")
    {
        operationControl->addSlider("rmax", parameters);
    }
    else if(operationName == "Erode")
    {
        operationControl->addSlider("erode", parameters);
    }
    else if(operationName == "Sharpen")
    {

    }
    else if(operationName == "Threshold")
    {
        operationControl->addSlider("thresh", parameters);
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

void OperationControl::addSlider(const QString& parameterName, ptree& parameters, int min, int max, unsigned int step)
{
    boost::optional<ptree&> parameter = parameters.get_child_optional(parameterName.toStdString());
    if(!parameter)
    {
        parameters.push_back(ptree::value_type(parameterName.toStdString(), ptree("0")));
        parameter = parameters.back().second;
    }

    SliderControl* sliderControl = new SliderControl(parameterName, *parameter, min, max, step);
    connect(sliderControl, SIGNAL(valueChanged()), this, SLOT(sliderValueChanged()));
    m_vbox->addWidget(sliderControl);
    m_sliderControls.push_back(sliderControl);
    setLayout(m_vbox);
}

void OperationControl::sliderValueChanged()
{
    emit valueChanged();
}
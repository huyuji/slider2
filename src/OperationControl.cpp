#include "OperationControl.h"
#include <exception>
#include "Const.h"

using boost::property_tree::ptree;

OperationControl* OperationControl::CreateOperationControl(const std::string& operationName, ptree& parameters)
{
    OperationControl* operationControl = new OperationControl(operationName);

    if(operationName == "Contrast")
    {
        operationControl->addSlider(ImageProcessor::Const::histogram, parameters);
    }
    else if(operationName == "Convert")
    {
        operationControl->addSlider(ImageProcessor::Const::bright, parameters);
        operationControl->addSlider(ImageProcessor::Const::contrast, parameters);
    }
    else if(operationName == "CorrectGamma")
    {
        operationControl->addSlider(ImageProcessor::Const::gamma, parameters);
    }
    else if(operationName == "DFT")
    {
        operationControl->addSlider(ImageProcessor::Const::rmax, parameters);
    }
    else if(operationName == "Erode")
    {
        operationControl->addSlider(ImageProcessor::Const::erode, parameters);
    }
    else if(operationName == "GaussianBlur")
    {
        operationControl->addSlider(ImageProcessor::Const::alpha, parameters);
        operationControl->addSlider(ImageProcessor::Const::beta, parameters);
        operationControl->addSlider(ImageProcessor::Const::ksize, parameters);
        operationControl->addSlider(ImageProcessor::Const::sigc, parameters);
        operationControl->addSlider(ImageProcessor::Const::sigx, parameters);

    }
    else if(operationName == "BilateralFilter")
    {
        operationControl->addSlider(ImageProcessor::Const::ksize, parameters);
        operationControl->addSlider(ImageProcessor::Const::sigc, parameters);
        operationControl->addSlider(ImageProcessor::Const::sigx, parameters);
    }
    else if(operationName == "Threshold")
    {
        operationControl->addSlider(ImageProcessor::Const::thresh, parameters);
    }
    else
    {
        throw std::runtime_error("unknown operation " + operationName);
    }

    return operationControl;
}

OperationControl::OperationControl(const std::string& name)
    : m_vbox(new QVBoxLayout()), m_name(name)
{
    setTitle(name.c_str());
}

void OperationControl::addSlider(const std::string& parameterName, ptree& parameters, int min, int max, unsigned int step)
{
    boost::optional<ptree&> parameter = parameters.get_child_optional(parameterName);
    if(!parameter)
    {
        parameters.push_back(ptree::value_type(parameterName, ptree("0")));
        parameter = parameters.back().second;
    }

    SliderControl* sliderControl = new SliderControl(parameterName.c_str(), *parameter, min, max, step);
    connect(sliderControl, SIGNAL(valueChanged()), this, SLOT(sliderValueChanged()));
    m_vbox->addWidget(sliderControl);
    m_sliderControls.push_back(sliderControl);
    setLayout(m_vbox);
}

void OperationControl::sliderValueChanged()
{
    emit valueChanged();
}
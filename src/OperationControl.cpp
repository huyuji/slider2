#include "OperationControl.h"
#include <exception>
#include "Const.h"
#include "util.h"

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
    : m_name(name)
{
    setContentsMargins(0, 0, 0, 0);

    m_buttonClose = new QPushButton("x");
    m_buttonClose->setFlat(false);
    m_buttonClose->setFixedWidth(ButtonWidth);
    m_buttonClose->setContentsMargins(0, 0, 0, 0);
    connect(m_buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    m_buttonDrag = new QPushButton("-");
    m_buttonDrag->setFlat(false);
    m_buttonDrag->setFixedWidth(ButtonWidth);
    m_buttonDrag->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    m_buttonDrag->setContentsMargins(0, 0, 0, 0);

    m_buttonLayout = new QVBoxLayout();
    m_buttonLayout->setSpacing(0);
    m_buttonLayout->addWidget(m_buttonClose);
    m_buttonLayout->addWidget(m_buttonDrag);

    m_box = new QGroupBox(name.c_str());
    m_boxLayout = new QVBoxLayout();
    m_boxLayout->setSpacing(0);
    m_box->setLayout(m_boxLayout);
    
    m_layout = new QHBoxLayout();
    m_layout->setSpacing(0);
    m_layout->addLayout(m_buttonLayout);
    m_layout->addWidget(m_box);

    setLayout(m_layout);
}

void OperationControl::addSlider(const std::string& parameterName, ptree& parameters, int min, int max, unsigned int step, unsigned int page)
{
    boost::optional<ptree&> parameter = parameters.get_child_optional(parameterName);
    if(!parameter)
    {
        parameters.push_back(ptree::value_type(parameterName, ptree("0")));
        parameter = parameters.back().second;
    }

    SliderControl* sliderControl = new SliderControl(parameterName.c_str(), *parameter, min, max, step, page);
    connect(sliderControl, SIGNAL(valueChanged()), this, SLOT(sliderValueChanged()));
    m_boxLayout->addWidget(sliderControl);
}

void OperationControl::sliderValueChanged()
{
    emit valueChanged();
}

void OperationControl::close()
{
    emit operationDeleted(this);
}

OperationControl::~OperationControl()
{
    ClearLayout(m_layout);
    delete m_layout;
}
#include "SliderControl.h"
#include "util.h"

using boost::property_tree::ptree;

SliderControl::SliderControl(
    const std::string& name, 
    ptree& parameter, 
    int min, int max, unsigned int step, unsigned int page,
    QWidget* parent)
    : QWidget(parent), m_name(name), m_parameter(parameter)
{
    int value = parameter.get_value<int>();

    QLabel *m_label = new QLabel(name.c_str());

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setTickPosition(QSlider::TicksAbove);
    m_slider->setRange(min, max);
    m_slider->setSingleStep(step);
    m_slider->setPageStep(page);
    m_slider->setValue(value);
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    m_spinBox = new QSpinBox();
    m_spinBox->setRange(min, max);
    m_spinBox->setSingleStep(step);
    m_spinBox->setValue(value);
    connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));

    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_label, 0, Qt::AlignLeft);
    m_layout->addWidget(m_slider, 1);
    m_layout->addWidget(m_spinBox, 0, Qt::AlignRight);
}

void SliderControl::sliderValueChanged(int value)
{
    m_spinBox->setValue(value);
    valueChanged(value);
}

void SliderControl::spinBoxValueChanged(int value)
{
    m_slider->setValue(value);
    valueChanged(value);
}

void SliderControl::valueChanged(int value)
{
    if(value != m_parameter.get_value<int>())
    {
        m_parameter.put_value(value);
        emit valueChanged();
    }
}
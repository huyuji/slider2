#include "SliderControl.h"

using boost::property_tree::ptree;

SliderControl::SliderControl(const std::string& name, ptree& parameter, int min, int max, unsigned int step)
    : m_name(name), m_parameter(parameter)
{
    int value = parameter.get_value<int>();
    QLabel *m_label = new QLabel(name.c_str());

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setFocusPolicy(Qt::StrongFocus);
    m_slider->setTickPosition(QSlider::TicksAbove);
    m_slider->setRange(min, max);
    m_slider->setSingleStep(step);
    m_slider->setValue(value);

    m_spinBox = new QSpinBox();
    m_spinBox->setRange(min, max);
    m_spinBox->setSingleStep(step);
    m_spinBox->setValue(value);

    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(m_label, 0, 0, Qt::AlignLeft);
    layout->addWidget(m_slider, 0, 1);
    layout->addWidget(m_spinBox, 0, 2, Qt::AlignRight);

    setLayout(layout);
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
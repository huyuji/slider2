#include "SliderControl.h"

SliderControl::SliderControl(const QString& name, int min, int max, unsigned int step)
    : m_value(0), m_name(name)
{
    QLabel *m_label = new QLabel(name);

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setFocusPolicy(Qt::StrongFocus);
    m_slider->setTickPosition(QSlider::TicksAbove);
    m_slider->setRange(min, max);
    m_slider->setSingleStep(step);
    m_slider->setValue(m_value);

    QSpinBox *m_spinBox = new QSpinBox;
    m_spinBox->setRange(min, max);
    m_spinBox->setSingleStep(step);
    m_spinBox->setValue(m_value);

    connect(m_slider, SIGNAL(valueChanged(int)), m_spinBox, SLOT(setValue(int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)), m_slider, SLOT(setValue(int)));

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(m_label, 0, 0, Qt::AlignLeft);
    layout->addWidget(m_slider, 0, 1);
    layout->addWidget(m_spinBox, 0, 2, Qt::AlignRight);

    setLayout(layout);
}
#include "SliderControl.h"

SliderControl::SliderControl(const QString& name)
{
    QLabel *m_label = new QLabel(name);

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setFocusPolicy(Qt::StrongFocus);
    m_slider->setTickPosition(QSlider::TicksBothSides);
    m_slider->setRange(0, 100);
    m_slider->setTickInterval(1);
    m_slider->setSingleStep(1);

    QSpinBox *m_spinBox = new QSpinBox;
    m_spinBox->setRange(0, 100);
    m_spinBox->setSingleStep(1);
    m_spinBox->setValue(0);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(m_label, 0, 0);
    layout->addWidget(m_slider, 0, 1);
    layout->addWidget(m_spinBox, 0, 2);
}
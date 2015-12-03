#pragma once
#ifndef idA361C69D_615C_4D1A_BC4ECF8BF5298678
#define idA361C69D_615C_4D1A_BC4ECF8BF5298678

#include <QtWidgets>

class SliderControl : public QWidget
{
    Q_OBJECT

public:
    SliderControl(const QString& name, int min, int max, unsigned int step);

private:
    QLabel* m_label;
    QSlider* m_slider;
    QSpinBox* m_spinBox;

    unsigned int m_value;
    const QString m_name;
};

#endif // header

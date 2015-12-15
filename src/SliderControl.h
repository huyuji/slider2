#pragma once
#ifndef idA361C69D_615C_4D1A_BC4ECF8BF5298678
#define idA361C69D_615C_4D1A_BC4ECF8BF5298678

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

class SliderControl : public QWidget
{
    Q_OBJECT

public:
    SliderControl(
        const std::string& name, 
        boost::property_tree::ptree& parameter, 
        int min, int max, unsigned int step, unsigned int page,
        QWidget* parent = nullptr);

signals:
    void valueChanged();

private slots:
    void sliderValueChanged(int);
    void spinBoxValueChanged(int);

private:
    void valueChanged(int);

    QHBoxLayout* m_layout;
        QLabel* m_label;
        QSlider* m_slider;
        QSpinBox* m_spinBox;

    boost::property_tree::ptree& m_parameter;
};

#endif // header

#pragma once
#ifndef id666D9969_BAF7_49BB_B1D206E3EE86EC63
#define id666D9969_BAF7_49BB_B1D206E3EE86EC63

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

class ConfigControl : public QWidget
{
    Q_OBJECT

public:
    ConfigControl(const std::string& name, boost::property_tree::ptree& parameter, int min, int max, unsigned int step, unsigned int page);
    ~ConfigControl();

signals:
    void valueChanged();

private slots:
    void sliderValueChanged(int);
    void spinBoxValueChanged(int);

private:
    void valueChanged(int);

    QLabel* m_label;
    QSlider* m_slider;
    QSpinBox* m_spinBox;
    QGridLayout* m_layout;

    const std::string m_name;
    boost::property_tree::ptree& m_parameter;
};

#endif // header

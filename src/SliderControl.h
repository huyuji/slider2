#include <QtWidgets>

class SliderControl : public QWidget
{
    Q_OBJECT

public:
    SliderControl(const QString& name);

private:
    QLabel* m_label;
    QSlider* m_slider;
    QSpinBox* m_spinBox;
};
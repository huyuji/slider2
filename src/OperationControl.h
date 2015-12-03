#include <QtWidgets>
#include "SliderControl.h"

class OperationControl : public QWidget
{
    Q_OBJECT

public:
    OperationControl();
};

class ContrastControl : public OperationControl
{
    Q_OBJECT

public:
    ContrastControl();

private:
    SliderControl* m_sliderControl;
};
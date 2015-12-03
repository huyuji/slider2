#include <QtWidgets>

class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    ControlPanel();

private:
    QPushButton* m_buttonLoad;
    QPushButton* m_buttonSave;

    QComboBox* m_operationList;
    QPushButton* m_buttonAddOperation;

    QGridLayout* m_gridLayout;
};
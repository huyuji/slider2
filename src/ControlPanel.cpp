#include "ControlPanel.h"
#include "OperationControl.h"

ControlPanel::ControlPanel()
{
    m_buttonLoad = new QPushButton("load");
    m_buttonSave = new QPushButton("save");

    m_operationList = new QComboBox();
    m_operationList->setInsertPolicy(QComboBox::InsertAlphabetically);
    m_operationList->addItem("Contrast");
    m_operationList->addItem("Convert");
    m_operationList->addItem("CorrectGamma");
    m_operationList->addItem("DFT");
    m_operationList->addItem("Erode");
    m_operationList->addItem("Sharpen");
    m_operationList->addItem("Threshold");

    m_buttonAddOperation = new QPushButton("add");

    OperationControl* operation = new ContrastControl();

    m_gridLayout = new QGridLayout();
    m_gridLayout->setAlignment(Qt::AlignTop);
    m_gridLayout->addWidget(m_buttonLoad, 0, 0);
    m_gridLayout->addWidget(m_buttonSave, 0, 1);
    m_gridLayout->addWidget(m_operationList, 0, 2);
    m_gridLayout->addWidget(m_buttonAddOperation, 0, 3);
    m_gridLayout->addWidget(operation, 1, 0);

    setLayout(m_gridLayout);
}
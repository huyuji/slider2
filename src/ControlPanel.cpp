#include "ControlPanel.h"
#include "OperationControl.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include "Const.h"

using boost::property_tree::ptree;

ControlPanel::ControlPanel(ptree& operations)
    : m_operations(operations)
{
    m_buttonLoad = new QPushButton("load");
    connect(m_buttonLoad, SIGNAL(clicked()), this, SLOT(loadConfigFile()));

    m_buttonSave = new QPushButton("save");
    connect(m_buttonSave, SIGNAL(clicked()), this, SLOT(saveOperations()));

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
    connect( m_buttonAddOperation, SIGNAL( clicked() ), this, SLOT(addOperation()) );

    m_gridLayout = new QGridLayout();
    m_gridLayout->setAlignment(Qt::AlignTop);
    m_gridLayout->addWidget(m_buttonLoad, 0, 0);
    m_gridLayout->addWidget(m_buttonSave, 0, 1);
    m_gridLayout->addWidget(m_operationList, 0, 2);
    m_gridLayout->addWidget(m_buttonAddOperation, 0, 3);

    setLayout(m_gridLayout);
}

void ControlPanel::addOperation()
{
    addOperation(m_operationList->currentText());
}

void ControlPanel::addOperation(const QString& operationName)
{
    OperationControl* operationControl = OperationControl::CreateOperationControl(operationName, m_operations);
    connect(operationControl, SIGNAL(valueChanged()), this, SLOT(operationValueChanged()));
    m_gridLayout->addWidget(operationControl, m_gridLayout->rowCount(), 0, 1, m_gridLayout->columnCount());
    setLayout(m_gridLayout);
}

void ControlPanel::saveOperations()
{
    std::ofstream json("operations.json");
    boost::property_tree::write_json(json, m_operations, true);
}

void ControlPanel::operationValueChanged()
{
    emit valueChanged();
}

void ControlPanel::loadConfigFile()
{
    QFileDialog dialog(this, tr("Open Config File"), QString(), tr("Json File (*.json)"));
    ptree configurations;
    while (dialog.exec() == QDialog::Accepted && !readConfigFile(dialog.selectedFiles().first(), configurations)) {}
    QString fileName = dialog.selectedFiles().first();

    if(configurations.size() == 1)
    {
        m_operations = configurations.begin()->second;
    }
    else
    {
        QGroupBox *groupBox = new QGroupBox(tr("Select a configuration"));
        QVBoxLayout *vbox = new QVBoxLayout();
        for(auto it = configurations.begin(); it != configurations.end(); ++it)
        {
            const ptree& config = it->second;
            std::string name = config.get<std::string>(ImageProcessor::Const::CONFIG_NAME);
            QRadioButton *radio = new QRadioButton(tr(name.c_str()));
            vbox->addWidget(radio);
        }
        vbox->addStretch(1);
        groupBox->setLayout(vbox);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
        connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

        QVBoxLayout *vboxOuter = new QVBoxLayout;
        vboxOuter->addWidget(groupBox);
        vboxOuter->addWidget(buttonBox);

        QDialog* popUp = new QDialog(this);
        popUp->setLayout(vboxOuter);
        popUp->exec();
    }
}

bool ControlPanel::readConfigFile(const QString &fileName, ptree& configurations)
{
    try
    {
        read_json(fileName.toStdString(), configurations);
        configurations = configurations.get_child(ImageProcessor::Const::CONFIG_CONFIGURATIONS);
        if(configurations.empty())
        {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("No configuration found in %1.").arg(QDir::toNativeSeparators(fileName)));
            return false;
        }
    }
    catch(std::exception& e)
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        return false;
    }

    return true;
}
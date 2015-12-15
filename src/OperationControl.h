#pragma once
#ifndef idB6191E8A_5691_4BBF_B66B745335976433
#define idB6191E8A_5691_4BBF_B66B745335976433

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

class OperationControl : public QWidget
{
    Q_OBJECT

public:
    static OperationControl* CreateOperationControl(
        const std::string& operationName, 
        boost::property_tree::ptree& parameters, 
        QWidget* parent = nullptr);

signals:
    void valueChanged();
    void closed(OperationControl*);
    void dragStarted(OperationControl*);

private slots:
    void close();
    void dragPressed();

private:
    static const int Min = 0;
    static const int Max = 100;
    static const unsigned int Step = 1;
    static const unsigned int Page = 5;
    static const unsigned int ButtonWidth = 20;

    QHBoxLayout* m_layout;
        QVBoxLayout* m_buttonLayout;
            QPushButton* m_buttonClose;
            QPushButton* m_buttonDrag;
        QGroupBox* m_box;
            QVBoxLayout* m_boxLayout;

    OperationControl(const std::string& name, QWidget* parent = nullptr);

    void addSlider(
        const std::string& parameterName, 
        boost::property_tree::ptree& parameters, 
        int min = Min, int max = Max, unsigned int step = Step, unsigned int page = Page);
};

#endif // header

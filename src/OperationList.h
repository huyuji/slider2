#pragma once
#ifndef id5A738EE8_E9AE_4E9F_968641C3158B4375
#define id5A738EE8_E9AE_4E9F_968641C3158B4375

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

class OperationControl;

class OperationList : public QWidget
{
    Q_OBJECT

public:
    OperationList(boost::property_tree::ptree& operations, QWidget* parent = nullptr);

    void newOperation(const std::string& operationName);

signals:
    void valueChanged();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

private slots:
    void startDrag(OperationControl*);
    void deleteOperation(OperationControl*);

private:
    static const char* MimeDataFormat;

    QVBoxLayout* m_layout;
    boost::property_tree::ptree& m_operations;

    void addOperation(const std::string& operationName, boost::property_tree::ptree& parameters);
};

#endif // header

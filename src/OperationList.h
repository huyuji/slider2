#pragma once
#ifndef id5A738EE8_E9AE_4E9F_968641C3158B4375
#define id5A738EE8_E9AE_4E9F_968641C3158B4375

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>
#include "OperationControl.h"

class OperationList : public QWidget
{
    Q_OBJECT

public:
    OperationList(QWidget* parent = nullptr);

    void add(OperationControl* item);
    void clear();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

signals:

private slots:
    void startDrag(OperationControl*);

private:
    static const char* MimeDataFormat;

    QVBoxLayout* m_layout;
};

#endif // header

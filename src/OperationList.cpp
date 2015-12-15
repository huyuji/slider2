#include "OperationList.h"
#include <qdebug.h>
#include "OperationControl.h"

using boost::property_tree::ptree;

const char* OperationList::MimeDataFormat = "data/operation";

OperationList::OperationList(boost::property_tree::ptree& operations, QWidget* parent)
    : QWidget(parent), m_operations(operations), m_movingOperationControl(nullptr)
{
    setAcceptDrops(true);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setAlignment(Qt::AlignTop);

    m_indicator = new QFrame(this);
    m_indicator->setFrameShape(QFrame::HLine);
    m_indicator->setFrameShadow(QFrame::Sunken);
    m_indicator->hide();

    if(!m_operations.empty())
    {
        for(auto it = m_operations.begin(); it != m_operations.end(); ++it)
        {
            addOperation(it->first, it->second);
        }

        emit valueChanged();
    }
}

void OperationList::addOperation(const std::string& operationName, boost::property_tree::ptree& parameters)
{
    OperationControl* operationControl = OperationControl::CreateOperationControl(operationName, parameters);
    connect(operationControl, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
    connect(operationControl, SIGNAL(closed(OperationControl*)), this, SLOT(deleteOperation(OperationControl*)));
    connect(operationControl, SIGNAL(dragStarted(OperationControl*)), this, SLOT(startDrag(OperationControl*)));
    m_layout->addWidget(operationControl, 0);
}

void OperationList::newOperation(const std::string& operationName)
{
    m_operations.push_back(ptree::value_type(operationName, ptree()));
    addOperation(operationName, m_operations.back().second);
    emit valueChanged();
}

void OperationList::deleteOperation(OperationControl* operationControl)
{
    const unsigned int index = m_layout->indexOf(operationControl);
    m_layout->removeWidget(operationControl);
    delete operationControl;
    auto it = m_operations.begin();
    std::advance(it, index);
    m_operations.erase(it);
    emit valueChanged();
}

void OperationList::startDrag(OperationControl* item)
{
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << (int)item;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeDataFormat, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(item->grab());

    m_indexBeforeMoving = m_layout->indexOf(item);
    m_layout->removeWidget(item);
    item->hide();
    m_movingOperationControl = item;
    Qt::DropAction dropAction = drag->exec();
}

void OperationList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(MimeDataFormat))
        event->accept();
    else
        event->ignore();
}

void OperationList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(MimeDataFormat)) {
        unsigned int y = 0;
        for(unsigned int i = 0; i < m_layout->count(); i++)
        {
            QWidget* item = m_layout->itemAt(i)->widget();
            const QRect& itemRect = item->geometry();
            y = itemRect.bottom();
            if(itemRect.contains(event->pos()))
            {
                y = itemRect.top();
                break;
            }
        }
        
        QRect rect(0, y, this->width(), 5);
        m_indicator->setGeometry(rect);
        m_indicator->show();

        event->accept();
    } else {
        event->ignore();
    }
}

void OperationList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(MimeDataFormat)) {
        unsigned int i = 0;
        for(; i < m_layout->count(); i++)
        {
            QWidget* item = m_layout->itemAt(i)->widget();
            const QRect& itemRect = item->geometry();
            if(itemRect.contains(event->pos()))
            {
                break;
            }
        }

        m_indicator->hide();
        m_layout->insertWidget(i, m_movingOperationControl);
        m_movingOperationControl->show();

        if(i != m_indexBeforeMoving)
        {
            auto it = m_operations.begin();
            std::advance(it, m_indexBeforeMoving);
            auto temp = *it;
            m_operations.erase(it);
            it = m_operations.begin();
            std::advance(it, i);
            m_operations.insert(it, temp);
            emit valueChanged();
        }

        event->accept();
    } else {
        event->ignore();
    }
}
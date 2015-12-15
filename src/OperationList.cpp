#include "OperationList.h"
#include "OperationControl.h"

using boost::property_tree::ptree;

const char* OperationList::MimeDataFormat = "data/operation";

OperationList::OperationList(boost::property_tree::ptree& operations, QWidget* parent)
    : QWidget(parent), m_operations(operations)
{
    setAcceptDrops(true);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setAlignment(Qt::AlignTop);

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
    const unsigned int index = m_layout->indexOf(item);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << index;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(MimeDataFormat, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(item->grab());

    QGraphicsOpacityEffect * effect = new QGraphicsOpacityEffect();
    effect->setOpacity(0.5);
    item->setGraphicsEffect(effect);

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
        QByteArray itemData = event->mimeData()->data(MimeDataFormat);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        unsigned int index = -1;
        dataStream >> index;

        foreach(QWidget* item, findChildren<QWidget*>())
        {
            if(event->answerRect().intersects(item->geometry()))
            {
                unsigned int targetIndex = m_layout->indexOf(item);
                if(targetIndex != index)
                {
                    auto item = m_layout->itemAt(index);
                    m_layout->removeItem(item);
                    m_layout->insertItem(targetIndex, item);
                }
            }
        }

        event->accept();
    } else {
        event->ignore();
    }
}

void OperationList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(MimeDataFormat)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("dropped"));
        
        //event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}
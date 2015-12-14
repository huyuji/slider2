#include "OperationList.h"
#include "util.h"

using boost::property_tree::ptree;

const char* OperationList::MimeDataFormat = "data/operation";

OperationList::OperationList(QWidget* parent)
    : QWidget(parent)
{
    setAcceptDrops(true);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setAlignment(Qt::AlignTop);
}

void OperationList::add(OperationControl* item)
{
    m_layout->addWidget(item, 0);
    connect(item, SIGNAL(dragStarted(OperationControl*)), this, SLOT(startDrag(OperationControl*)));
}

void OperationList::clear()
{
    qDeleteAll(findChildren<QWidget*>());
}

void OperationList::startDrag(OperationControl* item)
{
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    const unsigned int index = m_layout->indexOf(item);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << index;

    mimeData->setData(MimeDataFormat, itemData);
    drag->setMimeData(mimeData);
    drag->setPixmap(item->grab());

    m_layout->removeWidget(item);
    item->hide();
    delete item;

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
        event->setDropAction(Qt::MoveAction);
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
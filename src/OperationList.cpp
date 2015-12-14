#include "OperationList.h"
#include "util.h"

using boost::property_tree::ptree;

OperationList::~OperationList()
{
}

OperationList::OperationList()
{
    setAcceptDrops(true);
}

void OperationList::addItem(OperationControl* item)
{

}

void OperationList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-operation"))
        event->accept();
    else
        event->ignore();
}

void OperationList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-operation")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void OperationList::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-operation")) {
        QByteArray pieceData = event->mimeData()->data("image/x-operation");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint location;
        dataStream >> pixmap >> location;

        addPiece(pixmap, location);

        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}
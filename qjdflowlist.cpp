#include "qjdflowlist.h"
#include <QDebug>
#include <QComboBox>
#include <QLabel>
#include <QtGui>
#include <QMimeData>
#include <QDrag>

/// 创建的同时是否要创建jobxml
qjdFlowList::qjdFlowList(QWidget *parent) :
        QListWidget(parent)
{
    setFrameShape(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);

    setEnabled(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);

    connect(this, SIGNAL(currentRowChanged(int)),
            this,SLOT(itemChangeSlot(int)));

    index=0;

    QSize size;
    size.setHeight(20);
    size.setWidth(20);
    setIconSize(size);

    // 拖拽
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    /// 右键菜单
    menu = new QMenu(this); /// 设置tblMain的右键菜单
    actTurn = menu->addAction("Turn ON/OFF");
    actDel = menu->addAction("Delete");
//    actTer = menu->addAction("Terminate");

    connect(actTurn,SIGNAL(triggered()),this,SLOT(actTurnSlot()));
    connect(actDel,SIGNAL(triggered()),this,SLOT(actDelSlot()));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

void qjdFlowList::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"mouse press index"<<currentIndex().row();  //删除完添加row为-1 ???
    QListWidget::mousePressEvent(event);

    if(event->button()==Qt::MidButton  && currentIndex().row()>=0)
    {
        qDebug()<<"middle mouse press";
        // 设置相应的颜色边灰然后变蓝
        // 然后需要发送相应的信号到writejob当中去
        QListWidgetItem *item=currentItem();
        if(item->textColor()==Qt::blue)
        {
            item->setTextColor(Qt::lightGray);
            item->setIcon(QIcon(":/images/off.png"));
            int index=hashItem.value(item);
            emit sigTurnOff(index);
            return;
        }
        if(item->textColor()==Qt::lightGray)
        {
            item->setTextColor(Qt::blue);
            item->setIcon(QIcon(":/images/on.png"));
            int index=hashItem.value(item);
            emit sigTurnOn(index);
            return;
        }
    }

    if(event->button()==Qt::RightButton  && currentIndex().row()>=0)
    {
        qDebug()<<"right mouse press";
        QListWidgetItem *item=currentItem();
        // 需要提供右键菜单
        showContextMenu(event->pos());
    }
}

void qjdFlowList::mouseMoveEvent(QMouseEvent *event)
{
    QListWidget::mouseMoveEvent(event);
}

//void qjdFlowList::startDrag()
//{
//    QListWidgetItem *item = currentItem();
//    if (item)
//    {
//        QMimeData *mimeData = new QMimeData;
//        mimeData->setText(item->text());

//        QDrag *drag = new QDrag(this);
//        drag->setMimeData(mimeData);
//        drag->setDragCursor(QPixmap(":/images/add.png"),Qt::MoveAction);
////        drag->setPixmap(QPixmap(":/images/add.png"));
//        if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
//        {
//            delete item;   //!!! 警惕这句话
//        }
//    }
//}

void qjdFlowList::addFlow(const QString flowName, const QString flowPath)
{
    qDebug()<<"flowList addFlow~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    QListWidgetItem *item=new QListWidgetItem;
    item->setText(flowName);
    item->setTextColor(Qt::blue);
    item->setIcon(QIcon(":/images/on.png"));
    this->addItem(item);
    hashItem.insert(item, index);
    index++;

    setEnabled(true);
    //    qDebug()<<"current item ::" <<currentItem()<<currentRow();
    /// 应当把item一起传过去，方便邦定
    emit sigAddFlowWidget(flowName, flowPath);
}

void qjdFlowList::creatJobXML()
{
    QFile file;
    QString fileName;
    QDateTime a;
    fileName.append("jobxml/");  //路径,应当设置在当前流程目录下
    fileName.append("flow_");
    fileName.append(a.currentDateTime().toString("M-d_h_m_s"));
    //    fileName.append("1");
    fileName.append(".xml");
    qDebug()<<fileName;

    file.setFileName(fileName);
    if(!file.open(QFile::ReadWrite))
    {
        qDebug()<<"jobxml creat failed";
    }
    file.close();
    emit sigJobXMLfileName(fileName);  //在初始化中就发送信号了。。。
}

//void qjdFlowList::upFlow()
//{
//    qDebug()<<"upFlow";

//    if(currentRow()>0)
//    {
//        QListWidgetItem *current = this->currentItem();
//        int currIndex = this->row(current);

//        QListWidgetItem *prev = this->item(this->row(current) - 1);
//        int prevIndex = this->row(prev);
//        QListWidgetItem *temp = this->takeItem(prevIndex);
//        this->insertItem(prevIndex, current);
//        this->insertItem(currIndex, temp);

//        // xml
//        QListWidgetItem *upItem = currentItem();
//        emit sigUpFlow(hashItem.value(upItem));
//        qDebug()<<hashItem.value(upItem);
//    }
//    else
//    {
//        qDebug()<<"can`t up anymore";
//    }
//}


//void qjdFlowList::downFlow()
//{
//    qDebug()<<"downFlow";   //下去其实也有关系

//    if(currentRow()<(count()-1))
//    {
//        QListWidgetItem *current = this->currentItem();
//        int currIndex = this->row(current);

//        QListWidgetItem *next = this->item(this->row(current) + 1);
//        int nextIndex = this->row(next);
//        //        qDebug()<<currIndex<<nextIndex<<next->text();
//        QListWidgetItem *temp = this->takeItem(nextIndex);
//        this->insertItem(currIndex, temp);
//        this->insertItem(nextIndex, current);

//        QListWidgetItem *downItem = currentItem();
//        emit sigDownFlow(hashItem.value(downItem));

//    }
//    else
//    {
//        qDebug()<<"can`t down anymore";
//    }
//}

//void qjdFlowList::delFlow()
//{
//    qDebug()<<"deleteFlow";
//    QListWidgetItem *delItem = this->takeItem(currentRow());
//    emit sigDeleteFlow(hashItem.value(delItem));
//}


// 界面成功链接，xml 如何做呢？
void qjdFlowList::itemChangeSlot(int row)
{
    QListWidgetItem *item=currentItem();
    emit sigChangeStackWidgetIndex(hashItem.value(item));
}

/// ---------------------------------------------------------------------------///
/// --------------------------- 拖拽 -----------------------------------------///
// 目前自认为鼠标那部分完成了
void qjdFlowList::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"dragEnterEvent";
    QListWidget::dragEnterEvent(event);
}


void qjdFlowList::dropEvent(QDropEvent *event)
{
    qDebug()<<"dropEvent";
    QListWidget::dropEvent(event);

    QListWidgetItem *dragItem = currentItem();
    qDebug()<<hashItem.value(dragItem)<<currentRow();
    if(hashItem.value(dragItem)!=currentRow())
    {
        emit sigDrag(hashItem.value(dragItem), currentRow(),this->count());   // 除了传送stack的index，还要传送需要drag到的位置
    }
    // 无法获取共几行
//    qDebug()<<"~~~~~~~~"<<this->count();  //从1开始
}

/// 右键菜单
void qjdFlowList::showContextMenu(QPoint )
{
    // 显示在鼠标边上呢, 还是显示在item旁边
    qDebug()<<"showContextMenu";
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
}

void qjdFlowList::actTurnSlot()
{
    qDebug()<<"actTurnSlot";
    /// 照抄中键功能
    if(currentIndex().row()>=0)
    {
        QListWidgetItem *item=currentItem();
        if(item->textColor()==Qt::blue)
        {
            item->setTextColor(Qt::lightGray);
            item->setIcon(QIcon(":/images/off.png"));
            int index=hashItem.value(item);
            emit sigTurnOff(index);
            return;
        }
        if(item->textColor()==Qt::lightGray)
        {
            item->setTextColor(Qt::blue);
            item->setIcon(QIcon(":/images/on.png"));
            int index=hashItem.value(item);
            emit sigTurnOn(index);
            return;
        }
    }
}

void qjdFlowList::actDelSlot()
{
    qDebug()<<"actDelSlot";
    QListWidgetItem *delItem = this->takeItem(currentRow());
    emit sigDeleteFlow(hashItem.value(delItem));
}

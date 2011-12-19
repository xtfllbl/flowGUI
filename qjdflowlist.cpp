#include "qjdflowlist.h"
#include <QDebug>
#include <QComboBox>
#include <QLabel>
#include <QtGui>
#include <QMimeData>
#include <QDrag>

/// 创建的同时是否要创建jobxml
QJDFlowList::QJDFlowList(QWidget *parent) :
        QListWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);

//    setFrameShape(QFrame::WinPanel);
//    setFrameShadow(QFrame::Raised);

    setEnabled(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);

    connect(this, SIGNAL(currentRowChanged(int)),
            this,SLOT(itemChangeSlot(int)));

    index=0;
    hasMousePressed=false;

    QSize size;
    size.setHeight(20);
    size.setWidth(20);
    setIconSize(size);

    // 开启拖拽
    setDragEnabled(true);
    setAcceptDrops(true);
    // 关闭拖拽
//    setDragEnabled(false);
//    setAcceptDrops(false);

    /// 右键菜单
    menu = new QMenu(this); /// 设置tblMain的右键菜单
    actTurn = menu->addAction("Turn ON/OFF");
    actTurn->setIcon(QIcon(":/images/onoff.png"));
    actDel = menu->addAction("Delete");
    actDel->setIcon(QIcon(":/images/gtk-cancel.png"));

    connect(actTurn,SIGNAL(triggered()),this,SLOT(actTurnSlot()));
    connect(actDel,SIGNAL(triggered()),this,SLOT(actDelSlot()));
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    QPalette palette;
    QBrush brush(QColor(0, 0, 0, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
    this->setPalette(palette);
}

void QJDFlowList::mousePressEvent(QMouseEvent *event)
{
    /// 这个currentIndex.row很有问题,有一次点击的延迟?????
    hasMousePressed=true;
    QListWidget::mousePressEvent(event);
//    qDebug()<<"mouse press index"<<currentIndex().row();  //删除完添加row为-1 ???

    mousePressIndex=currentIndex().row();
    if(event->button()==Qt::MidButton  && currentIndex().row()>=0)
    {
//        qDebug()<<"middle mouse press";
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
//        qDebug()<<"right mouse press";
        // 需要提供右键菜单
        showContextMenu(event->pos());
    }
}

void QJDFlowList::mouseMoveEvent(QMouseEvent *event)
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

void QJDFlowList::addFlow(const QString flowName, const QString flowPath)
{
//    qDebug()<<"flowList addFlow~~~~~~~~~~~~~~~~~~~~~~~~~~~";
    qDebug()<<"QJDFlowList::addFlow::"<<hashItem;
    QListWidgetItem *item=new QListWidgetItem;
    item->setText(flowName);
    item->setTextColor(Qt::blue);
    item->setIcon(QIcon(":/images/on.png"));

    /// 添加到最底下,如何添加到当前index之后,并且保证xml????
    // 1. 添加到显示
    // 2. 添加到hash
    /// 插入到最后
    int emitValue=0;
    if(hasMousePressed==false)
    {
        this->addItem(item);
        hashItem.insert(item, index);
        emitValue=index;
    }
    /// 插入到鼠标点击之后的后面
    if(hasMousePressed==true)
    {
        this->insertItem(currentRow()+1,item);
//        hashItem.insert(item, currentRow()+1); // 在稍候的函数中,详细处理插入
         insertHashValue(currentRow()+1,item);  // 插入hashItem,插入之后,之后的都要改变
         emitValue=currentRow()+1;
         this->setCurrentRow(currentRow()+1);
    }
    index++;
    qDebug()<<"QJDFlowList::addFlow::"<<hashItem;

    setEnabled(true);
    //    qDebug()<<"current item ::" <<currentItem()<<currentRow();
    /// 应当把item一起传过去，方便邦定
    emit sigAddFlowWidget(flowName, flowPath, emitValue);  // 生成界面,无所谓顺序了

    emit sigAddHashListAndCreat(item);
}

void QJDFlowList::insertHashValue(int value, QListWidgetItem *saveItem)
{
    QListWidgetItem *hashModuleName;
    foreach(hashModuleName,hashItem.keys())
    {
        if(hashModuleName!=saveItem && hashItem.value(hashModuleName)>=value)
        {
            hashItem.insert(hashModuleName,hashItem.value(hashModuleName)+1);
        }
    }
    hashItem.insert(saveItem,value);
}

void QJDFlowList::creatJobXML()
{
    QFile file;
    QString fileName;
    QDateTime a;
    fileName.append("xml/job/");  //路径,应当设置在当前流程目录下
    fileName.append("flow_");
    fileName.append(a.currentDateTime().toString("M-d_h_m_s"));
    //    fileName.append("1");
    fileName.append(".xml");
//    qDebug()<<fileName;

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
void QJDFlowList::itemChangeSlot(int /*row*/)
{
    QListWidgetItem *item=currentItem();
    qDebug()<<currentItem();
    emit sigChangeStackWidgetIndex(item);
}

/// ---------------------------------------------------------------------------///
/// --------------------------- 拖拽 -----------------------------------------///
// 目前自认为鼠标那部分完成了
void QJDFlowList::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"QJDFlowList::dragEnterEvent";
    QListWidget::dragEnterEvent(event);
}


void QJDFlowList::dropEvent(QDropEvent *event)
{
    qDebug()<<"QJDFlowList::dropEvent";
    QListWidget::dropEvent(event);

    QListWidgetItem *dragItem = currentItem();
    qDebug()<<hashItem.value(dragItem)<<currentRow();

//    qDebug()<<"!!!!!!!!!drag!!!!!!!!!!";
    /// 以下传输的信号有问题, 准确的说是hash出了问题,拖拽后没有更新
    /// 那个hash应当以mouse拖拽完毕的index为准
//    qDebug()<<"beforemouse:"<<mousePressIndex<<"beforehash:"<<hashItem.value(dragItem)<<"now::"<<currentRow();
    emit sigDrag(hashItem.value(dragItem), currentRow(),this->count());

    resetHashValue(hashItem.value(dragItem), currentRow(), dragItem);
}

// 重置hashItem
void QJDFlowList::resetHashValue(int before, int after,QListWidgetItem *saveItem)
{
    // down
//    qDebug()<<"saveItem:"<<saveItem<<"after:"<<after;
    if(after-before>0)
    {
        QListWidgetItem *hashModuleName;
        foreach(hashModuleName,hashItem.keys())
        {
            qDebug()<<hashItem.value(hashModuleName)<<after;
            if(hashModuleName!=saveItem &&
                    hashItem.value(hashModuleName)<=after &&
                    hashItem.value(hashModuleName)>=before)
            {
//                qDebug()<<"in~~";
                hashItem.insert(hashModuleName,hashItem.value(hashModuleName)-1);
            }
        }
        hashItem.insert(saveItem,after);
    }

    // up
    if(after-before<0)
    {
        QListWidgetItem *hashModuleName;
        foreach(hashModuleName,hashItem.keys())
        {
            qDebug()<<hashItem.value(hashModuleName)<<after;
            if(hashModuleName!=saveItem &&
                    hashItem.value(hashModuleName)>=after &&
                    hashItem.value(hashModuleName)<=before)
            {
//                qDebug()<<"in~~";
                hashItem.insert(hashModuleName,hashItem.value(hashModuleName)+1);
            }
        }
        hashItem.insert(saveItem,after);
    }
//    qDebug()<<"hashItem::"<<hashItem;

}


/// 右键菜单
void QJDFlowList::showContextMenu(QPoint )
{
    // 显示在鼠标边上呢, 还是显示在item旁边
    qDebug()<<"showContextMenu";
    if(menu)
    {
        menu->exec(QCursor::pos());
    }
}

void QJDFlowList::actTurnSlot()
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

void QJDFlowList::actDelSlot()
{
    qDebug()<<"QJDFlowList::actDelSlot";
    QListWidgetItem *delItem = this->takeItem(currentRow());
    emit sigDeleteFlow(hashItem.value(delItem),hashItem.count()-1);
    resetDelHashValue(currentRow(),this->count()-1,delItem);
}

void QJDFlowList::resetDelHashValue(int current, int all,QListWidgetItem *delItem)
{
    index--;
    if(all-current==0)
    {
        hashItem.remove(delItem);
    }
    if(all-current>0)
    {
        hashItem.remove(delItem);
        QListWidgetItem *hashModuleName;
        foreach(hashModuleName,hashItem.keys())
        {
            if(hashModuleName!=delItem && hashItem.value(hashModuleName)>current)
            {
                hashItem.insert(hashModuleName,hashItem.value(hashModuleName)-1);
            }
        }
    }
}

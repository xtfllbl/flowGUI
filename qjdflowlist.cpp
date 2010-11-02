#include "qjdflowlist.h"
#include <QDebug>
#include <QComboBox>
#include <QLabel>
#include <QtGui>

/// 创建的同时是否要创建jobxml
qjdFlowList::qjdFlowList(QWidget *parent) :
        QListWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    //设置右键显示的内容,以及执行槽
    menu = new QMenu(this); /// 设置tblMain的右键菜单
    actDelete = menu->addAction("Delete this flow");

    // 除了删除界面上的，还要删除xml当中的内容
    connect(actDelete, SIGNAL(triggered()), this, SLOT(deleteFlow()));

//    creatJobXML();
}

void qjdFlowList::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        //        startPos = event->pos();

    }

    // 单击相应的item,切换到相应的widget
    emit sigPressIndex(currentItem()->text());
}

void qjdFlowList::mouseMoveEvent(QMouseEvent *event)
{
    QListWidget::mouseMoveEvent(event);
}

void qjdFlowList::deleteFlow()
{
}

void qjdFlowList::addFlow(QString flowName)
{
    this->addItem(flowName);
}

void qjdFlowList::creatJobXML()
{
    QFile file;
    QString fileName;
    QDateTime a;
    fileName.append("/home/xtf/Code/argXML2gui/job/");  //路径,应当设置在当前流程目录下
    fileName.append("flow_");
    fileName.append(a.currentDateTime().toString("M-d_h:m"));
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

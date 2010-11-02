#include "qjdmodulelist.h"
#include <QDebug>
#include <QtGui>
qjdModuleList::qjdModuleList(QWidget *parent) :
    QTableWidget(parent)
{
    /// table 更加灵活
//    setAcceptDrops(false);  //允许放下
//    setDragEnabled(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    indexXML=new readXMLIndex();

    analysisIndexXML();
    addModule();
//    this->setSelectionModel();
    // 应当解析出来的添加进来
//    setDragDropMode(QAbstractItemView::DragDrop);
    connect(this,SIGNAL(itemDoubleClicked(QTableWidgetItem*))
            ,this,SLOT(handleItemDoubleClickedEvent(QTableWidgetItem*)));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void qjdModuleList::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
}

void qjdModuleList::handleItemDoubleClickedEvent(QTableWidgetItem *item)
{   
    int position=item->row();
    QString flowName=this->item(position,0)->text();
    // 1.还要添加到邻居的flow列表中
    emit sigAddFlow(flowName);
    // 2.双击之后，添加模块(widget),首先要生成widget，包括读取模块xml
    QString flowPath=indexXML->getModulePath().at(position);

    qDebug()<<"double clicked::"<<flowName<<flowPath;
    emit sigAddFlowWidget(flowName,flowPath);

}


void qjdModuleList::analysisIndexXML()
{
    /// ----------------------------解析索引----------------------------
//    QString indexFilePath = QFileDialog::getOpenFileName(this, tr("Open File"),
//        "/home/xtf/xml", tr("Index xml files (*.xml);;HTML files (*.html);;"
//                    "SVG files (*.svg);;User Interface files (*.ui)"));
//    fileXMLIndex.setFileName(indexFilePath);

    fileXMLIndex.setFileName("/home/xtf/xml/index.xml");
    fileXMLIndex.open(QFile::ReadOnly);
    indexXML->read(&fileXMLIndex);
    /// ------------------------------------------------------------------
}

void qjdModuleList::addModule()
{
    QStringList a,b,c,d;
    a=indexXML->getModuleName();
    b=indexXML->getModuleVersion();
    c=indexXML->getModulePath();   //路径
    d=indexXML->getModuleDesc();    // 介绍,应当怎么显示呢？传输到其他控件麻烦，暂时搁置

    setRowCount(a.size());
    setColumnCount(2);

    QStringList labels;
    labels.append("name");
    labels.append("version");
    setHorizontalHeaderLabels(labels);
//    setVerticalHeaderLabels(labels);
    this->verticalHeader()->setVisible(false);

    for(int i=0;i<a.size();i++)
    {
        QTableWidgetItem *itemName = new QTableWidgetItem();
        QTableWidgetItem *itemVersion = new QTableWidgetItem();
        itemName->setText(a.at(i));
        itemVersion->setText(b.at(i));
        setItem(i, 0, itemName);
        setItem(i, 1, itemVersion);
    }
}

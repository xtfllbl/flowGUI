#include "qjdmodulelist.h"
#include <QDebug>
#include <QtGui>
#include "src/qjdargu.h"

QJDModuleList::QJDModuleList(QWidget *parent) :
    QTreeWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);
//    setFrameShape(QFrame::WinPanel);
//    setFrameShadow(QFrame::Raised);
    setHeaderHidden(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    indexXML=new readXMLIndex();

    analysisIndexXML();
    addModule();

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int))
            ,this,SLOT(handleItemDoubleClickedEvent(QTreeWidgetItem*,int)));
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPalette palette;
    QBrush brush(QColor(85, 0, 255, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush);
    this->setPalette(palette);

}

void QJDModuleList::analysisIndexXML()
{
    /// ----------------------------解析索引----------------------------
    fileXMLIndex.setFileName(":/xml/index.xml");
    fileXMLIndex.open(QFile::ReadOnly);
    indexXML->read(&fileXMLIndex);
}

void QJDModuleList::addModule()
{
    QStringList a,b,c,d,e;
    a=indexXML->getModuleName();
    b=indexXML->getModuleVersion();
    c=indexXML->getModulePath();   //路径
    d=indexXML->getModuleDesc();    // 介绍,应当怎么显示呢？传输到其他控件麻烦，暂时搁置
    e=indexXML->getModuleType();

    QTreeWidgetItem *io = new QTreeWidgetItem(this);
    io->setText(0,"Data IO");
    expandItem(io);
    QTreeWidgetItem *process = new QTreeWidgetItem(this);
    process->setText(0,"Data Process");
    expandItem(process);

    for(int i=0;i<a.size();i++)
    {
        if(e[i]=="Data IO")
        {
            int row=0;
            QTreeWidgetItem *itemIO = new QTreeWidgetItem(io);
            itemIO->setText(row,a[i]);
            row++;
        }
        if(e[i]=="Data Process")
        {
            int row=0;
            QTreeWidgetItem *itemProcess = new QTreeWidgetItem(process);
            itemProcess->setText(row,a[i]);
            row++;
        }
    }
}

void QJDModuleList::handleItemDoubleClickedEvent(QTreeWidgetItem *item, int col)
{
//    qDebug()<<item->text(col);  // 不想要的text也会有。。

    QJDArgu *argu=new QJDArgu();
    QStringList a,c;
    a=indexXML->getModuleName();
    c=indexXML->getModulePath();   //路径

    QString flowName=item->text(col);
    int flowIndex=-1;
    for(int i=0;i<a.size();i++)
    {
        if(a[i]==flowName)
        {
            flowIndex=i;
        }
    }

    if(flowName=="Data IO" || flowName=="Data Process")
    {
        return;
    }
    else
    {
        /// TODO功能: 添加流程到最后,或者添加到当前的index之后
        // 双击之后，添加模块(widget),首先要生成widget，包括读取模块xml
        QString flowPath;
        flowPath=argu->flowGuiPath+indexXML->getModulePath().at(flowIndex);
        emit sigAddFlowList(flowName,flowPath);
    }
}

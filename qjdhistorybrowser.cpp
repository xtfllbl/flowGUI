#include "qjdhistorybrowser.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QProgressBar>
#include "src/qjdargu.h"

/// 功能介绍
// 显示历史log的地方
QJDHistoryBrowser::QJDHistoryBrowser(QWidget *parent) :
        QTableWidget(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setFocusPolicy(Qt::NoFocus);
    this->setObjectName(QString::fromUtf8("processWidget"));
    this->setMinimumSize(QSize(0, 0));

    this->setFrameShape(QFrame::WinPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setAutoScroll(false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setAlternatingRowColors(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setTextElideMode(Qt::ElideRight);
    this->setShowGrid(true);
    this->setGridStyle(Qt::SolidLine);
    this->setSortingEnabled(false);
    this->setWordWrap(true);
    this->setCornerButtonEnabled(true);

    // --------------------------------------------------------------- //
    _hasLog=false;
    saveSelect=0;

    setRowCount(4);
    setColumnCount(4);

    setLogFile();
    connect(this,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(emitShowLog(QModelIndex)));

    QJDArgu *argu=new QJDArgu;
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(clearAndReset()));
    timer->start(argu->HistoryLogRefreshInterval);   //每隔10秒钟才刷新一次

}


/// 这个需要定时刷新的，使用model是否会好点
void QJDHistoryBrowser::setLogFile()
{
    /// 最好界面上哪里转一圈....
    /// Refreashing History Log
    hashFilePath.clear();  //刷新清除

    /// -------------------------head--------------------///
    QTableWidgetItem *headItem1=new QTableWidgetItem;
    QTableWidgetItem *headItem2=new QTableWidgetItem;
    QTableWidgetItem *headItem3=new QTableWidgetItem;
    QTableWidgetItem *headItem4=new QTableWidgetItem;
    headItem1->setText("Name");
    headItem2->setText("Stat");
    headItem3->setText("Progress");
    headItem4->setText("Last Modify");    // 为什么没有显示
    setHorizontalHeaderItem(0,headItem1);
    setHorizontalHeaderItem(1,headItem2);
    setHorizontalHeaderItem(2,headItem3);
    setHorizontalHeaderItem(3,headItem4);

    /// -----------------------内容------------------------///
    logDir.setPath("log");  //就是本地目录
    logDir.setFilter(QDir::Files | QDir::NoSymLinks);  //是文件，不需要链接
    fileInfoList=logDir.entryInfoList();
    QStringList fileNameList;
    QStringList filePathList;
    QStringList fileTimeList;

    for(int i=0;i<fileInfoList.size();i++)
    {
        if(fileInfoList.at(i).baseName().contains("job"))
        {
            // qDebug()<<"~~~~~~~~"<<fileInfoList.at(i).baseName();
            fileNameList.append(fileInfoList.at(i).baseName());
            filePathList.append(fileInfoList.at(i).filePath());
            fileTimeList.append(fileInfoList.at(i).lastModified().toString("yyyy-MM-dd,hh:mm:ss"));
        }
    }

    if(fileNameList.size()>0)
    {
        _hasLog=true;
    }
    else
    {
        _hasLog=false;
    }

    /// 还需要添加“进度”
    for(int i=0;i<fileNameList.size();i++)
    {
        hashFilePath.insert(i,filePathList.at(i));  // 保存文件路径
        QTableWidgetItem *nameItem=new QTableWidgetItem;
        QTableWidgetItem *statItem=new QTableWidgetItem;
        QTableWidgetItem *timeItem=new QTableWidgetItem;
        QProgressBar *progressBar=new QProgressBar;
        progressBar->setAlignment(Qt::AlignCenter);

        nameItem->setText(fileNameList.at(i));
        if(getLogStat(filePathList.at(i))=="success")
        {
            statItem->setTextColor(QColor(0, 0, 255, 255));
        }
        if(getLogStat(filePathList.at(i))=="error")
        {
            statItem->setTextColor(QColor(255, 0, 0, 255));
        }
        statItem->setText(getLogStat(filePathList.at(i)));

        timeItem->setText(fileTimeList.at(i));
        progressBar->setValue(getLogProgress(filePathList.at(i)));
        // getLogProgress
        this->setItem(i,0,nameItem);  // 名字
        this->setItem(i,1,statItem);  // 状态
        this->setCellWidget(i,2,progressBar);
        this->setItem(i,3,timeItem);  // 时间
        /// 还需要显示状态,但是需要读取文件
    }

    if(fileNameList.size()>=saveSelect)
    {
        setCurrentCell(saveSelect,0);
    }

    /// 需要显示的信息
    // job名称,状态,最后修改时间－－－－双击显示日志
}

/// 读取状态
QString QJDHistoryBrowser::getLogStat(QString fileName)
{
    QString SUCCESS="success";
    QString ERROR="error";
    QFile file;
    file.setFileName(fileName);
    file.open(QFile::ReadOnly);
    QTextStream stream;
    stream.setDevice(&file);
    QString all=stream.readAll();
    //    qDebug()<<all;
    file.close();
    if(all.contains("Success!"))  // 校验
        return SUCCESS;
    else
        return ERROR;
}

/// 读取进度
int QJDHistoryBrowser::getLogProgress(QString fileName)
{
//    qDebug()<<"getLogProgress";
    QFile file;
    file.setFileName(fileName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream;
    stream.setDevice(&file);

    int PROGRESS=-1;
    while(!file.atEnd())
    {
        QString line=file.readLine(100);
        line=line.trimmed();  //不换行

        QString per;
        QString right;
        if(line.contains("Complete:"))
        {
            right=line.mid(11);
            per=right.remove("%...");  /// per就是了
            PROGRESS=per.toInt();
            //            qDebug()<<line<<per;    // 这行是需要的，如何处理
        }
    }
    file.close();

    return PROGRESS;
}

// 清除原始数据，定时检查
void QJDHistoryBrowser::clearAndReset()
{
    this->clear();
    setLogFile();
}

bool QJDHistoryBrowser::hasLog()
{
    if(_hasLog==true)
    {
        return true;
    }
    else
        return false;

}

void QJDHistoryBrowser::mousePressEvent(QMouseEvent *event)
{
    QTableWidget::mousePressEvent(event);
    saveSelect=currentRow();  // 保存当前行
}

void QJDHistoryBrowser::emitShowLog(QModelIndex index)
{
    // 双击显示日志
    // 然后再发出信号（文件名称）
    emit showLog(hashFilePath.value(index.row()));  // fileName
}

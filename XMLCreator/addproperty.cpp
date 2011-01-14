#include "addproperty.h"
#include "ui_addproperty.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>

AddProperty::AddProperty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddProperty)
{
    ui->setupUi(this);
    ui->groupCheck->hide();
    ui->groupHide->hide();
    ui->groupLine->hide();
    ui->groupRadio->hide();
    ui->groupSpin->hide();
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

AddProperty::~AddProperty()
{
    delete ui;
}

void AddProperty::on_btnAdd_clicked()
{

    /// 传递过来文件名要比创建控件来的还要晚
    qDebug()<<"setPropertyName::"<<ui->lineProperty;   //模块名称在主界面，需要传递过来
//    jobFileName=fileName;
    isFileNameSet=true;
    initJob();  // 设置之后直接初始化

}

bool AddProperty::initJob()
{
    qDebug()<<"start init ";
    // 先要知道which file
    if(isFileNameSet==true)
    {
        fileJob.setFileName(jobFileName);
        if(!fileJob.open(QFile::ReadWrite))
            qDebug()<<"can`t open jobFileXML";
        // 先是只读，看看需不需要初始化
        QDomDocument doc;
        if (!doc.setContent(&fileJob))
        {
            qDebug()<<"setContent error or file is not init first, need to be init to a xml file!";

            writeHead(&fileJob);
        }
        else
        {
            qDebug()<<"file alread exist and init, it can be used to add module.";
        }
    }
    fileJob.close();

    if(isFileNameSet==false)
    {
        qDebug()<<"writeJob:: job file name is still not set";
        return false;
    }
    return true;
}

void AddProperty::writeHead(QIODevice *file)
{
    /// 头部采用简单方式写入，方便后续处理
    qDebug()<<"writeHead----------------------------------------";
    QTextStream out(file);
    out<<"<?xml version='1.0' encoding='UTF-8'?>\n"
            <<"<!DOCTYPE job>\n"
            <<"<job version=\"1.0\">\n"
            << "</job>\n";
}

bool AddProperty::readJob()
{
    qDebug()<<"read";
    if(fileRead.isOpen())
        qDebug()<<"fileRead is already open";
    fileRead.setFileName(jobFileName);

    if(!fileRead.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"can`t open jobFileXML in readonly";
        alreadyReadJob=false;
        return false;
    }
    /// 每次都设置，是不是导致错误的根源
    if (!doc.setContent(&fileRead, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<"has sth wrong,need init first";
        QWidget *widgetMSG=new QWidget();
        QMessageBox::information(widgetMSG, tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        fileRead.close();
        alreadyReadJob=false;

        return false;
    }
    else
    {
//        addModuleName(moduleName,moduleID);
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}

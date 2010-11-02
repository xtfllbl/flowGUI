#include "writejob.h"
#include <QMessageBox>

writeJob::writeJob(QObject *parent) :
    QObject(parent)
{
    /// 一个creatui就有一个writejob,首先需要判断是否初始化，没有再使用初始化
    jobFileName.clear();
    isFileNameSet=false;
}

void writeJob::setJobFileName(const QString fileName)
{
    /// 传递过来文件名要比创建控件来的还要晚
    qDebug()<<"writeJob::setJobFileName::"<<fileName;
    jobFileName=fileName;
    isFileNameSet=true;
    initJob();  // 设置之后直接初始化
}

bool writeJob::initJob()
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

void writeJob::writeHead(QIODevice *file)
{
    /// 头部采用简单方式写入，方便后续处理
    qDebug()<<"writeHead----------------------------------------";
    QTextStream out(file);
    out<<"<?xml version='1.0' encoding='UTF-8'?>\n"
            <<"<!DOCTYPE job>\n"
            <<"<job version=\"1.0\">\n"
            << "</job>\n";
}

bool writeJob::readJob()
{
    qDebug()<<"read";
    if(fileRead.isOpen())
        qDebug()<<"fileRead is already open";
    fileRead.setFileName(jobFileName);

    if(!fileRead.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"can`t open jobFileXML in readonly";
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
        return false;
    }
    else
    {
        // 正常分析, 可正常添加控件
        qDebug()<<"~~~~~";
        QDomElement root = doc.documentElement();
//        qDebug()<<root.tagName();
        QDomElement file =  doc.createElement("file");

        QDomElement el_address = doc.createElement("address");
        QDomAttr attr=doc.createAttribute("name");
        attr.setValue("value"); /// 最后有一斜杠,很多地方都有
        el_address.setAttributeNode(attr);
        QDomAttr attr2=doc.createAttribute("name2");
        attr2.setValue("value2");
        el_address.setAttributeNode(attr2);
        file.appendChild(el_address);

        root.appendChild(file);
        fileRead.close();
        return true;
        /// save只是将原来的东西又写了一边，而且没有覆盖。。。
    }
}

bool writeJob::firstWriteJob()
{
    qDebug()<<"write";
    /// 写可以等模块分析完毕之后
    fileWrite.setFileName(jobFileName);
    if(!fileWrite.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug()<<"can`t open jobFileXML in writeOnly";
        return false;
    }

    const int IndentSize = 4;

    QTextStream out(&fileWrite);
    doc.save(out, IndentSize);   //doc是可以延续的
    fileWrite.close();

    return true;
}

void writeJob::addNormalWidget(const QString property, const QString desc, const QString datatype,
                     const QString min, const QString max, const QString displaytype,
                     const QString displayvalue, const QStringList optiontext, const QStringList optionvalue)
{
    qDebug()<<"\nwriteJob::addNormalWidget::"<<  property<<   desc<<   datatype<<min<<   max<<   displaytype<<
      displayvalue<<   optiontext<<   optionvalue;

    // 1.打开文件，分析文件，准备添加
    if(!readJob())
        qDebug()<<"readJob failed";

    // 2. 添加各个属性
    //    if(!property.isEmpty())
    //    {}
    //    if(!desc.isEmpty())
    //    {}
    //    if(!datatype.isEmpty())
    //    {}
    //    if(!min.isEmpty())
    //    {}
    //    if(!max.isEmpty())
    //    {}
    //    if(!displaytype.isEmpty())
    //    {}
    //    if(!displayvalue.isEmpty())
    //    {}
    //    if(!optiontext.isEmpty())
    //    {}
    //    if(!optionvalue.isEmpty())
    //    {}

    if(!firstWriteJob())
        qDebug()<<"writejob failed";

}

void writeJob::addVisibleWidget(const QString property, const QString desc, const QString datatype,
                      const QString min, const QString max, const QString displaytype,
                      const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                      const QString visible)
{
    qDebug()<<"\nwriteJob::addVisibleWidget"<<  property<<   desc<<   datatype<<min<<   max<<   displaytype<<
      displayvalue<<   optiontext<<   optionvalue<< visible;

    /// 什么也不做，也会出错...
    //    if(!property.isEmpty())
    //    {}
    //    if(!desc.isEmpty())
    //    {}
    //    if(!datatype.isEmpty())
    //    {}
    //    if(!min.isEmpty())
    //    {}
    //    if(!max.isEmpty())
    //    {}
    //    if(!displaytype.isEmpty())
    //    {}
    //    if(!displayvalue.isEmpty())
    //    {}
    //    if(!optiontext.isEmpty())
    //    {}
    //    if(!optionvalue.isEmpty())
    //    {}
    //    if(!visible.isEmpty())
    //    {}
}


void writeJob::setModuleName(QString)
{}

void writeJob::setProperty(QString)
{}

void writeJob::setDesc(QString)
{}

void writeJob::setDatatype(QString)
{}

void writeJob::setMax(QString)
{}

void writeJob::setMin(QString)
{}

void writeJob::setDisplayType(QString)
{}

void writeJob::setDisplayValue(QString)
{}

void writeJob::setShowValue(QString)
{}

void writeJob::setOptionText(QString)
{}

void writeJob::setOptionValue(QString)
{}

bool writeJob::write(QIODevice */*device*/)
{
//    qDebug()<<"write";
//    const int IndentSize = 4;

//    QTextStream out(device);
//    doc.save(out, IndentSize);  //是不是由于这个是全局的，所以可以直接保存？不需要考虑？
    return true;
}

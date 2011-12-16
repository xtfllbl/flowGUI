#include "writejob.h"
#include <QMessageBox>

writeJob::writeJob(QObject *parent) :
        QObject(parent)
{
    /// 一个creatui就有一个writejob,首先需要判断是否初始化，没有再使用初始化
    jobFileName.clear();
    isFileNameSet=false;
    alreadyReadJob=false;
}

void writeJob::setModuleName(const QString name, const QString id)
{
//    qDebug()<<"module Name has been set";
    moduleName=name;
    moduleID=id;
}

void writeJob::setJobFileName(const QString fileName)
{
    /// 传递过来文件名要比创建控件来的还要晚
//    qDebug()<<"writeJob::setJobFileName::"<<fileName;
    jobFileName=fileName;
    isFileNameSet=true;
    initJob();  // 设置之后直接初始化
}

bool writeJob::initJob()
{
//    qDebug()<<"start init ";
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
//    qDebug()<<"writeHead----------------------------------------";
    QTextStream out(file);
    out<<"<?xml version='1.0' encoding='UTF-8'?>\n"
            <<"<!DOCTYPE job>\n"
            <<"<job version=\"1.0\">\n"
            << "</job>\n";
}

/// 在addXXXWidget 内被调用的，注意不要反调
// 初始化
bool writeJob::readJob()
{
//    qDebug()<<"read";
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
        addModuleName(moduleName,moduleID);  /// 变成每次添加node都会添加这个...
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}


/// 分析完毕用来写入的函数
bool writeJob::write()
{
    qDebug()<<"writeJob::write xml";
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

void writeJob::addModuleName(const QString name, const QString id)
{
//    qDebug()<<"addModuleName";
    QDomElement root = doc.documentElement();
    qDebug()<<root.text();
    moduleEle =  doc.createElement("Module");
    QDomAttr moduleAttr=doc.createAttribute("name");
    moduleAttr.setValue(name);
    QDomAttr moduleAttr2=doc.createAttribute("status");
    moduleAttr2.setValue("on");
    QDomAttr moduleAttr3=doc.createAttribute("id");
    moduleAttr3.setValue(id);
    // 如果在这里加上id，到时候找的到么？？？比需要传进来的
    moduleEle.setAttributeNode(moduleAttr);
    moduleEle.setAttributeNode(moduleAttr2);
    moduleEle.setAttributeNode(moduleAttr3);  //这里已经直接写入了，没有返回保存的余地了
    root.appendChild(moduleEle);  /// 应当在模块内部添加，而不是外部
}

void writeJob::addNormalWidget(const QString property, const QString desc, const QString datatype,
                               const QString min, const QString max, const QString displaytype,
                               const QString displayvalue, const QStringList optiontextList,
                               const QStringList optionvalueList, const QString id, const QString boxID,
                               const QList<QString> radioIDList)
{
//    qDebug()<<"writeJob::addNormalWidget::"<<  property<<   desc<<   datatype<<min<<   max<<   displaytype<<
//            displayvalue<<   optiontextList<<   optionvalueList<<id<<boxID<<radioIDList;

    // 1.打开文件，分析文件，准备添加
    if(alreadyReadJob==false)
    {
        if(!readJob())
        {
            qDebug()<<"readJob failed";
        }
    }
    // 2. 添加各个属性
    QDomElement propertyEle =  doc.createElement("property");
    QDomElement descEle =  doc.createElement("desc");
    QDomElement datatypeEle =  doc.createElement("datatype");
    QDomElement minEle =  doc.createElement("min");
    QDomElement maxEle =  doc.createElement("max");
    QDomElement displaytypeEle =  doc.createElement("displaytype");
    QDomElement idEle =  doc.createElement("id");
    //    QDomElement optiontextEle =  doc.createElement("option");  //多次创建，且在内层，不应当在此创建。
    if(!property.isEmpty())
    {
        QDomAttr idAttr=doc.createAttribute("id");   // 写上id和名字用来判断
        if(id!="")
        {
            idAttr.setValue(id);
        }
        if(id=="")
        {
            idAttr.setValue(boxID);  //还要在下面的radio中加入id阿(radioIDList)
        }
        QDomAttr propertyAttr=doc.createAttribute("name");
        propertyAttr.setValue(property);

        propertyEle.setAttributeNode(idAttr);
        propertyEle.setAttributeNode(propertyAttr);
        moduleEle.appendChild(propertyEle);    /// 应该将module添加进来
    }

    if(!desc.isEmpty())
    {
        QDomText descText=doc.createTextNode(desc);
        descEle.appendChild(descText);
        propertyEle.appendChild(descEle);
    }
    if(!datatype.isEmpty())
    {
        QDomText datatypeText=doc.createTextNode(datatype);
        datatypeEle.appendChild(datatypeText);
        propertyEle.appendChild(datatypeEle);
    }
    if(!min.isEmpty())
    {
        QDomText minText=doc.createTextNode(min);
        minEle.appendChild(minText);
        propertyEle.appendChild(minEle);
    }
    if(!max.isEmpty())
    {
        QDomText maxText=doc.createTextNode(max);
        maxEle.appendChild(maxText);
        propertyEle.appendChild(maxEle);
    }
    if(!displaytype.isEmpty())
    {
        QDomAttr displaytypeAttr=doc.createAttribute("name");
        displaytypeAttr.setValue(displaytype);

        displaytypeEle.setAttributeNode(displaytypeAttr);

        /// ----------------------radio && combo---------------------------- ///
        if(!optiontextList.isEmpty())
        {
            for(int i=0;i<optiontextList.size();i++)
            {
                QDomElement optiontextEle =  doc.createElement("option");   // 这个是要动态创建的，因为要多个。。。

                QDomAttr optiontextAttr=doc.createAttribute("value");
                optiontextAttr.setValue(optiontextList.at(i));
                optiontextEle.setAttributeNode(optiontextAttr);

                ///                <option value="CMP" id="Z2Q3bfYnd4KqMrx">checked</option>
                if(!radioIDList.isEmpty())   //写上radioButton的id
                {
                    QDomAttr optionRadioAttr=doc.createAttribute("id");
                    optionRadioAttr.setValue(radioIDList.at(i));
                    optiontextEle.setAttributeNode(optionRadioAttr);
                }
                if(!optionvalueList.isEmpty())
                {
                    QDomText optionvalueText=doc.createTextNode(optionvalueList.at(i));
                    optiontextEle.appendChild(optionvalueText);
                }

                displaytypeEle.appendChild(optiontextEle);
            }
        }
        /// -------------------------------------------------- ///

        // 这里会有多余数据，禁用他
        if(!displayvalue.isEmpty() && optiontextList.isEmpty())
        {
            QDomText displayvalueText=doc.createTextNode(displayvalue);
            displaytypeEle.appendChild(displayvalueText);
        }
        propertyEle.appendChild(displaytypeEle);
    }

    if(!write())
        qDebug()<<"writejob failed";

}

///  id没有加入，无法修改
void writeJob::addHideWidget(const QString property, const QString desc, const QString datatype,
                                const QString min, const QString max, const QString displaytype,
                                const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                                const QString visible, const QString boxID, const QString lineID)
{
    qDebug()<<"writeJob::addHideWidget"<<  property<<   desc<<   datatype<<min<<   max<<   displaytype<<
            displayvalue<<   optiontext<<   optionvalue<< visible<<boxID<<lineID;

    if(alreadyReadJob==false)
    {
        if(!readJob())
        {
            qDebug()<<"readJob failed";
        }
    }
    QDomElement root = doc.documentElement();

    QDomElement propertyEle =  doc.createElement("property");
    QDomElement descEle =  doc.createElement("desc");
    QDomElement datatypeEle =  doc.createElement("datatype");
    QDomElement minEle =  doc.createElement("min");
    QDomElement maxEle =  doc.createElement("max");
    QDomElement displaytypeEle =  doc.createElement("displaytype");
    QDomElement visibleEle =  doc.createElement("hide");  //much better than visible
    QDomElement bxoIDEle =  doc.createElement("id");
    if(!property.isEmpty())
    {
        QDomAttr idAttr=doc.createAttribute("id");   // 写上id和名字用来判断
        if(boxID!="")
        {
            idAttr.setValue(boxID);
        }
        if(boxID=="")
        {
            idAttr.setValue(boxID);  //还要在下面的radio中加入id阿(radioIDList)
        }
        QDomAttr propertyAttr=doc.createAttribute("name");
        propertyAttr.setValue(property);

        propertyEle.setAttributeNode(idAttr);
        propertyEle.setAttributeNode(propertyAttr);
        moduleEle.appendChild(propertyEle);    /// 应该将module添加进来
    }
    if(!desc.isEmpty())
    {
        QDomText descText=doc.createTextNode(desc);
        descEle.appendChild(descText);
        propertyEle.appendChild(descEle);
    }
    if(!datatype.isEmpty())
    {
        QDomText datatypeText=doc.createTextNode(datatype);
        datatypeEle.appendChild(datatypeText);
        propertyEle.appendChild(datatypeEle);
    }
    if(!min.isEmpty())
    {
        QDomText minText=doc.createTextNode(min);
        minEle.appendChild(minText);
        propertyEle.appendChild(minEle);
    }
    if(!max.isEmpty())
    {
        QDomText maxText=doc.createTextNode(max);
        maxEle.appendChild(maxText);
        propertyEle.appendChild(maxEle);
    }
    /// -------------------------------------------------- ///
    if(!displaytype.isEmpty())
    {
        QDomAttr displaytypeAttr=doc.createAttribute("name");
        displaytypeAttr.setValue(displaytype);

        displaytypeEle.setAttributeNode(displaytypeAttr);

        if(!lineID.isEmpty())
        {
            QDomAttr displaytypeIDAttr=doc.createAttribute("id");
            displaytypeIDAttr.setValue(lineID);
            displaytypeEle.setAttributeNode(displaytypeIDAttr);
        }
        if(!optiontext.isEmpty())
        {
            for(int i=0;i<optiontext.size();i++)
            {
                QDomElement optiontextEle =  doc.createElement("option");   // 这个是要动态创建的，因为要多个。。。

                QDomAttr optiontextAttr=doc.createAttribute("value");
                optiontextAttr.setValue(optiontext.at(i));
                optiontextEle.setAttributeNode(optiontextAttr);

                if(!optionvalue.isEmpty())
                {
                    QDomText optionvalueText=doc.createTextNode(optionvalue.at(i));
                    optiontextEle.appendChild(optionvalueText);
                }

                displaytypeEle.appendChild(optiontextEle);
            }
        }
        // 这里会有多余数据
        if(!displayvalue.isEmpty() && optiontext.isEmpty())
        {
            QDomText displayvalueText=doc.createTextNode(displayvalue);
            displaytypeEle.appendChild(displayvalueText);
        }
        propertyEle.appendChild(displaytypeEle);
    }
    /// -------------------------------------------------- ///

    if(!visible.isEmpty())
    {
        QDomText visibleText=doc.createTextNode(visible);
        visibleEle.appendChild(visibleText);
        propertyEle.appendChild(visibleEle);
    }


    if(!write())
        qDebug()<<"writejob failed";
}


bool writeJob::lineEditChanged(QString text, QString id)
{
    qDebug()<<"lineEditChanged"<<text<<id;

    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleElement(moduleNameEle,text,id);
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}

bool writeJob::lineEditChanged(QString text, QString boxID, QString id)
{
    qDebug()<<"lineEditChanged"<<text<<boxID<<id;

    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleHideElement(moduleNameEle,text,boxID,id);
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}

bool writeJob::checkBoxChanged(QString text, QString id)
{
    qDebug()<<"checkBoxChanged"<<text<<id;

    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleElement(moduleNameEle,text,id);  // 要之后的一个步骤不同。。。
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}

bool writeJob::spinBoxChanged(QString text, QString id)
{
    qDebug()<<"spinBoxChanged"<<text<<id;
    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleElement(moduleNameEle,text,id);
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}

bool writeJob::comboBoxChanged(QString text, QString id)
{
    qDebug()<<"comboBoxChanged"<<text<<id;
    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleElement(moduleNameEle,text,id);
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}

bool writeJob::groupBoxChanged(bool checked,QString id)
{
    qDebug()<<"groupBoxChanged::checked"<<checked<<id;
    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleHideElement(moduleNameEle,checked,id);  // 处理需要更换方法
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}


bool writeJob::radioButtonChanged(QString text, QString id, QString otherID)
{
    qDebug()<<"radioButtonChanged"<<text<<id<<otherID;   // 这个id,不是property的id
    QFile readFile;
    readFile.setFileName(jobFileName);
    if(!readFile.open(QFile::ReadOnly))
        qDebug()<<"can`t open jobFileXML";

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&readFile, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an jobxml  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")
    {
        qDebug()<< tr("The file is not an jobxml version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name");

    while (!moduleNameEle.isNull())
    {
        QString name = moduleNameEle.attribute("name");
        parseModuleElement(moduleNameEle,text,id,otherID);
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }
    readFile.close();

    QFile writeFile;
    writeFile.setFileName(jobFileName);
    if(!writeFile.open(QFile::WriteOnly))
        qDebug()<<"can`t open jobFileXML";
    const int IndentSize = 4;
    QTextStream out(&writeFile);
    doc.save(out, IndentSize);
    writeFile.close();
    return true;
}

void writeJob::parseModuleElement(QDomElement const& moduleNameEle,QString text,QString id)
{
    QDomElement child=moduleNameEle.firstChildElement("property");
    while (!child.isNull())
    {
        QString idname = child.attribute("id");
        if(idname==id)
        {
            /// qDebug()<<"pro name::"<<child.attribute("name");  //需要property下面的displaytype才能决定
            parsePropertyElement(child,text);  // checkbox, 有可能radio需要单独处理
            break; // 处理完成则无须再往下走
        }
        child = child.nextSiblingElement("property");
    }
}

void writeJob::parseModuleHideElement(QDomElement const& moduleNameEle,bool checked,QString id)
{
    QDomElement child=moduleNameEle.firstChildElement("property");
    while (!child.isNull())
    {
        QString idname = child.attribute("id");
        if(idname==id)
        {
            /// qDebug()<<"pro name::"<<child.attribute("name");  //需要property下面的displaytype才能决定
            parsePropertyHideElement(child,checked);  // checkbox, 有可能radio需要单独处理
            break; // 处理完成则无须再往下走
        }
        child = child.nextSiblingElement("property");
    }
}

void writeJob::parseModuleHideElement(const QDomElement &moduleNameEle, QString text, QString boxID, QString id)
{
    qDebug()<<"parseModuleHideElement"<<text<<boxID<<id;
    QDomElement child=moduleNameEle.firstChildElement("property");
    while (!child.isNull())
    {
        QString idname = child.attribute("id");
        if(idname==boxID)
        {
            /// qDebug()<<"pro name::"<<child.attribute("name");  //需要property下面的displaytype才能决定
            parsePropertyHideElement(child,text,id);  // checkbox, 有可能radio需要单独处理
            break; // 处理完成则无须再往下走
        }
        child = child.nextSiblingElement("property");
    }
}

void writeJob::parseModuleElement(const QDomElement &moduleNameEle, QString text, QString id, QString boxID)
{
    QDomElement child=moduleNameEle.firstChildElement("property");
    while (!child.isNull())
    {
        QString idname = child.attribute("id");
        if(idname==boxID)
        {
            /// qDebug()<<"pro name::"<<child.attribute("name");  //需要property下面的displaytype才能决定
            parsePropertyElement(child,text,id);  // checkbox, 有可能radio需要单独处理
            break; // 处理完成则无须再往下走
        }
        child = child.nextSiblingElement("property");
    }
}

/// 目前太单纯，需要根据不同的控件来处理
void writeJob::parsePropertyElement(QDomElement property,QString text)
{
    QDomElement oldDisplayType=property.firstChildElement("displaytype");
    /// 需要根据这个类型来判断
    qDebug()<<"changed widget type::"<<oldDisplayType.attribute("name");
    QString type=oldDisplayType.attribute("name");

    // ------------------------------- 一般控件--------------------------//
    if(type=="lineedit" || type=="checkbox" || type=="spinbox" || type=="fileread" || type=="filesave")
    {
        QDomElement newDisplayType = doc.createElement("displaytype");
        newDisplayType.setAttribute("name",oldDisplayType.attribute("name"));  //保留控件类型
        QDomText newDisplayText = doc.createTextNode(text);
        newDisplayType.appendChild(newDisplayText);
        property.replaceChild(newDisplayType,oldDisplayType);
    }

    // ------------------------------- comboBox--------------------------//
    if(type=="combobox")
    {
        qDebug()<<"handle combobox";
        QDomElement optionEle=oldDisplayType.firstChildElement("option");
        QList<QDomElement> newEle;
        QList<QDomElement> oldEle;
        while(!optionEle.isNull())
        {
            if(optionEle.attribute("value")==text)
            {
                QDomElement newOptionEle = doc.createElement("option");
                newOptionEle.setAttribute("value",optionEle.attribute("value"));
                QDomText newOptionText = doc.createTextNode("checked");
                newOptionEle.appendChild(newOptionText);
                newEle.append(newOptionEle);
                oldEle.append(optionEle);
            }
            else
            {
                QDomElement newOptionEle = doc.createElement("option");
                newOptionEle.setAttribute("value",optionEle.attribute("value"));
                QDomText newOptionText = doc.createTextNode("unchecked");
                newOptionEle.appendChild(newOptionText);
                newEle.append(newOptionEle);
                oldEle.append(optionEle);
            }
            optionEle=optionEle.nextSiblingElement("option");
        }
        /// 需要替换全部保存，到最后一起替换，证实此方法可行，中途替换将导致循环的不进行
        for(int i=0;i<newEle.size();i++)
        {
            oldDisplayType.replaceChild(newEle[i],oldEle[i]);
        }
    }
    if(type=="filecombobox")
    {
        qDebug()<<"handle filecombobox";
        QDomElement optionEle=oldDisplayType.firstChildElement("option");

        QDomElement newOptionEle = doc.createElement("option");
        newOptionEle.setAttribute("value",text);  //保留控件类型
        QDomText newOptionText = doc.createTextNode(optionEle.text());
        newOptionEle.appendChild(newOptionText);
        oldDisplayType.replaceChild(newOptionEle,optionEle);
    }
    if(type=="radiobutton")
    {
        qDebug()<<"handle radiobutton:: this can`t be shown,beacuse of the anthoer parseProperty";
        /// 这个比较难搞，目前还没发出信号
    }
}

void writeJob::parsePropertyHideElement(QDomElement property,bool checked)
{
    QDomElement oldHideEle=property.firstChildElement("hide");

    QDomElement newHideEle = doc.createElement("hide");
//    newHideEle.setAttribute("name",oldDisplayType.attribute("name"));  //保留控件类型
    if(checked==true)
    {
        QDomText newHideTxt = doc.createTextNode("NO");
        newHideEle.appendChild(newHideTxt);
        property.replaceChild(newHideEle,oldHideEle);
    }
    if(checked==false)
    {
        QDomText newHideTxt = doc.createTextNode("YES");
        newHideEle.appendChild(newHideTxt);
        property.replaceChild(newHideEle,oldHideEle);
    }
}

void writeJob::parsePropertyHideElement(QDomElement property, QString text, QString /*id*/)
{
    /// 先要从写上入手，写入id
    QDomElement oldDisplayType=property.firstChildElement("displaytype");
    /// 需要根据这个类型来判断
    qDebug()<<"changed widget type::"<<oldDisplayType.attribute("name");
    QString type=oldDisplayType.attribute("name");
    if(type=="lineedit")
    {
        QDomElement newDisplayType = doc.createElement("displaytype");
        newDisplayType.setAttribute("name",oldDisplayType.attribute("name"));  //保留控件类型
        newDisplayType.setAttribute("id",oldDisplayType.attribute("id"));  //保留控件类型
        QDomText newDisplayText = doc.createTextNode(text);
        newDisplayType.appendChild(newDisplayText);
        property.replaceChild(newDisplayType,oldDisplayType);
    }
}

void writeJob::parsePropertyElement(QDomElement property, QString text, QString id)
{
    /// 先要从写上入手，写入id
    QDomElement oldDisplayType=property.firstChildElement("displaytype");
    /// 需要根据这个类型来判断
    qDebug()<<"changed widget type::"<<oldDisplayType.attribute("name");
    QString type=oldDisplayType.attribute("name");
    if(type=="radiobutton")
    {
        qDebug()<<"handle radiobutton:: "<<text<<id;
        QDomElement optionEle=oldDisplayType.firstChildElement("option");
        QList<QDomElement> newEle;
        QList<QDomElement> oldEle;
        while(!optionEle.isNull())
        {
            /// id也要重新写回
            qDebug()<<optionEle.attribute("value")<<optionEle.attribute("id")<<text<<id;
            if(optionEle.attribute("id")==id)   // checked
            {
//                qDebug()<<"checked";
                QDomElement newOptionEle = doc.createElement("option");
                newOptionEle.setAttribute("value",optionEle.attribute("value"));
                newOptionEle.setAttribute("id",optionEle.attribute("id"));

                QDomText newOptionText = doc.createTextNode("checked");
                newOptionEle.appendChild(newOptionText);
                newEle.append(newOptionEle);
                oldEle.append(optionEle);
            }
            else
            {
//                qDebug()<<"unchecked";
                QDomElement newOptionEle = doc.createElement("option");
                newOptionEle.setAttribute("value",optionEle.attribute("value"));
                newOptionEle.setAttribute("id",optionEle.attribute("id"));

                QDomText newOptionText = doc.createTextNode("unchecked");
                newOptionEle.appendChild(newOptionText);
                newEle.append(newOptionEle);
                oldEle.append(optionEle);
            }
            optionEle=optionEle.nextSiblingElement("option");
        }
        for(int i=0;i<newEle.size();i++)
        {
            oldDisplayType.replaceChild(newEle[i],oldEle[i]);
        }
    }
}
bool writeJob::deleteThisJob()
{
    qDebug()<<"job will be delete from the xml";
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
        delModuleName(moduleName);
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}

void writeJob::delModuleName(const QString name)
{
    qDebug()<<"delModuleName";
    QDomElement root = doc.documentElement();
    QDomElement modEle = root.firstChildElement("Module");

    while (!modEle.isNull())
    {
        if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
        {
            root.removeChild(modEle);
            break;
        }
        modEle = modEle.nextSiblingElement("Module");
    }
    if(!write())
        qDebug()<<"delete job failed";
}

//bool writeJob::upThisJob()
//{
//    qDebug()<<"job will be move up from the xml";
//    if(fileRead.isOpen())
//        qDebug()<<"fileRead is already open";
//    fileRead.setFileName(jobFileName);

//    if(!fileRead.open(QFile::ReadOnly | QFile::Text))
//    {
//        qDebug()<<"can`t open jobFileXML in readonly";
//        alreadyReadJob=false;
//        return false;
//    }
//    /// 每次都设置，是不是导致错误的根源
//    if (!doc.setContent(&fileRead, true, &errorStr, &errorLine,&errorColumn))
//    {
//        // 错误处理，先不必要
//        qDebug()<<"has sth wrong,need init first";
//        QWidget *widgetMSG=new QWidget();
//        QMessageBox::information(widgetMSG, tr("DOM Bookmarks"),
//                                 tr("Parse error at line %1, column %2:\n%3")
//                                 .arg(errorLine)
//                                 .arg(errorColumn)
//                                 .arg(errorStr));
//        fileRead.close();
//        alreadyReadJob=false;

//        return false;
//    }
//    else
//    {
//        upModuleName(moduleName);
//        alreadyReadJob=true;

//        fileRead.close();
//        return true;
//    }
//}

//void writeJob::upModuleName(const QString name)
//{
//    qDebug()<<"upModuleName";
//    QDomElement root = doc.documentElement();
//    QDomElement modEle = root.firstChildElement("Module");

//    while (!modEle.isNull())
//    {
//        if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
//        {
//            QDomElement prevModEle=modEle.previousSiblingElement("Module");
//            root.insertBefore(modEle,prevModEle);
//            break;
//        }
//        modEle = modEle.nextSiblingElement("Module");
//    }
//    if(!write())
//        qDebug()<<"up job failed";
//}

bool writeJob::resetThisJob(int upValue)
{
    qDebug()<<"writeJob::job will be reset(move up) from the xml";
    if(fileRead.isOpen())
        qDebug()<<"writeJob::fileRead is already open";
    fileRead.setFileName(jobFileName);
//    qDebug()<<"writeJob::jobFileName::"<<jobFileName;

    if(!fileRead.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"writeJob::can`t open jobFileXML in readonly";
        alreadyReadJob=false;
        return false;
    }
    /// 每次都设置，是不是导致错误的根源
    if (!doc.setContent(&fileRead, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<"writeJob::has sth wrong,need init first";
        QWidget *widgetMSG=new QWidget();
        QMessageBox::information(widgetMSG, tr("writeJob::DOM Bookmarks"),
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
        for(int i=0;i<upValue;i++)
        {
            resetModuleName(moduleName);
        }
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}

void writeJob::resetModuleName(const QString name)
{
        qDebug()<<"writeJob::resetModuleName";
        QDomElement root = doc.documentElement();
        QDomElement modEle = root.firstChildElement("Module");

        while (!modEle.isNull())
        {
            if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
            {
                /// 采用的替换法,多替换几次???难道仅仅是这个函数多执行几次???
                QDomElement prevModEle=modEle.previousSiblingElement("Module");
                root.insertBefore(modEle,prevModEle);
                break;
            }
            modEle = modEle.nextSiblingElement("Module");
        }
        if(!write())
            qDebug()<<"writeJob::reset job failed";
}
//bool writeJob::downThisJob()
//{
//    qDebug()<<"job will be move down from the xml";
//    if(fileRead.isOpen())
//        qDebug()<<"fileRead is already open";
//    fileRead.setFileName(jobFileName);

//    if(!fileRead.open(QFile::ReadOnly | QFile::Text))
//    {
//        qDebug()<<"can`t open jobFileXML in readonly";
//        alreadyReadJob=false;
//        return false;
//    }
//    /// 每次都设置，是不是导致错误的根源
//    if (!doc.setContent(&fileRead, true, &errorStr, &errorLine,&errorColumn))
//    {
//        // 错误处理，先不必要
//        qDebug()<<"has sth wrong,need init first";
//        QWidget *widgetMSG=new QWidget();
//        QMessageBox::information(widgetMSG, tr("DOM Bookmarks"),
//                                 tr("Parse error at line %1, column %2:\n%3")
//                                 .arg(errorLine)
//                                 .arg(errorColumn)
//                                 .arg(errorStr));
//        fileRead.close();
//        alreadyReadJob=false;

//        return false;
//    }
//    else
//    {
//        downModuleName(moduleName);
//        alreadyReadJob=true;

//        fileRead.close();
//        return true;
//    }
//}

//void writeJob::downModuleName(const QString name)
//{
//    qDebug()<<"downModuleName";
//    QDomElement root = doc.documentElement();
//    QDomElement modEle = root.firstChildElement("Module");

//    while (!modEle.isNull())
//    {
//        if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
//        {
//            QDomElement nextModEle=modEle.nextSiblingElement("Module");
//            root.insertAfter(modEle,nextModEle);
//            break;
//        }
//        modEle = modEle.nextSiblingElement("Module");
//    }
//    if(!write())
//        qDebug()<<"down job failed";
//}

bool writeJob::turnOnThisJob()
{
    // 需要将关键信息写入xml
    qDebug()<<"job will be  turn on  from the xml";
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
        turnOnModuleName(moduleName);
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}

/// 致命的问题：在多重名字相同的情况下会删除现得到的那个，需要使用id问题解决
void writeJob::turnOnModuleName(const QString name)
{
    qDebug()<<"turnOnModuleName";
    QDomElement root = doc.documentElement();
    QDomElement modEle = root.firstChildElement("Module");

    while (!modEle.isNull())
    {
        if(modEle.attribute("name")==name && modEle.attribute("status")=="off" && modEle.attribute("id")==moduleID)
        {
            modEle.setAttribute("status","on");
            break;
        }
        modEle = modEle.nextSiblingElement("Module");
    }
    if(!write())
        qDebug()<<"delete job failed";
}

bool writeJob::turnOffThisJob()
{
    qDebug()<<"job will be turn off from the xml";
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
        turnOffModuleName(moduleName);
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}



/// ----------------------------------  拖拽部分-----------------------------------------------///
void writeJob::turnOffModuleName(const QString name)
{
    qDebug()<<"turnOffModuleName";
    QDomElement root = doc.documentElement();
    QDomElement modEle = root.firstChildElement("Module");

    while (!modEle.isNull())
    {
        if(modEle.attribute("name")==name && modEle.attribute("status")=="on" && modEle.attribute("id")==moduleID)
        {
            modEle.setAttribute("status","off");
            break;
        }
        modEle = modEle.nextSiblingElement("Module");
    }
    if(!write())
        qDebug()<<"delete job failed";
}

bool writeJob::dragThisJob(int index,int allRow,QString upOrDown)
{
    qDebug()<<"3.job will be drag to the xml"<<index;
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
        dragModuleName(moduleName,index,allRow,upOrDown);
        alreadyReadJob=true;

        fileRead.close();
        return true;
    }
}

void writeJob::dragModuleName(const QString name, const int index, const int allRow, const QString upOrDown)
{
    qDebug()<<"4.dragModuleName";
    QDomElement root = doc.documentElement();
    QDomElement modEle = root.firstChildElement("Module");
    if(index==0)  //插到头
    {
        while (!modEle.isNull())
        {
            if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
            {
                QDomElement firstModEle=root.firstChildElement("Module");
                root.insertBefore(modEle,firstModEle);
                break;
            }
            modEle = modEle.nextSiblingElement("Module");
        }
    }
    else if(index==(allRow-1))  // 插到尾
    {
        while (!modEle.isNull())
        {
            if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
            {
                QDomElement lastModEle=root.lastChildElement("Module");
                root.insertAfter(modEle,lastModEle);
                break;
            }
            modEle = modEle.nextSiblingElement("Module");
        }
    }

    else   //其他情况
    {
        QDomElement insertAfterEle;
        QDomElement rightEle;

        insertAfterEle=modEle;
        //难道还要分上到下，下到上??
        if(upOrDown=="up")
        {
            for(int i=0; i<index-1;i++)
            {
                modEle = modEle.nextSiblingElement("Module");
                insertAfterEle=modEle;
                qDebug()<<i<<insertAfterEle.attribute("name");
            }
        }
        if(upOrDown=="down")
        {
            for(int i=0; i<index;i++)
            {
                modEle = modEle.nextSiblingElement("Module");
                insertAfterEle=modEle;
                qDebug()<<i<<insertAfterEle.attribute("name");
            }
        }
        modEle=root.firstChildElement("Module");
        while (!modEle.isNull())
        {
            if(modEle.attribute("name")==name && modEle.attribute("id")==moduleID)
            {
                rightEle=modEle;
                qDebug()<<rightEle.attribute("name")<<insertAfterEle.attribute("name");
                root.insertAfter(rightEle,insertAfterEle);
                break;
            }
            modEle = modEle.nextSiblingElement("Module");
        }
    }

    if(!write())
        qDebug()<<"delete job failed";
}

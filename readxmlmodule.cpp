#include "readxmlmodule.h"
#include <QMessageBox>
#include <QtXml>

readXMLModule::readXMLModule(QObject *parent) :
    QObject(parent)
{
    // init
    domDocument.clear();
    hasProperty=false;
    hasDesc=false;
    hasDataType=false;
    hasMax=false;
    hasMin=false;
    hasDisplayType=false;
    hasOption=false;
}

void readXMLModule::nodeClear()
{
    hasProperty=false;
    hasDesc=false;
    hasDataType=false;
    hasMax=false;
    hasMin=false;
    hasDisplayType=false;
    hasOption=false;

    property.clear();
    desc.clear();
    datatype.clear();
    max.clear();
    min.clear();
    displaytype.clear();
    displayvalue.clear();
    optiontext.clear();
    optionvalue.clear();
    showvalue.clear();
}

void readXMLModule::setXML(QIODevice */*d*/)
{
}

bool readXMLModule::read(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    // 这是一切的开始，将所有数据全部读入了，所以修改之后写出就是了，非常方便
    if (!domDocument.setContent(device, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "ui")   // tag 限定
    {
        qDebug()<<tr("The file is not an ui  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an ui version 1.0 file.");
        return false;
    }
    QDomElement module = root.firstChildElement("module");
    modulename=module.attribute("name");
    emit sigModuleName(modulename);
//    qDebug()<<modulename;

    QDomElement child=module.firstChildElement("property");
    /// 只处理property,当中有其他的可不行，例如隐藏之类的
    while (!child.isNull())
    {
        nodeClear();  //清理节点包含信息
        property = child.attribute("name");

        parsePropertyElement(child);

        // 设置一套机制传送信息
        /// 貌似有没有没有必要
        emit sigNodeData(property,desc,datatype,min,max,displaytype,displayvalue,optiontext,optionvalue,showvalue);   //9

        child = child.nextSiblingElement("property");
    }
    emit sigReadFinish();

    return true;
}

/// 在这里要处理多种类型的控件，还要分开进行处理
void readXMLModule::parsePropertyElement(const QDomElement &element)
{
//    qDebug()<<"parsePropertyElement";
    hasProperty=true;
    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        if (child.tagName() == "property")
        {
            parsePropertyElement(child);  //同根递归
        }
        else if (child.tagName() == "desc")
        {
            desc=child.text();
//            qDebug()<<"desc:: "<<desc;
            hasDesc=true;
            /// datatype is not a child
        }

        /// 取值范围竟然是和名称同一级别。。。
        else if (child.tagName() == "datatype")
        {
            datatype=child.text();
//            qDebug()<<"datatype::"<<datatype;
            hasDataType=true;
        }
        else if (child.tagName() == "max")
        {
            max=child.text();
//            qDebug()<<"max::"<<max;
            hasMax=true;
        }
        else if (child.tagName() == "min")
        {
            min=child.text();
//            qDebug()<<"min::"<<min;
            hasMin=true;
        }

        /// 还有些特定的属性
        else if (child.tagName() == "displaytype")
        {
            QDomElement option;
            displaytype=child.attribute("name");
            displayvalue=child.text();
//            qDebug()<<"displaytype::"<<displaytype;
            hasDisplayType=true;
            if(child.hasChildNodes())   /// 会包含多个nodes
            {
                option=child.firstChildElement("option");
                // 遍历所有节点, 还需要知道节点的value
                while(!option.isNull())
                {
//                    qDebug()<<"type has childnode";   // 1 is enough
                    hasOption=true;
                    optionvalue<<option.text();
                    optiontext<<option.attribute("value");
//                    qDebug()<<option.attribute("value")<<option.text();
                    option=option.nextSiblingElement();
                }
            }
        }
        else if (child.tagName() == "visible")  /// 用来取代label?
        {
            showvalue=child.text();   //YES=show, NO=hide
        }
        child = child.nextSiblingElement(); //循环
    }
}

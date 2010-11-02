#include "readxmlindex.h"

readXMLIndex::readXMLIndex(QObject *parent) :
    QObject(parent)
{
    allModuleName.clear();
    allModuleVersion.clear();
    allModulePath.clear();
}

void readXMLIndex::setXML(QIODevice */*d*/)
{}

void readXMLIndex::nodeClear()
{
    name.clear();
    version.clear();
    path.clear();
    desc.clear();
}

bool readXMLIndex::read(QIODevice *device)
{
    qDebug()<<"read";
    QString errorStr;
    int errorLine;
    int errorColumn;

    // 错误处理
    if (!domDocument.setContent(device, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "index")   // tag 限定
    {
        qDebug()<<tr("The file is not an index  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an index version 1.0 file.");
        return false;
    }

    QDomElement modulegroup = root.firstChildElement("modulegroup");
    QDomElement child=modulegroup.firstChildElement("module");
    /// 只处理property,当中有其他的可不行，例如隐藏之类的
    while (!child.isNull())
    {
        nodeClear();  //清理节点包含信息
        name = child.attribute("name");
        version = child.attribute("version");
//        qDebug()<<"module name::"<<name;
//        qDebug()<<"module version::"<<version;
        parsePropertyElement(child);

        setModuleName(name,version,path,desc);
        qDebug()<<name<<version<<path<<desc;
        child = child.nextSiblingElement("module");
    }

    return true;
}


void readXMLIndex::parsePropertyElement(const QDomElement &element)
{
    QDomElement child = element.firstChildElement();
    while (!child.isNull())
    {
        if (child.tagName() == "module")
        {
            parsePropertyElement(child);  //同根递归
        }
        else if (child.tagName() == "path")
        {
            path=child.text();
//            qDebug()<<"moduleXMLpath:: "<<path;
        }
        else if (child.tagName() == "desc")
        {
            desc=child.text();
//            qDebug()<<"module desc:: "<<desc;
        }
        child = child.nextSiblingElement(); //循环
    }
}

void readXMLIndex::setModuleName(QString moduleName, QString moduleVersion,
                                 QString modulePath, QString moduleDesc)
{
    allModuleName.append(moduleName);
    allModuleVersion.append(moduleVersion);
    allModulePath.append(modulePath);
    allModuleDesc.append(moduleDesc);
}

QStringList readXMLIndex::getModuleName()
{
    return allModuleName;
}

QStringList readXMLIndex::getModuleVersion()
{
    return allModuleVersion;
}

QStringList readXMLIndex::getModulePath()
{
    return allModulePath;
}

QStringList readXMLIndex::getModuleDesc()
{
    return allModuleDesc;
}

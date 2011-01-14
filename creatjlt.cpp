#include "creatjlt.h"
#include "src/qjdprocess.h"
#include <QMessageBox>

creatJLT::creatJLT(QObject *parent) :
    QObject(parent)
{
    xmlFileName.clear();
    exFile.setFileName("joblist/ceshishuchu.txt");
    count=99;
    processCount=0;
    hashJob.clear();
}

bool creatJLT::setXMLJobListFileName(const QString fileName)
{
    qDebug()<<"set excute file name"<<fileName;
    xmlFileName=fileName;
    return true;
}

void creatJLT::analysisJobXML()
{
    fileXMLJob.setFileName(xmlFileName);
    fileXMLJob.open(QFile::ReadOnly);
    readXmlJobList(&fileXMLJob);
}

bool creatJLT:: readXmlJobList(QIODevice *device)
{
    qDebug()<<"creatJLT:: read"<<count;
    if (!exFile.open(QIODevice::WriteOnly))
    {
        qDebug()<<"exFile open field";
    }
    else
    {
        qDebug()<<"it is now in else~~~~~~~~~~~~~~~~~~";
        textStream.setDevice(&exFile);
        textStream<<processCount<<"\n";
        textStream<<"/home/lub/program/qt/flow/MainFlow\n";
        textStream<<"job";
        textStream<<processCount;
        textStream<<".log\n";
        textStream<<count;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    // 错误处理
    domDocument.clear();
    if (!domDocument.setContent(device, true, &errorStr, &errorLine,&errorColumn))
    {
        // 错误处理，先不必要
        qDebug()<<tr("setContent error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        return false;
    }
    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "job")   // tag 限定
    {
        qDebug()<<tr("The file is not an index  file.");
        return false;
    }
    else if (root.hasAttribute("version")  && root.attribute("version") != "1.0")  //还要限定版本...
    {
        qDebug()<< tr("The file is not an job version 1.0 file.");
        return false;
    }

    QDomElement moduleNameEle = root.firstChildElement("Module");
    qDebug()<<moduleNameEle.attribute("name"); // 这是模块名称,需要多次读取

    while (!moduleNameEle.isNull())
    {
        if(moduleNameEle.attribute("status")=="on")
        {
            QString name = moduleNameEle.attribute("name");
            textStream<<"\n"<<name<<"|";
            parseModuleElement(moduleNameEle);
        }
        moduleNameEle = moduleNameEle.nextSiblingElement("Module");
    }

    exFile.close();
    fileXMLJob.close();
    qDebug()<<"exFile closed";
    return true;
}

 void creatJLT::startRun()
 {
     qDebug()<<"start Run";
     if(hashJob.values().size()==4)
     {
         qDebug()<<"No more than 4 process in one time.";
//         QMessageBox::warning(this,"Not Allowed","No more than 4 process in one time.");
         return;
     }

     if(!hashJob.values().contains(4))
         processCount=4;
     if(!hashJob.values().contains(3))
         processCount=3;
     if(!hashJob.values().contains(2))
         processCount=2;
     if(!hashJob.values().contains(1))
         processCount=1;

     analysisJobXML();
     RunProcess();
 }
 void creatJLT::RunProcess()
  {
      QStringList paralist;
      paralist.append("joblist/ceshishuchu.txt");

      QJDProcess *JDP=new QJDProcess;
      connect(JDP, SIGNAL(sigFinished(int,int,QProcess::ExitStatus)), this, SLOT(processFinished(int, int, QProcess::ExitStatus)));
      JDP->start("MainFlow",paralist);
      JDP->savePID();
      hashJob.insert(JDP->getPID(),processCount);
  }

 void creatJLT::processFinished(int pid, int sig, QProcess::ExitStatus exitStatus)
  {
      qDebug()<<pid<<sig<<exitStatus;
      hashJob.remove(pid);
  }

 /// 处理各个模块
 void creatJLT::parseModuleElement(QDomElement const& moduleNameEle)
 {
     QDomElement child=moduleNameEle.firstChildElement("property");

     while (!child.isNull())
     {
         QString name = child.attribute("name");  
         parsePropertyElement(child);
         child = child.nextSiblingElement("property");
     }
 }

 /// 处理各个属性
void creatJLT::parsePropertyElement(QDomElement const& property)
{
    // 需要考,考虑控件类型
    QDomElement displayType=property.firstChildElement("displaytype");
    QDomElement hideEle=property.firstChildElement("hide");
    if(hideEle.text()=="YES")
    {
        return;
    }

    QString widgetType=displayType.attribute("name");
    if(widgetType=="lineedit" || widgetType=="spinbox")
    {
        qDebug()<<"line/spin write in as:: "<<property.attribute("name")<<displayType.text();
        textStream<<property.attribute("name")<<"="<<displayType.text()<<";";
    }
    if(widgetType=="checkbox")
    {
        if(displayType.text()=="checked")
        {
            qDebug()<<"check write in as:: "<<property.attribute("name")<<"1";
            textStream<<property.attribute("name")<<"="<<"1"<<";";
        }
        if(displayType.text()=="unchecked")
        {
            qDebug()<<"check write in as:: "<<property.attribute("name")<<"0";
            textStream<<property.attribute("name")<<"="<<"0"<<";";
        }
    }
    if(widgetType=="combobox")
    {
        /// 还要处理下
        qDebug()<<"widgetType is combobox";
        QDomElement option=displayType.firstChildElement("option");
        while(!option.isNull())
        {
            if(option.text()=="checked")
            {
                qDebug()<<"combo write in as:: "<<property.attribute("name")<<option.attribute("value");
                textStream<<property.attribute("name")<<"="<<option.attribute("value")<<";";
            }
            option=option.nextSiblingElement("option");
        }
    }
    if(widgetType=="radiobutton")
    {
        qDebug()<<"widgetType is radioButton";
        QDomElement option=displayType.firstChildElement("option");
        while(!option.isNull())
        {
            if(option.text()=="checked")
            {
                qDebug()<<"radio write in as:: "<<property.attribute("name")<<option.attribute("value");
                textStream<<property.attribute("name")<<"="<<option.attribute("value")<<";";
            }
            option=option.nextSiblingElement("option");
        }
    }
}


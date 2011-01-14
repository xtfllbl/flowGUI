#ifndef READXML_H
#define READXML_H

#include <QObject>
#include <QIODevice>
#include <QtXml>
class readXMLModule : public QObject
{
    Q_OBJECT
public:
    explicit readXMLModule(QObject *parent = 0);

signals:    
    // 发送节点信息
    void sigNodeData(QString property,QString desc,QString datatype,
                     QString min,QString max,QString displaytype,QString displaytext,QString displayvalue,
                     QStringList optiontext,QStringList optionvalue,QString hidevalue,QString hidetype);
    // 发送模块名称
    void sigModuleName(QString modulename);
    void sigReadFinish();


public slots:
    void setXML(QIODevice *d);
    bool read(QIODevice *device);
    void parsePropertyElement(const QDomElement &element);

private:
    QDomDocument domDocument;
    bool hasProperty;
    bool hasDesc;
    bool hasDataType;
    bool hasMax;
    bool hasMin;
    bool hasDisplayType;
    bool hasOption;

    QString modulename;
    QString property;
    QString desc;
    QString datatype;
    QString max;
    QString min;
    QString displaytype;
    QString displaytext;
    QString displayvalue;
    QString hidevalue;
    QString hidetype;
    QStringList optiontext;
    QStringList optionvalue;

    void nodeClear();

};

#endif // READXML_H

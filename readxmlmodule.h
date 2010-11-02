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
    // 发送信息，每当要添加东西，就显得非常不方便
    void sigNodeData(QString property,QString desc,QString datatype,
                     QString min,QString max,QString displaytype,QString displayvalue,
                     QStringList optiontext,QStringList optionvalue,QString showvalue);
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
    QString displayvalue;
    QString showvalue;
    QStringList optiontext;
    QStringList optionvalue;

    void nodeClear();

};

#endif // READXML_H

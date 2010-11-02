#ifndef READXMLINDEX_H
#define READXMLINDEX_H

#include <QObject>
#include <QIODevice>
#include <QtXml>

class readXMLIndex : public QObject
{
    Q_OBJECT
public:
    explicit readXMLIndex(QObject *parent = 0);

signals:
    void sigModuleData(QString modulename,QString moduleversion,QString moduleXMLpath);

public slots:
    void setXML(QIODevice *d);
    bool read(QIODevice *device);
    void parsePropertyElement(const QDomElement &element);

    void setModuleName(QString moduleName,QString moduleVersion,QString modulePath,QString moduleDesc);
    QStringList getModuleName();
    QStringList getModuleVersion();
    QStringList getModulePath();
    QStringList getModuleDesc();
private:
    QDomDocument domDocument;

    QString name;
    QString version;
    QString path;
    QString desc;

    QStringList allModuleName;
    QStringList allModuleVersion;
    QStringList allModulePath;
    QStringList allModuleDesc;
    void nodeClear();
};

#endif // READXMLINDEX_H

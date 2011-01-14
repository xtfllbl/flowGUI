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

    void setModuleName(QString moduleName,QString moduleVersion,QString modulePath,
                       QString moduleDesc, QString moduleType);
    QStringList getModuleName();
    QStringList getModuleVersion();
    QStringList getModulePath();
    QStringList getModuleDesc();
    QStringList getModuleType();
private:
    QDomDocument domDocument;

    QString name;
    QString version;
    QString path;
    QString desc;
    QString type;

    QStringList allModuleName;
    QStringList allModuleVersion;
    QStringList allModulePath;
    QStringList allModuleDesc;
    QStringList allModuleType;
    void nodeClear();
};

#endif // READXMLINDEX_H

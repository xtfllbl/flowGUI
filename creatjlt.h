#ifndef CREATJLT_H
#define CREATJLT_H

#include <QObject>
#include <QtXml>
#include <QFile>

class creatJLT : public QObject
{
    Q_OBJECT
public:
    explicit creatJLT(QObject *parent = 0);

signals:

private:
    QString xmlFileName;
    QFile fileXMLJob;
    QDomDocument domDocument;

    void parseModuleElement(QDomElement const&);
    void parsePropertyElement(const QDomElement &element);
    bool readXmlJobList(QIODevice *);
    void creatTxtJobList();
    void analysisJobXML();


    QFile exFile;
    QTextStream textStream;

    int count;
public slots:
    bool setXMLJobListFileName(const QString fileName);
    void startRun();
};

#endif // CREATJLT_H

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
    void analysisJobXML();
    void RunProcess();

    QFile exFile;
    QTextStream textStream;

    int count;
    int processCount;

    QHash<int , int> hashJob;

public slots:
    bool setXMLJobListFileName(const QString fileName);
    void startRun();
    void processFinished(int pid, int sig, QProcess::ExitStatus);

};

#endif // CREATJLT_H

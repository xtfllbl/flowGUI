#ifndef WRITEJOB_H
#define WRITEJOB_H

#include <QObject>
#include <QtXml>

class writeJob : public QObject
{
    Q_OBJECT
public:
    explicit writeJob(QObject *parent = 0);

signals:

public slots:

//    void writeJobList();

    void addNormalWidget(const QString property, const QString desc, const QString datatype,
                         const QString min, const QString max, const QString displaytype,
                         const QString displayvalue, const QStringList optiontext, const QStringList optionvalue);
    void addVisibleWidget(const QString property, const QString desc, const QString datatype,
                          const QString min, const QString max, const QString displaytype,
                          const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                          const QString visible);
    void setJobFileName(QString fileName);
private:
    QString jobFileName;
    bool isFileNameSet;
    QFile fileJob;
    QFile fileRead;
    QFile fileWrite;
    QDomDocument doc;

    QString errorStr;
    int errorLine;
    int errorColumn;

    /// ----------------------------------------///
    bool initJob();
    bool readJob();
    bool firstWriteJob();
    void writeHead(QIODevice *);
    bool write(QIODevice *device);


    void setModuleName(QString);
    void setProperty(QString);
    void setDesc(QString);
    void setDatatype(QString);
    void setMax(QString);
    void setMin(QString);
    void setDisplayType(QString);
    void setDisplayValue(QString);
    void setShowValue(QString);
    void setOptionText(QString);
    void setOptionValue(QString);
};

#endif // WRITEJOB_H

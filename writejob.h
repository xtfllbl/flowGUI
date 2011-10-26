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
//    void sigLineEditChanged(QString);
public slots:

//    void writeJobList();

    void setModuleName(const QString name,const QString id);

    void addNormalWidget(const QString property, const QString desc, const QString datatype,
                         const QString min, const QString max, const QString displaytype,
                         const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                         const QString id, const QString boxID, const QList<QString> radioIDList);
    void addHideWidget(const QString property, const QString desc, const QString datatype,
                          const QString min, const QString max, const QString displaytype,
                          const QString displayvalue, const QStringList optiontext, const QStringList optionvalue,
                          const QString visible, const QString boxID, const QString lineID);
    void setJobFileName(QString fileName);

    bool lineEditChanged(QString text, QString id);
    bool lineEditChanged(QString text, QString boxID, QString id);
    bool checkBoxChanged(QString text, QString id);
    bool comboBoxChanged(QString text, QString id);
    bool spinBoxChanged(QString text, QString id);
    bool radioButtonChanged(QString text, QString id,QString otherID);
    bool groupBoxChanged(bool checked,QString id);

    bool deleteThisJob();  //删除这项
//    bool upThisJob();
    bool resetThisJob(int upValue);
//    bool downThisJob();
    bool turnOnThisJob();
    bool turnOffThisJob();
    bool dragThisJob(int,int,QString);

private:
    QString jobFileName;
    QString moduleName;
    QString moduleID;
    bool isFileNameSet;
    QFile fileJob;
    QFile fileRead;
    QFile fileWrite;
    QFile fileDelete;
    QDomDocument doc;
    QDomElement moduleEle;

    QString errorStr;
    int errorLine;
    int errorColumn;

    bool alreadyReadJob;    
    /// ----------------------------------------///
    bool initJob();
    bool readJob();
    bool write();
    void writeHead(QIODevice *);

    void parseModuleElement(QDomElement const& moduleNameEle,QString text,QString id);
    void parseModuleElement(QDomElement const& moduleNameEle,QString text,QString id, QString boxID);
    void parseModuleHideElement(QDomElement const& moduleNameEle,bool checked,QString id);
    void parseModuleHideElement(QDomElement const& moduleNameEle,QString text, QString boxID,QString id);

    void parsePropertyElement(QDomElement property,QString text);
    void parsePropertyElement(QDomElement property,QString text,QString id);
    void parsePropertyHideElement(QDomElement property,QString text,QString id);
    void parsePropertyHideElement(QDomElement property,bool checked);

    void addModuleName(const QString name,const QString id); //添加的时候需要id
    void delModuleName(const QString name);  // 其他时候只需要检查id
//    void upModuleName(const QString name);
    void resetModuleName(const QString name);
//    void downModuleName(const QString name);
    void turnOnModuleName(const QString name);
    void turnOffModuleName(const QString name);
    void dragModuleName(const QString name, const int index, const int allRow,const QString upOrDown);
};

#endif // WRITEJOB_H

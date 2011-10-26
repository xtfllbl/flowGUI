#ifndef QJDHISTORYBROWSER_H
#define QJDHISTORYBROWSER_H

#include <QTableWidget>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include<QDateTime>

class QJDHistoryBrowser : public QTableWidget
{
    Q_OBJECT
public:
    explicit QJDHistoryBrowser(QWidget *parent = 0);

    QHash<int,QString> hashFilePath;  //保存着文件的路径
private:
    QDir logDir;
    QFile logFile1;
    QFile logFile2;
    QFile logFile3;
    QFile logFile4;
    QFileInfoList fileInfoList;

    bool _hasLog;
    int saveSelect;


    void setLogFile();
    QString getLogStat(QString fileName);
    int getLogProgress(QString fileName);
signals:
    void showLog(QString fileName);
private slots:
    void emitShowLog(QModelIndex);
public slots:
    void clearAndReset();
    bool hasLog();

    void mousePressEvent(QMouseEvent *event);

};

#endif // QJDHISTORYBROWSER_H

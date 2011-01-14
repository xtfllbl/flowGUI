#ifndef QJDGROUPBOX_H
#define QJDGROUPBOX_H

#include <QGroupBox>

class QJDGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit QJDGroupBox(QWidget *parent = 0);

    QString id();
signals:
    void sigGroupChecked(bool,QString);
public slots:
private:
    QString _id;
    QString setRandString(QString & randString);

//    void setChecked(bool checked);  // 没法重载，涉嫌原有的私有成员
//    void _q_setChildrenEnabled(bool b);
private slots:
    void emitGroupChecked(bool);
};

#endif // QJDGROUPBOX_H

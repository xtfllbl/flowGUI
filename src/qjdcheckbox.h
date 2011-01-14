#ifndef QJDCHECKBOX_H
#define QJDCHECKBOX_H

#include <QCheckBox>

class QJDCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit QJDCheckBox(QWidget *parent = 0);

    QString id();
signals:
    void sigStateChanged(QString state, QString id);

public slots:
private:
    QString _id;
    QString setRandString(QString & randString);

private slots:
    void emitStateChanged(int);

};

#endif // QJDCHECKBOX_H

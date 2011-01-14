#ifndef QJDSPINBOX_H
#define QJDSPINBOX_H

#include <QSpinBox>

class QJDSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit QJDSpinBox(QWidget *parent = 0);

    QString id();
signals:
    void sigValueChanged(QString value, QString id);

public slots:
private:
    QString _id;
    QString setRandString(QString & randString);
private slots:
    void emitValueChanged(QString);
};

#endif // QJDSPINBOX_H

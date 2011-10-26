#ifndef QJDFILECOMBOBOX_H
#define QJDFILECOMBOBOX_H

#include <QComboBox>

class QJDFileComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QJDFileComboBox(QWidget *parent = 0);
    QString id();

signals:
    void sigIndexChanged(QString text, QString id);

public slots:
private:
    QString _id;
    QString setRandString(QString & randString);
private slots:
    void emitIndexChanged(QString);

};

#endif // QJDFILECOMBOBOX_H

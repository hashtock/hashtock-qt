#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QQmlEngine>
//#include <QQmlApplicationEngine>

class Helper : public QObject
{
    Q_OBJECT
public:
    explicit Helper(QObject *parent = 0);
//    QQmlApplicationEngine *engine;
    QQmlEngine *engine;

public slots:
    void onLoadMe();
    void slotReadReady();

};

#endif // HELPER_H

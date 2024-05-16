#ifndef MEDIACENTRE_H
#define MEDIACENTRE_H

#include <QObject>
#include "MyPlayer.h"

class MediaCentre : public QObject
{
    Q_OBJECT
public:
    explicit MediaCentre(QObject *parent = nullptr);
    void Init();
    void AsyncPreparePlayer();
    void SetMediaSourcePath(std::string path);
    void StopPlayer();

signals:

public slots:

private:
    MyPlayer *pPlayer = nullptr;
};

#endif // MEDIACENTRE_H

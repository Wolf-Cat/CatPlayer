#ifndef MEDIACENTRE_H
#define MEDIACENTRE_H

#include <QObject>
#include <memory>
#include "../player/MyPlayer.h"
#include "SDL.h"

class MediaCentre : public QObject
{
    Q_OBJECT
public:
    explicit MediaCentre(QObject *parent = nullptr);
    void Init();
    void AsyncPreparePlayer();
    static int PlayerMsgloop(void* arg);
    void SetMediaSourcePath(std::string path);
    void StopPlayer();

signals:

public slots:

private:
    std::shared_ptr<MyPlayer> m_pPlayer = nullptr;

    SDL_mutex *m_pMutex = nullptr;
    SDL_Thread *m_pMsgThread = nullptr;   // 消息循环的线程
};

#endif // MEDIACENTRE_H

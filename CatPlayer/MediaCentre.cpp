#include "MediaCentre.h"

MediaCentre::MediaCentre(QObject *parent) : QObject(parent)
{

}

void MediaCentre::Init()
{
    AsyncPreparePlayer();
}

void MediaCentre::AsyncPreparePlayer()
{
    // 创建消息循环线程 （主界面和播放器之间的消息循环）
    if (m_pMsgThread != nullptr) {
        return;
    }

    m_pMsgThread = SDL_CreateThread(PlayerMsgloop, "PlayerMsgloop", this);

    if (m_pPlayer == nullptr) {
        m_pPlayer = std::make_shared<MyPlayer>();
        m_pPlayer->StreamOpen("testVideo.mp4");
    }
}

int MediaCentre::PlayerMsgloop(void* arg)
{
    return 0;
}

void MediaCentre::SetMediaSourcePath(std::string path)
{

}

void MediaCentre::StopPlayer()
{

}

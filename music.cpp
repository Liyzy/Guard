#include "music.h"
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>

// Ϊ�˽��mac�������޷����,��֮����ʹ�þ���·���������Ŀ��,����,����Դ��ַ�ʽ��ʱ����
static const QString s_curDir = QDir::currentPath() + "/";

AudioPlayer::AudioPlayer(QObject *parent)
    : QObject(parent)
    , m_backgroundMusic(NULL)
{
    // ����һֱ���ŵı�������
    QUrl backgroundMusicUrl = QUrl::fromLocalFile(s_curDir + "../GUARD/music/8bitDungeonLevel.mp3");
    if (QFile::exists(backgroundMusicUrl.toLocalFile()))
    {
        m_backgroundMusic = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();

        QMediaContent media(backgroundMusicUrl);
        backgroundMusicList->addMedia(media);
        backgroundMusicList->setCurrentIndex(0);
        // ���ñ�������ѭ������
        backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_backgroundMusic->setPlaylist(backgroundMusicList);
    }
}

void AudioPlayer::startBGM()
{
    if (m_backgroundMusic)
        m_backgroundMusic->play();
}

void AudioPlayer::playSound(SoundType soundType)
{
    static const QUrl mediasUrls[] =
    {
        QUrl::fromLocalFile(s_curDir + "../GUARD/music/tower_place.wav"),
        QUrl::fromLocalFile(s_curDir + "../GUARD/music/life_lose.wav"),
        QUrl::fromLocalFile(s_curDir + "../GUARD/music/laser_shoot.wav"),
        QUrl::fromLocalFile(s_curDir + "../GUARD/music/enemy_destroy.wav")
    };
    static QMediaPlayer player;

    if (QFile::exists(mediasUrls[soundType].toLocalFile()))
    {
        player.setMedia(mediasUrls[soundType]);
        player.play();
    }
}

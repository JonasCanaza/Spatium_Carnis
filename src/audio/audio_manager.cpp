#include "audio_manager.h"

#include <raylib.h>

namespace AudioManager
{
    struct MusicData
    {
        Music music;
        bool isPaused;
    };

    struct SfxData
    {
        Sound sound;
        bool isPaused;
    };

    static MusicData musicTracks[MusicID::MUSIC_COUNT];
    static SfxData sfxTracks[SFX_COUNT];

    static bool IsValidMusicIndex(int id);
    static bool IsValidSfxIndex(int id);

    void Init() // IMPORTANT: ONLY DO "Init();" IN "game.cpp"
    {
        InitAudioDevice();

        musicTracks[MusicID::MUSIC_MENU] = { LoadMusicStream("res/sounds/music/menu.mp3"), false };
        musicTracks[MusicID::MUSIC_MENU].music.looping = true;

        musicTracks[MusicID::MUSIC_GAMEPLAY] = { LoadMusicStream("res/sounds/music/gameplay.mp3"), false };
        musicTracks[MusicID::MUSIC_GAMEPLAY].music.looping = true;
    }

    void Update()
    {
        for (int i = 0; i < MUSIC_COUNT; i++)
        {
            UpdateMusicStream(musicTracks[i].music);
        }
    }

    void Close() // IMPORTANT: ONLY DO "Close();" IN "game.cpp"
    {
        for (int i = 0; i < MUSIC_COUNT; i++)
        {
            UnloadMusicStream(musicTracks[i].music);
        }

        for (int i = 0; i < SFX_COUNT; i++)
        {
            UnloadSound(sfxTracks[i].sound);
        }

        CloseAudioDevice();
    }

    void PlayMusic(MusicID id)
    {
        int musicID = static_cast<int>(id);

        if (!IsValidMusicIndex(musicID))
        {
            return;
        }

        PlayMusicStream(musicTracks[musicID].music);
        musicTracks[musicID].isPaused = false;
    }

    void PauseMusic(MusicID id)
    {
        int musicID = static_cast<int>(id);

        if (!IsValidMusicIndex(musicID))
        {
            return;
        }

        if (!musicTracks[musicID].isPaused)
        {
            PauseMusicStream(musicTracks[musicID].music);
            musicTracks[musicID].isPaused = true;
        }
    }

    void ResumeMusic(MusicID id)
    {
        int musicID = static_cast<int>(id);

        if (!IsValidMusicIndex(musicID))
        {
            return;
        }

        if (musicTracks[musicID].isPaused)
        {
            ResumeMusicStream(musicTracks[musicID].music);
            musicTracks[musicID].isPaused = false;
        }
    }

    void StopMusic(MusicID id)
    {
        int musicID = static_cast<int>(id);

        if (!IsValidMusicIndex(musicID))
        {
            return;
        }

        StopMusicStream(musicTracks[musicID].music);
        musicTracks[musicID].isPaused = false;
    }

    void PlaySfx(SfxID id)
    {
        int sfxID = static_cast<int>(id);

        if (!IsValidSfxIndex(sfxID))
        {
            return;
        }

        PlaySound(sfxTracks[sfxID].sound);
        sfxTracks[sfxID].isPaused = false;
    }

    static bool IsValidMusicIndex(int id)
    {
        return id >= 0 && id < MUSIC_COUNT;
    }

    static bool IsValidSfxIndex(int id)
    {
        return id >= 0 && id < SFX_COUNT;
    }
}
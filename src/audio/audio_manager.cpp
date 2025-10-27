#include "audio_manager.h"

#include <vector>
#include <string>

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
        std::string sourceFile;
        std::vector<Sound> instances;
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

        sfxTracks[SFX_BUTTON_HOVER].sourceFile = "res/sounds/sfx/button/hover.wav";
        sfxTracks[SFX_BUTTON_PRESSED].sourceFile = "res/sounds/sfx/button/pressed.wav";
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
            for (int j = 0; j < static_cast<int>(sfxTracks[i].instances.size()); j++)
            {
                UnloadSound(sfxTracks[i].instances[j]);
            }

            sfxTracks[i].instances.clear();
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

        bool played = false;
        SfxData& track = sfxTracks[sfxID];

        for (int i = 0; i < static_cast<int>(track.instances.size()); i++)
        {
            if (!IsSoundPlaying(track.instances[i]))
            {
                PlaySound(track.instances[i]);
                played = true;
                break;
            }
        }

        if (!played)
        {
            Sound newSound = LoadSound(track.sourceFile.c_str());
            PlaySound(newSound);
            track.instances.push_back(newSound);
        }

        for (int i = 0; i < static_cast<int>(track.instances.size());)
        {
            if (!IsSoundPlaying(track.instances[i]) && i != track.instances.size() - 1)
            {
                UnloadSound(track.instances[i]);
                track.instances.erase(track.instances.begin() + i);
            }
            else
            {
                i++;
            }
        }
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
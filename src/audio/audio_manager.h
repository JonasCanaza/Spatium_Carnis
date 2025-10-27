#pragma once

namespace AudioManager
{
    enum MusicID
    {
        MUSIC_MENU,
        MUSIC_GAMEPLAY,
        MUSIC_COUNT
    };

    enum SfxID
    {
        SFX_SHOOT,
        SFX_EXPLOSION,
        SFX_BUTTON,
        SFX_COUNT
    };

    void Init();
    void Update();
    void Close();

    void PlayMusic(MusicID id);
    void PauseMusic(MusicID id);
    void ResumeMusic(MusicID id);
    void StopMusic(MusicID id);

    void PlaySfx(SfxID id);
}
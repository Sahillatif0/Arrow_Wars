#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>

class SoundManager {
    std::unordered_map<std::string, Sound> sounds;
    bool enabled;

public:
    SoundManager() : enabled(true) {}

    void load(const std::string& name, const std::string& path) {
        if (sounds.find(name) == sounds.end()) {
            sounds[name] = LoadSound(path.c_str());
        }
    }

    void play(const std::string& name) {
        if (enabled && sounds.find(name) != sounds.end()) {
            PlaySound(sounds[name]);
        }
    }

    void setEnabled(bool value) { enabled = value; }
    bool isEnabled() const { return enabled; }

    void unloadAll() {
        for (auto& pair : sounds) {
            UnloadSound(pair.second);
        }
        sounds.clear();
    }

    ~SoundManager() {
        unloadAll();
    }
};

// Global sound manager instance
static SoundManager g_soundManager;

#endif // SOUND_MANAGER_H
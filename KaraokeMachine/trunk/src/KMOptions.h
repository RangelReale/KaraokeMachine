#ifndef H_KMOPTIONS_H
#define H_KMOPTIONS_H

/**
 * @class KMOptions
 *
 * @brief Options
 */
class KMOptions {
public:
    KMOptions() : midiport_(-1) {}

    int GetMidiPort() { return midiport_; }
    void SetMidiPort(int v) { midiport_=v; }
private:
    int midiport_;
};


#endif //H_KMOPTIONS_H

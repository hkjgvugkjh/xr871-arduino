// Minimal Audio stub
#ifndef _AUDIO_XR871_H_
#define _AUDIO_XR871_H_
#include <stdint.h>
typedef enum { AUDIO_SAMPLE_RATE_8K=8000, AUDIO_SAMPLE_RATE_11K=11025, AUDIO_SAMPLE_RATE_16K=16000, AUDIO_SAMPLE_RATE_22K=22050, AUDIO_SAMPLE_RATE_32K=32000, AUDIO_SAMPLE_RATE_44K=44100, AUDIO_SAMPLE_RATE_48K=48000 } audio_sample_rate_t;
typedef enum { AUDIO_BITS_PER_SAMPLE_8=8, AUDIO_BITS_PER_SAMPLE_16=16, AUDIO_BITS_PER_SAMPLE_24=24, AUDIO_BITS_PER_SAMPLE_32=32 } audio_bits_per_sample_t;
typedef enum { AUDIO_CHANNELS_MONO=1, AUDIO_CHANNELS_STEREO=2 } audio_channels_t;
typedef enum { OUTPUT_MODE_I2S=0, OUTPUT_MODE_DAC, OUTPUT_MODE_PWM } audio_output_mode_t;
typedef enum { INPUT_MODE_I2S=0, INPUT_MODE_DMIC, INPUT_MODE_ADC, INPUT_MODE_PDM } audio_input_mode_t;
class AudioOutput {
public:
    AudioOutput() {}
    ~AudioOutput() {}
    bool begin(audio_sample_rate_t=AUDIO_SAMPLE_RATE_16K, audio_bits_per_sample_t=AUDIO_BITS_PER_SAMPLE_16, audio_channels_t=AUDIO_CHANNELS_MONO) { return false; }
    void end() {}
    size_t write(const uint8_t*, size_t) { return 0; }
    size_t write(int16_t) { return 0; }
    size_t writeSamples(const int16_t*, size_t) { return 0; }
    bool playFile(const char*) { return false; }
    bool playWAV(const uint8_t*, size_t) { return false; }
    bool playMP3(const uint8_t*, size_t) { return false; }
    bool playAAC(const uint8_t*, size_t) { return false; }
    bool startStream() { return false; }
    bool stopStream() { return false; }
    void setVolume(float) {}
    float getVolume() { return 0; }
    void setMute(bool) {}
    bool isMuted() { return false; }
    bool isPlaying() { return false; }
    bool isPaused() { return false; }
    bool pause() { return false; }
    bool resume() { return false; }
    bool stop() { return false; }
    void setSampleRate(audio_sample_rate_t) {}
    void setBitsPerSample(audio_bits_per_sample_t) {}
    void setChannels(audio_channels_t) {}
    void setOutputMode(audio_output_mode_t) {}
    void setI2SPins(int, int, int, int=-1) {}
private:
    bool _initialized;
    bool _playing;
    bool _paused;
    bool _muted;
    float _volume;
    audio_sample_rate_t _sampleRate;
    audio_bits_per_sample_t _bitsPerSample;
    audio_channels_t _channels;
    audio_output_mode_t _outputMode;
    int _bclkPin;
    int _lrckPin;
    int _dataOutPin;
    int _dataInPin;
};
class AudioInput {
public:
    AudioInput() {}
    ~AudioInput() {}
    bool begin(audio_sample_rate_t=AUDIO_SAMPLE_RATE_16K, audio_bits_per_sample_t=AUDIO_BITS_PER_SAMPLE_16, audio_channels_t=AUDIO_CHANNELS_MONO) { return false; }
    void end() {}
    size_t read(uint8_t*, size_t) { return 0; }
    size_t readSamples(int16_t*, size_t) { return 0; }
    int16_t readSample() { return 0; }
    bool startRecording() { return false; }
    bool stopRecording() { return false; }
    bool saveToWAV(const char*, const uint8_t*, size_t) { return false; }
    bool saveToMP3(const char*, const uint8_t*, size_t) { return false; }
    void setGain(float) {}
    float getGain() { return 0; }
    bool isRecording() { return false; }
    int available() { return 0; }
    void setSampleRate(audio_sample_rate_t) {}
    void setBitsPerSample(audio_bits_per_sample_t) {}
    void setChannels(audio_channels_t) {}
    void setInputMode(audio_input_mode_t) {}
    void setI2SPins(int, int, int) {}
    void setDMICPins(int, int) {}
private:
    bool _initialized;
    bool _recording;
    float _gain;
    audio_sample_rate_t _sampleRate;
    audio_bits_per_sample_t _bitsPerSample;
    audio_channels_t _channels;
    audio_input_mode_t _inputMode;
    int _bclkPin;
    int _lrckPin;
    int _dataInPin;
};
class AudioCodec {
public:
    AudioCodec() {}
    ~AudioCodec() {}
    bool begin() { return false; }
    void end() {}
    void setVolume(float) {}
    float getVolume() { return 0; }
    void setHeadphoneVolume(float) {}
    float getHeadphoneVolume() { return 0; }
    void setSpeakerVolume(float) {}
    float getSpeakerVolume() { return 0; }
    void setMicGain(float) {}
    float getMicGain() { return 0; }
    void setLineInGain(float) {}
    float getLineInGain() { return 0; }
    void setMute(bool) {}
    void setHeadphoneMute(bool) {}
    void setSpeakerMute(bool) {}
    void setMicMute(bool) {}
    void setSampleRate(audio_sample_rate_t) {}
    void setBitsPerSample(audio_bits_per_sample_t) {}
    void setI2SFormat(int) {}
    void enableMicInput(bool) {}
    void enableLineInput(bool) {}
    void enableHeadphoneOutput(bool) {}
    void enableSpeakerOutput(bool) {}
private:
    bool _initialized;
    float _volume;
    float _headphoneVolume;
    float _speakerVolume;
};
extern AudioOutput audioOut;
extern AudioInput audioIn;
extern AudioCodec codec;
#endif

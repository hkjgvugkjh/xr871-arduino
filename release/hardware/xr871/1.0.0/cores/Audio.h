/**
 * @file Audio.h
 * @brief ESP32 Compatible Audio for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides I2S/DMIC audio input/output for XR871.
 * Supports playback and recording.
 */

#ifndef _AUDIO_XR871_H_
#define _AUDIO_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Audio sample rates
typedef enum {
    AUDIO_SAMPLE_RATE_8K = 8000,
    AUDIO_SAMPLE_RATE_11K = 11025,
    AUDIO_SAMPLE_RATE_16K = 16000,
    AUDIO_SAMPLE_RATE_22K = 22050,
    AUDIO_SAMPLE_RATE_32K = 32000,
    AUDIO_SAMPLE_RATE_44K = 44100,
    AUDIO_SAMPLE_RATE_48K = 48000
} audio_sample_rate_t;

// Audio bit depth
typedef enum {
    AUDIO_BITS_PER_SAMPLE_8 = 8,
    AUDIO_BITS_PER_SAMPLE_16 = 16,
    AUDIO_BITS_PER_SAMPLE_24 = 24,
    AUDIO_BITS_PER_SAMPLE_32 = 32
} audio_bits_per_sample_t;

// Audio channels
typedef enum {
    AUDIO_CHANNELS_MONO = 1,
    AUDIO_CHANNELS_STEREO = 2
} audio_channels_t;

// Audio output mode
typedef enum {
    OUTPUT_MODE_I2S = 0,
    OUTPUT_MODE_DAC,
    OUTPUT_MODE_PWM
} audio_output_mode_t;

// Audio input mode
typedef enum {
    INPUT_MODE_I2S = 0,
    INPUT_MODE_DMIC,
    INPUT_MODE_ADC,
    INPUT_MODE_PDM
} audio_input_mode_t;

// Audio format
typedef enum {
    AUDIO_FORMAT_PCM = 0,
    AUDIO_FORMAT_MP3,
    AUDIO_FORMAT_WAV,
    AUDIO_FORMAT_AAC,
    AUDIO_FORMAT_AMR,
    AUDIO_FORMAT_OPUS
} audio_format_t;

// Audio buffer configuration
#define AUDIO_BUFFER_SIZE 4096
#define AUDIO_MAX_BUFFERS 4

// ============================================================
// Audio Output Class
// ============================================================
class AudioOutput {
public:
    AudioOutput();
    ~AudioOutput();
    
    // Initialize output
    bool begin(audio_sample_rate_t sampleRate = AUDIO_SAMPLE_RATE_16K,
               audio_bits_per_sample_t bitsPerSample = AUDIO_BITS_PER_SAMPLE_16,
               audio_channels_t channels = AUDIO_CHANNELS_MONO);
    void end();
    
    // Play data
    size_t write(const uint8_t* data, size_t len);
    size_t write(int16_t sample);
    size_t writeSamples(const int16_t* samples, size_t count);
    
    // Play from file
    bool playFile(const char* filename);
    bool playWAV(const uint8_t* data, size_t len);
    bool playMP3(const uint8_t* data, size_t len);
    bool playAAC(const uint8_t* data, size_t len);
    
    // Streaming
    bool startStream();
    bool stopStream();
    
    // Volume control
    void setVolume(float volume);  // 0.0 to 1.0
    float getVolume();
    void setMute(bool mute);
    bool isMuted();
    
    // Status
    bool isPlaying();
    bool isPaused();
    bool pause();
    bool resume();
    bool stop();
    
    // Configuration
    void setSampleRate(audio_sample_rate_t rate);
    void setBitsPerSample(audio_bits_per_sample_t bits);
    void setChannels(audio_channels_t channels);
    void setOutputMode(audio_output_mode_t mode);
    
    // I2S pin configuration
    void setI2SPins(int bclk, int lrck, int data_out, int data_in = -1);
    
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
    
    // I2S pins
    int _bclkPin;
    int _lrckPin;
    int _dataOutPin;
    int _dataInPin;
    
    bool initI2S();
    bool deinitI2S();
};

// ============================================================
// Audio Input Class
// ============================================================
class AudioInput {
public:
    AudioInput();
    ~AudioInput();
    
    // Initialize input
    bool begin(audio_sample_rate_t sampleRate = AUDIO_SAMPLE_RATE_16K,
               audio_bits_per_sample_t bitsPerSample = AUDIO_BITS_PER_SAMPLE_16,
               audio_channels_t channels = AUDIO_CHANNELS_MONO);
    void end();
    
    // Record data
    size_t read(uint8_t* data, size_t len);
    size_t readSamples(int16_t* samples, size_t count);
    int16_t readSample();
    
    // Start/stop recording
    bool startRecording();
    bool stopRecording();
    
    // Save to file
    bool saveToWAV(const char* filename, const uint8_t* data, size_t len);
    bool saveToMP3(const char* filename, const uint8_t* data, size_t len);
    
    // Volume control
    void setGain(float gain);  // 0.0 to 1.0
    float getGain();
    
    // Status
    bool isRecording();
    int available();
    
    // Configuration
    void setSampleRate(audio_sample_rate_t rate);
    void setBitsPerSample(audio_bits_per_sample_t bits);
    void setChannels(audio_channels_t channels);
    void setInputMode(audio_input_mode_t mode);
    
    // I2S pin configuration
    void setI2SPins(int bclk, int lrck, int data_in);
    void setDMICPins(int clk, int data);
    
private:
    bool _initialized;
    bool _recording;
    float _gain;
    audio_sample_rate_t _sampleRate;
    audio_bits_per_sample_t _bitsPerSample;
    audio_channels_t _channels;
    audio_input_mode_t _inputMode;
    
    // I2S pins
    int _bclkPin;
    int _lrckPin;
    int _dataInPin;
    
    bool initI2S();
    bool deinitI2S();
};

// ============================================================
// Audio Codec Class (for AC101, ES8388, etc.)
// ============================================================
class AudioCodec {
public:
    AudioCodec();
    ~AudioCodec();
    
    bool begin();
    void end();
    
    // Volume control
    void setVolume(float volume);
    float getVolume();
    void setHeadphoneVolume(float volume);
    float getHeadphoneVolume();
    void setSpeakerVolume(float volume);
    float getSpeakerVolume();
    
    // Input control
    void setMicGain(float gain);
    float getMicGain();
    void setLineInGain(float gain);
    float getLineInGain();
    
    // Mute control
    void setMute(bool mute);
    void setHeadphoneMute(bool mute);
    void setSpeakerMute(bool mute);
    void setMicMute(bool mute);
    
    // Format control
    void setSampleRate(audio_sample_rate_t rate);
    void setBitsPerSample(audio_bits_per_sample_t bits);
    void setI2SFormat(int format);
    
    // Path control
    void enableMicInput(bool enable);
    void enableLineInput(bool enable);
    void enableHeadphoneOutput(bool enable);
    void enableSpeakerOutput(bool enable);
    
private:
    bool _initialized;
    float _volume;
    float _headphoneVolume;
    float _speakerVolume;
};

// Global instances
extern AudioOutput audioOut;
extern AudioInput audioIn;
extern AudioCodec codec;

#ifdef __cplusplus
}
#endif

#endif

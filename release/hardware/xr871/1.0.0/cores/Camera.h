/**
 * @file Camera.h
 * @brief ESP32 Compatible Camera for XR871
 * @author Hermes Agent
 * @date 2026-08-27
 *
 * Provides camera interface for XR871 CSI camera sensor.
 * Supports OV2640, OV7670, and other common camera modules.
 * ESP32 API compatible.
 */

#ifndef _CAMERA_XR871_H_
#define _CAMERA_XR871_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Camera frame sizes
typedef enum {
    FRAMESIZE_QVGA = 0,    // 320x240
    FRAMESIZE_CIF,         // 352x288
    FRAMESIZE_VGA,         // 640x480
    FRAMESIZE_SVGA,        // 800x600
    FRAMESIZE_XGA,         // 1024x768
    FRAMESIZE_SXGA,        // 1280x1024
    FRAMESIZE_UXGA,        // 1600x1200
    FRAMESIZE_QXGA,        // 2048x1536
    FRAMESIZE_5MP,         // 2592x1944
    FRAMESIZE_MAX
} framesize_t;

// Camera pixel formats
typedef enum {
    PIXFORMAT_RGB565 = 0,
    PIXFORMAT_YUV422,
    PIXFORMAT_GRAYSCALE,
    PIXFORMAT_JPEG,
    PIXFORMAT_RAW,
    PIXFORMAT_RGB888
} pixformat_t;

// Camera config structure
typedef struct {
    framesize_t frame_size;
    pixformat_t pixel_format;
    int jpeg_quality;
    int fb_count;
    int brightness;
    int contrast;
    int saturation;
    int sharpness;
    int ae_level;
    int gainceiling;
    int bpc;
    int wpc;
    int lenc;
    int hmirror;
    int vflip;
    int colorbar;
} camera_config_t;

// Camera sensor PID
typedef enum {
    OV2640_PID = 0x2640,
    OV7670_PID = 0x7670,
    OV7725_PID = 0x7725,
    NT99141_PID = 0x1410,
    GC0308_PID = 0xc308,
    GC032A_PID = 0xc32a,
    BF3005_PID = 0xf3005,
    BF20A6_PID = 0x20a6,
} camera_pid_t;

// ============================================================
// Camera Class
// ============================================================
class Camera {
public:
    Camera();
    ~Camera();
    
    // Initialize camera
    bool begin(camera_config_t config);
    bool begin(framesize_t framesize = FRAMESIZE_VGA, pixformat_t format = PIXFORMAT_JPEG);
    bool reset();
    void end();
    
    // Capture
    uint8_t* capture();
    uint8_t* capture(int timeout);
    bool release();
    
    // Get frame buffer
    uint8_t* getFB();
    int getWidth();
    int getHeight();
    size_t getSize();
    pixformat_t getFormat();
    
    // Set parameters
    void setFrameSize(framesize_t size);
    void setPixelFormat(pixformat_t format);
    void setJpegQuality(int quality);  // 0-63, lower = better quality
    void setBrightness(int level);     // -2 to 2
    void setContrast(int level);       // -2 to 2
    void setSaturation(int level);     // -2 to 2
    void setSharpness(int level);      // -2 to 2
    void setGainceiling(int ceiling);
    void setHMirror(bool hmirror);
    void setVFlip(bool vflip);
    void setExposure(int exposure);
    void setGain(int gain);
    void setWhiteBalance(bool enable);
    void setAwbGain(bool enable);
    void setWbMode(int mode);
    void setSpecialEffect(int effect);
    void setWPC(bool enable);
    void setBPC(bool enable);
    void setLENC(bool enable);
    void setAELevel(int level);
    void setDenoise(int level);
    
    // Status
    bool isInitialized();
    bool isCapturing();
    camera_pid_t getPID();
    
    // Streaming
    bool startStream();
    bool stopStream();
    bool isStreaming();
    
private:
    bool _initialized;
    bool _capturing;
    bool _streaming;
    camera_config_t _config;
    uint8_t* _frameBuffer;
    size_t _frameSize;
    int _width;
    int _height;
    
    bool allocateFrameBuffer();
    void freeFrameBuffer();
    void updateResolution();
    bool initCSI();
};

// ============================================================
// Sensor class for direct register access
// ============================================================
class Sensor {
public:
    Sensor();
    
    void init();
    void reset();
    void setPixformat(pixformat_t format);
    void setFramesize(framesize_t size);
    void setFramerate(int framerate);
    void setBrightness(int level);
    void setContrast(int level);
    void setSaturation(int level);
    void setSharpness(int level);
    void setGainceiling(int ceiling);
    void setQuality(int quality);
    void setColorbar(int enable);
    void setWhitebal(int enable);
    void setGainctrl(int enable);
    void setAec(int enable);
    void setHmirror(int enable);
    void setVflip(int enable);
    void setBpc(int enable);
    void setWpc(int enable);
    void setRawGma(int enable);
    void setLenc(int enable);
    void setAec2(int enable);
    void setAwb(int enable);
    void setExp(int exposure);
    
    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t value);
    
    int getMID();
    int getPID();
    
    void initOV2640();
    void initOV7670();
    void initOV7725();
};

#ifdef __cplusplus
}
#endif

#endif

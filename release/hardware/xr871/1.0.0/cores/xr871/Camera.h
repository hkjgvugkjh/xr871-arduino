// Minimal Camera stub
#ifndef _CAMERA_XR871_H_
#define _CAMERA_XR871_H_
#include <stdint.h>
typedef enum { FRAMESIZE_QVGA=0, FRAMESIZE_CIF, FRAMESIZE_VGA, FRAMESIZE_SVGA, FRAMESIZE_XGA, FRAMESIZE_SXGA, FRAMESIZE_UXGA, FRAMESIZE_QXGA, FRAMESIZE_5MP, FRAMESIZE_MAX } framesize_t;
typedef enum { PIXFORMAT_RGB565=0, PIXFORMAT_YUV422, PIXFORMAT_GRAYSCALE, PIXFORMAT_JPEG, PIXFORMAT_RAW, PIXFORMAT_RGB888 } pixformat_t;
typedef struct { framesize_t frame_size; pixformat_t pixel_format; int jpeg_quality; int fb_count; int brightness; int contrast; int saturation; int sharpness; int ae_level; int gainceiling; int bpc; int wpc; int lenc; int hmirror; int vflip; int colorbar; } camera_config_t;
class Camera {
public:
    Camera() {}
    ~Camera() {}
    bool begin(camera_config_t) { return false; }
    bool begin(framesize_t=FRAMESIZE_VGA, pixformat_t=PIXFORMAT_JPEG) { return false; }
    bool reset() { return false; }
    void end() {}
    uint8_t* capture() { return NULL; }
    uint8_t* capture(int) { return NULL; }
    bool release() { return false; }
    uint8_t* getFB() { return NULL; }
    int getWidth() { return 0; }
    int getHeight() { return 0; }
    size_t getSize() { return 0; }
    pixformat_t getFormat() { return PIXFORMAT_JPEG; }
    void setFrameSize(framesize_t) {}
    void setPixelFormat(pixformat_t) {}
    void setJpegQuality(int) {}
    void setBrightness(int) {}
    void setContrast(int) {}
    void setSaturation(int) {}
    void setSharpness(int) {}
    void setGainceiling(int) {}
    void setHMirror(bool) {}
    void setVFlip(bool) {}
    void setExposure(int) {}
    void setGain(int) {}
    void setWhiteBalance(bool) {}
    void setAwbGain(bool) {}
    void setWbMode(int) {}
    void setSpecialEffect(int) {}
    void setWPC(bool) {}
    void setBPC(bool) {}
    void setLENC(bool) {}
    void setAELevel(int) {}
    void setDenoise(int) {}
    bool isInitialized() { return false; }
    bool isCapturing() { return false; }
    int getPID() { return 0; }
    bool startStream() { return false; }
    bool stopStream() { return false; }
    bool isStreaming() { return false; }
};
#endif

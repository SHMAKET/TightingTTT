#include <Adafruit_NeoPixel.h>

enum EasingType {
    LINEAR,     // по умолчанию
    EASE_IN,    // медленно начинается, потом ускоряется
    EASE_OUT,   // быстро начинается, потом замедляется
    EASE_IN_OUT // медленно → быстро → медленно
};

class SmoothStrip {
private:
    Adafruit_NeoPixel &strip;
    uint8_t maxBrightness;
    uint8_t currentBrightness;
    uint32_t baseColor;         // исходный цвет

    bool fading;
    bool fadeDirection;         // true = ON, false = OFF
    unsigned long fadeStart;
    unsigned long fadeDuration;
    EasingType easing;

    float ease(float t) {
        switch (easing) {
            case EASE_IN:      return t * t;
            case EASE_OUT:     return t * (2 - t);
            case EASE_IN_OUT:  return (t < 0.5) ? 2*t*t : -1+(4-2*t)*t;
            case LINEAR:
            default:           return t;
        }
    }

    uint32_t scaleColor(uint32_t color, uint8_t brightness) {
        uint8_t r = ((color >> 16) & 0xFF) * brightness / 255;
        uint8_t g = ((color >> 8)  & 0xFF) * brightness / 255;
        uint8_t b = ( color        & 0xFF) * brightness / 255;
        return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    void applyScaledColor(uint8_t brightness) {
        uint32_t scaled = scaleColor(baseColor, brightness);
        for (int i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, scaled);
        }
        strip.show();
    }

public:
    SmoothStrip(Adafruit_NeoPixel &s, uint8_t maxB = 150)
        : strip(s), maxBrightness(maxB), currentBrightness(0), baseColor(0),
            fading(false), fadeDirection(true),
            fadeStart(0), fadeDuration(0), easing(LINEAR) {}

    void begin() {
        strip.begin();
        strip.show();
    }

    void ON(uint32_t color, unsigned long duration, EasingType e = LINEAR) {
        // если уже включена и не анимируется → выходим
        if ((!fading && currentBrightness == maxBrightness) || (fading && fadeDirection)) 
            return;

        baseColor = color;
        currentBrightness = 0;
        applyScaledColor(0); // загрузим чёрный

        fadeDirection = true;
        fadeDuration = duration;
        fadeStart = millis();
        fading = true;
        easing = e;
    }

    void OFF(unsigned long duration, EasingType e = LINEAR) {
         // если уже выключена и не анимируется → выходим
        if ((!fading && currentBrightness == 0) || (fading && !fadeDirection)) 
            return;

        fadeDirection = false;
        fadeDuration = duration;
        fadeStart = millis();
        fading = true;
        easing = e;
    }

    void update() {
        if (!fading) return;

        unsigned long now = millis();
        unsigned long elapsed = now - fadeStart;

        if (elapsed >= fadeDuration) {
            currentBrightness = fadeDirection ? maxBrightness : 0;
            applyScaledColor(currentBrightness);
            fading = false;
            return;
        }

        float progress = (float)elapsed / fadeDuration;
        float eased = ease(progress);

        if (fadeDirection) {
            currentBrightness = (uint8_t)(eased * maxBrightness);
        } else {
            currentBrightness = (uint8_t)((1.0 - eased) * maxBrightness);
        }

        applyScaledColor(currentBrightness);
    }

    bool isFading() const {
        return fading;
    }
};

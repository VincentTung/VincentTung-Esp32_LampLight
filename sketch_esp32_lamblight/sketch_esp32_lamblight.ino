
// https://dl.espressif.com/dl/package_esp32_index.json
// https://espressif.github.io/arduino-esp32/package_esp32_index.json

#include <FastLED.h>

FASTLED_USING_NAMESPACE


#define BUTTON_PIN 3

#define DATA_PIN 2

//#define CLK_PIN 4

#define LED_TYPE WS2812B

#define COLOR_ORDER GRB

#define NUM_LEDS 64

CRGB leds[NUM_LEDS];



#define BRIGHTNESS 96

#define FRAMES_PER_SECOND 120



void setup() {

  pinMode(BUTTON_PIN, INPUT);

  delay(3000);  // 3秒的恢复延迟



  // 告诉FastLED关于LED条带配置

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  //FastLED.addLeds(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);



  // 设置主亮度控制

  FastLED.setBrightness(BRIGHTNESS);
}



// 列出要循环的模式。每个都定义为下面的单独函数。

typedef void (*SimplePatternList[])();

SimplePatternList gPatterns = { confetti, off };



uint8_t gCurrentPatternNumber = 0;  // 当前模式的索引号

uint8_t gHue = 0;  // 由许多模式使用的旋转"基色"



void loop()

{

  // 调用当前模式函数一次，更新'leds'数组

  gPatterns[gCurrentPatternNumber]();



  // 将'leds'数组发送到实际的LED条

  FastLED.show();

  // 插入一个延迟以保持帧率适中

  FastLED.delay(1000 / FRAMES_PER_SECOND);



  // 做一些定期更新

  EVERY_N_MILLISECONDS(20) {
    gHue++;
  }  // 慢慢地通过彩虹循环"基色"

  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // 定期更改模式



  int buttonState;

  buttonState = digitalRead(3);

  if (buttonState != HIGH) {

    nextPattern();

    delay(500);
  }
}



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))



void nextPattern()

{

  // 在当前模式号上加一，并在结束时回绕

  gCurrentPatternNumber = (gCurrentPatternNumber + 1 % ARRAY_SIZE(gPatterns));
}

void confetti()

{

  // 随机颜色的小点，它们会闪烁并平滑地淡出

  fadeToBlackBy(leds, NUM_LEDS, 10);

  int pos = random16(NUM_LEDS);

  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}



void off()

{

  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
}
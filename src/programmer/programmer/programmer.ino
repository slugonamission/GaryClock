#include <SPI.h>
#include <SD.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// !!! IMPORTANT !!!
// IF THIS IS DEFINED, ADD ~5kB ONTO THE PROGRAM SIZE!
//#define SKIP_SHIT

#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST NULL
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_CLK 13
#define TFT_BACKLIGHT 9

#define SD_CS 4

// Do we have an SD card or not?
int sd_card = 0;

// Startup sequence
char* stringTab[] = {
  "Initialising ToucHMore",
  "Opening SD...",
  "Finding graphic",
  "Touch More!",
  NULL
};

#define SDLINE 1
#define GRAPHICLINE 3

// Menu stuff
char* menuTab[] = {
  "Set Time",
  "Set LEDs",
  "Set Brightness",
  "Flappy Plumbridge",
  "Credits",
  NULL
};

int selectedItem = 0;

// Page config
int curPage = 0;
#define PAGE_MENU 0
#define PAGE_TIME 1
#define PAGE_LEDS 255
#define PAGE_BRIGHTNESS 3
#define PAGE_FLAPPY 255
#define PAGE_CREDITS 255
#define PAGE_UNDEFINED 255

// Joystick
char keydown = 0;
#define JOY_NONE 0
#define JOY_UP 1
#define JOY_LEFT 2
#define JOY_RIGHT 3
#define JOY_DOWN 4
#define JOY_PRESS 5

// Forward method decls.
void writeOK();
void writeFail();
void drawPBM(char* filename, const int width, const int height, const int xOff, const int yOff);

// Menu page
void printMenuHeader();
void printMenu();
void loopMenu();

// ----------------------------------------
// Time page
#define TIMESEG_HOUR 0
#define TIMESEG_MIN 1
#define TIMESEG_SEC 2
#define TIMESEG_BACK 3

void printTimeHeader();
void printTime();
void loopTime();
unsigned char timeSelectedSegment = TIMESEG_HOUR;
unsigned char timeCurrent[3] = {0, 0, 0};

// -----------------------------------------
// Brightness page
void printBrightnessHeader();
void loopBrightness();
unsigned char curBrightness = 255;

// -----------------------------------------
// Undefined page
void printUndefinedHeader();
void loopUndefined();

// Joystick helper
char getJoyDir();

// The actual screen...
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

// "BUT JAMIE!", I hear you say, "PBM ENCODES THE WIDTH AND HEIGHT YOU NOOB", you go on.
// Well, I can't be fucked. Reading those requires proper string reading routines, proper
// buffers, and conversion routines. We're running out of space already (so much so I'm tempted
// to remove the filetype check), so deal with it.
void drawPBM(char* filename, const int width, const int height, const int xOff, const int yOff)
{
  File f = SD.open(filename, FILE_READ);
  int headerLines = 4;
  int curX = 0;
  int curY = 0;
  int maxPix = width * height;
  
  char buf[3];
  
  if(!f)
    tft.println("Couldn't open file");
  
  // Read the header
  buf[0] = f.read();
  buf[1] = f.read();
  
  if(buf[0] != 'P' || buf[1] != '6')
  {
    tft.println("Unknown file");
    return;
  }
  
  // Read the rest of the header
  while(headerLines)
  {
    if(f.read() == '\n')
      headerLines--;
  }
  
   for(curY = yOff; curY < yOff + height; curY++)
   {
     for(curX = xOff; curX < xOff + width; curX++)
     {
       buf[0] = f.read(); // R
       buf[1] = f.read(); // G
       buf[2] = f.read(); // B
       tft.drawPixel(curX, curY, tft.Color565(buf[0], buf[1], buf[2]));
     }
   }
   
   f.close();
}

void printMenuHeader()
{
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(30, 0);
  tft.println("ToucHMore Remote");
  tft.drawFastHLine(0, 10, 160, ST7735_YELLOW);
  tft.setCursor(0, 15);
}

void printMenu()
{
  int menuIndex = 0;
  tft.setCursor(0, 15);
  
  while(menuTab[menuIndex] != 0)
  {
    if(menuIndex == selectedItem)
      tft.setTextColor(ST7735_BLACK, ST7735_YELLOW);
    else
      tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.println(menuTab[menuIndex++]);
  }
}

void selectItem(int item)
{
  selectedItem = item;
  printMenu();
}

void writeOK()
{
  tft.write('[');
  tft.setTextColor(ST7735_GREEN);
  tft.print("OK");
  tft.setTextColor(ST7735_WHITE);
  tft.write(']');
}

void writeFail()
{
  tft.write('[');
  tft.setTextColor(ST7735_RED);
  tft.print("FAIL");
  tft.setTextColor(ST7735_WHITE);
  tft.write(']');
}

void setup()
{
  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  analogWrite(TFT_BACKLIGHT, 255);
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_MAGENTA);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
  
  // Init SD card...
  // Check for SD Card
  if (!SD.begin(SD_CS, TFT_MOSI, TFT_MISO, TFT_CLK)) 
  {
    // No SD card, or failed to initialise card
    sd_card=0;
    // Arduino SD library does something to SPI speed when it fails
    // So need to reset otherwise screen is slow.
    SPI.setClockDivider(SPI_CLOCK_DIV4); // 16/4 MHz
  }  
  else sd_card=1;
  
  int curStrIdx = 0;
  
#ifndef SKIP_SHIT
  while(stringTab[curStrIdx] != 0)
  {
    tft.print(stringTab[curStrIdx]);
    
    if(curStrIdx == SDLINE)
    {
      if(sd_card)
      {
      }
      else
      {
        writeFail();
        tft.println();
        tft.print("!!! HALT !!!");
        while(1);
      }
    }
    
    writeOK();
    
    if(curStrIdx == GRAPHICLINE)
    {
      drawPBM("touch.pbm", 80, 40, 40, 68);
    }
    
    delay(2000);
    tft.println();
    curStrIdx++;
  }
  
  delay(10000);
#endif
  printMenuHeader();
  printMenu();
}

char getJoyDir()
{
  int x;
  
  x = analogRead (0);               // Read Jog switch input
  
  if ((x < 100) & (keydown==0)) {
    // Select
    keydown=1;
    return JOY_PRESS;
  }
  else if ((x < 200) & (keydown==0)) {
    // Up
    // move up one menu item, if at top wrap to bottom
    keydown=1;
    return JOY_UP;
  }
  else if ((x < 400) & (keydown==0)){
    // Left
    keydown=1;
    return JOY_LEFT;    
  }
  else if ((x < 600) & (keydown==0)){
    // Down
    // move down one menu item, if at bottom wrap to top
    keydown=1;
    return JOY_DOWN;
  }
  else if ((x < 800) & (keydown==0)){
    // Right
    keydown=1;    
    return JOY_RIGHT;
  }  
  else if (x >= 800)
  {
    keydown=0; // key released
  }
  
  return JOY_NONE;
}

void loop()
{
  if(curPage == PAGE_MENU)
    loopMenu();
  if(curPage == PAGE_TIME)
    loopTime();
  if(curPage == PAGE_BRIGHTNESS)
    loopBrightness();
  if(curPage == PAGE_UNDEFINED)
    loopUndefined();
}

void loopMenu()
{
  char dir = getJoyDir();
  if(dir == JOY_DOWN)
    selectItem(selectedItem + 1);
  else if(dir == JOY_UP)
    selectItem(selectedItem - 1);
  else if(dir == JOY_PRESS)
  {
    switch(selectedItem)
    {
      case 0:
        printTimeHeader();
        curPage = PAGE_TIME;
        break;
        
      case 2:
       printBrightnessHeader();
       curPage = PAGE_BRIGHTNESS;
       break;
       
     default:
       printUndefinedHeader();
       curPage = PAGE_UNDEFINED;
       break;
    }
  }
}

// -------------------------------------------------

void printTimeHeader()
{
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(50, 0);
  tft.println("Time Config");
  tft.drawFastHLine(0, 10, 160, ST7735_YELLOW);
  tft.setCursor(0, 15);
  
  printTime();
}

void printTime()
{
  tft.setCursor(55, 30);
  
  // Hours...
  if(timeSelectedSegment == TIMESEG_HOUR)
    tft.setTextColor(ST7735_BLACK, ST7735_YELLOW);
  if(timeCurrent[0] < 10)
    tft.write('0');
  tft.print(timeCurrent[0]);
  
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.write(':');
  
  // Minutes...
  if(timeSelectedSegment == TIMESEG_MIN)
    tft.setTextColor(ST7735_BLACK, ST7735_YELLOW);
  if(timeCurrent[1] < 10)
    tft.write('0');
  tft.print(timeCurrent[1]);
  
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.write(':');
  
  // Seconds...
  if(timeSelectedSegment == TIMESEG_SEC)
    tft.setTextColor(ST7735_BLACK, ST7735_YELLOW);
  if(timeCurrent[2] < 10)
    tft.write('0');
  tft.print(timeCurrent[2]);
  
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  
  tft.setCursor(55, 60);
  
  if(timeSelectedSegment == TIMESEG_BACK)
    tft.setTextColor(ST7735_BLACK, ST7735_YELLOW);
  tft.print("<<< Back");
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
}

void loopTime()
{
  char dir = getJoyDir();
  
  if(dir == JOY_LEFT)
  {
    timeSelectedSegment--;
    printTime();
  }
  
  if(dir == JOY_RIGHT)
  {
    timeSelectedSegment++;
    printTime();
  }
  
  if(dir == JOY_UP && timeSelectedSegment <= TIMESEG_SEC)
  {
    if(timeSelectedSegment == TIMESEG_HOUR && timeCurrent[timeSelectedSegment] == 11)
      timeCurrent[timeSelectedSegment] = 0;
    else if(timeCurrent[timeSelectedSegment] == 59)
      timeCurrent[timeSelectedSegment] = 0;
    else
      timeCurrent[timeSelectedSegment]++;
    printTime();
  }
  
  if(dir == JOY_DOWN && timeSelectedSegment <= TIMESEG_SEC)
  {
    if(timeCurrent[timeSelectedSegment] == 0)
    {
      if(timeSelectedSegment == TIMESEG_HOUR)
        timeCurrent[timeSelectedSegment] = 11;
      else
        timeCurrent[timeSelectedSegment] = 59;
    }
    else
      timeCurrent[timeSelectedSegment]--;
    printTime();
  }
  
  if(dir == JOY_PRESS && timeSelectedSegment == TIMESEG_BACK)
  {
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
}

// -------------------------------------------------

void printBrightnessHeader()
{
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(50, 0);
  tft.println("Brightness");
  tft.drawFastHLine(0, 10, 160, ST7735_YELLOW);
  tft.setCursor(0, 15);
  tft.println(curBrightness);
}

void loopBrightness()
{
  char dir = getJoyDir();
  if(dir == JOY_UP)
  {
    curBrightness += 10;
    analogWrite(TFT_BACKLIGHT, curBrightness);
    tft.setCursor(0, 15);
    tft.println(curBrightness);
  }
  else if(dir == JOY_DOWN)
  {
    curBrightness -= 10;
    analogWrite(TFT_BACKLIGHT, curBrightness);
    tft.setCursor(0, 15);
    tft.println(curBrightness);
  }
  else if(dir == JOY_LEFT)
  {
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
}

// -------------------------------------------------------
void printUndefinedHeader()
{
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(50, 0);
  tft.println("UNDEFINED");
  tft.drawFastHLine(0, 10, 160, ST7735_RED);
  tft.setCursor(0, 50);
  tft.println("This page does not exist yet. Please press left to return.");
}

void loopUndefined()
{
  char dir = getJoyDir();
  if(dir == JOY_LEFT)
  {
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
}

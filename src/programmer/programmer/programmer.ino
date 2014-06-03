#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// !!! IMPORTANT !!!
// IF THIS IS DEFINED, ADD ~5kB ONTO THE PROGRAM SIZE!
//#define SKIP_SHIT

// TFT pins
#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST NULL
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_CLK 13
#define TFT_BACKLIGHT 9

// TFT default colours
#define TEXTCOL ST7735_WHITE     // Standard text colour
#define TEXTBG  ST7735_BLACK     // Standard text BG
#define SELTEXTCOL ST7735_BLACK  // Selected text colour
#define SELTEXTBG  ST7735_YELLOW // Selected text background

// SD special pins
#define SD_CS 4

// I2C address in 7-bit format
#define I2C_ADDR 0x28

// I2C vars
int i2c_regptr = 0;

// Because Adafruit don't actually say the font dimensions, and instead
// just scatter magic bloody numbers everywhere...
#define FONT_WIDTH 6
#define FONT_HEIGHT 8

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
  "Quiz",
  NULL
};

int selectedItem = 0;

// Page config
int curPage = 0;
#define PAGE_MENU 0
#define PAGE_TIME 1
#define PAGE_LEDS 255
#define PAGE_BRIGHTNESS 3
#define PAGE_FLAPPY 4
#define PAGE_CREDITS 5
#define PAGE_QUIZ 6
#define PAGE_UNDEFINED 255

// Joystick
char keydown = 0;
#define JOY_NONE 0
#define JOY_UP 1
#define JOY_LEFT 2
#define JOY_RIGHT 3
#define JOY_DOWN 4
#define JOY_PRESS 5
char getJoyDir();

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
// Flappy Plumbridge
void printFlappyHeader();
void loopFlappy();

// -----------------------------------------
// Credits page
void printCreditsHeader();
void loopCredits();

// -----------------------------------------
// Quiz page
void printQuizHeader();
void printQuestion();
void loopQuiz();
void selectAnswer(int ans);
void checkAnswer();
int questionNo = 0;
int selectedAnswer = 0;
int correctAnswer = 0;

File questionFile;
bool questionFileOpen = false;

#define QUIZANS_TOP 54

// -----------------------------------------
// Undefined page
void printUndefinedHeader();
void loopUndefined();


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

void writeOK()
{
  tft.write('[');
  tft.setTextColor(ST7735_GREEN);
  tft.print("OK");
  tft.setTextColor(TEXTCOL);
  tft.write(']');
}

void writeFail()
{
  tft.write('[');
  tft.setTextColor(ST7735_RED);
  tft.print("FAIL");
  tft.setTextColor(TEXTCOL);
  tft.write(']');
}

void i2c_recv(int numBytes)
{
  if(numBytes == 0)
    return;
    
  // Get the reg no
  i2c_regptr = Wire.read();
  numBytes--;
  
  switch(i2c_regptr)
  {
    case 0:
    {
      int timePtr = 0;
      while(numBytes && (timePtr != 3))
      {
        timeCurrent[timePtr] = Wire.read();
        numBytes--;
        timePtr++;
      }
    }
  }
  
  // Blackhole
  while(numBytes--)
    Wire.read();
}

void i2c_trans()
{
}

void setup()
{
  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  analogWrite(TFT_BACKLIGHT, 255);
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_MAGENTA);
  tft.fillScreen(TEXTBG);
  tft.setRotation(3);
  
  Wire.begin(I2C_ADDR);
  Wire.onReceive(i2c_recv);
  Wire.onRequest(i2c_trans);
  
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
    tft.setCursor(0, curStrIdx * FONT_HEIGHT);
    tft.print(stringTab[curStrIdx]);

    tft.setCursor(160 - (4*FONT_WIDTH), curStrIdx * FONT_HEIGHT);    
    
    if(curStrIdx == SDLINE)
    {
      if(!sd_card)
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
    curStrIdx++;
  }
  
  for(int i = 255; i >= 0; i--)
  {
    analogWrite(TFT_BACKLIGHT, i);
    delay(6);
  }
  
  for(int i = 0; i < 256; i++)
  {
    analogWrite(TFT_BACKLIGHT, i);
    delay(6);
  }
  
  delay(5000);
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
  if(curPage == PAGE_FLAPPY)
    loopFlappy();
  if(curPage == PAGE_CREDITS)
    loopCredits();
  if(curPage == PAGE_QUIZ)
    loopQuiz();
  if(curPage == PAGE_UNDEFINED)
    loopUndefined();
}

// --------------------------------------------------
void printMenuHeader()
{
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(30, 0);
  tft.println("ToucHMore Remote");
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
  tft.setCursor(0, 15);
}

void selectItem(int item)
{
  // Highlight new one...
  tft.setCursor(0, 15 + (FONT_HEIGHT * item));
  tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  tft.print(menuTab[item]);
  
  // Blank the previous item
  tft.setCursor(0, 15 + (FONT_HEIGHT * selectedItem));
  tft.setTextColor(TEXTCOL, TEXTBG);
  tft.print(menuTab[selectedItem]);
  
  selectedItem = item;
}

void printMenu()
{
  int menuIndex = 0;
  tft.setCursor(0, 15);
  
  tft.setTextColor(TEXTCOL);
  
  while(menuTab[menuIndex] != 0)
  {
    if(menuIndex == selectedItem)
      tft.setTextColor(SELTEXTCOL, SELTEXTBG);
    else
      tft.setTextColor(TEXTCOL);
    tft.println(menuTab[menuIndex++]);
  }
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
       
     case 3:
       printFlappyHeader();
       curPage = PAGE_FLAPPY;
       break;
       
     case 4:
       printCreditsHeader();
       curPage = PAGE_CREDITS;
       break;
       
     case 5:
       printQuizHeader();
       curPage = PAGE_QUIZ;
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
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
  tft.setCursor(0, 15);
  
  printTime();
}

void printTime()
{
  tft.setCursor(55, 30);
  
  // Hours...
  if(timeSelectedSegment == TIMESEG_HOUR)
    tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  if(timeCurrent[0] < 10)
    tft.write('0');
  tft.print(timeCurrent[0]);
  
  tft.setTextColor(TEXTCOL, TEXTBG);
  tft.write(':');
  
  // Minutes...
  if(timeSelectedSegment == TIMESEG_MIN)
    tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  if(timeCurrent[1] < 10)
    tft.write('0');
  tft.print(timeCurrent[1]);
  
  tft.setTextColor(TEXTCOL, TEXTBG);
  tft.write(':');
  
  // Seconds...
  if(timeSelectedSegment == TIMESEG_SEC)
    tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  if(timeCurrent[2] < 10)
    tft.write('0');
  tft.print(timeCurrent[2]);
  
  tft.setTextColor(TEXTCOL, TEXTBG);
  
  tft.setCursor(55, 60);
  
  if(timeSelectedSegment == TIMESEG_BACK)
    tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  tft.print("<<< Back");
  tft.setTextColor(TEXTCOL, TEXTBG);
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
  tft.fillScreen(TEXTBG);
  
  tft.setCursor(50, 0);
  tft.println("Brightness");
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
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
void printFlappyHeader()
{
  tft.fillScreen(TEXTBG);
  
  tft.setCursor(35, 0);
  tft.println("Flappy Plumbridge");
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
  
  tft.fillRect(15, 15, 10, 50, ST7735_GREEN);
  tft.fillRect(15, 85, 10, 43, ST7735_CYAN);
  
  tft.setCursor(15, 65);
  tft.println("NOT IMPLEMENTED YET");
}

void loopFlappy()
{
  char dir = getJoyDir();
  if(dir == JOY_LEFT)
  {
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
}

// -------------------------------------------------------
void printCreditsHeader()
{
  tft.fillScreen(TEXTBG);
  drawPBM("neil.pbm", 160, 107, 0, 10);
}

void loopCredits()
{
  char dir = getJoyDir();
  if(dir == JOY_LEFT)
  {
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
}

// -------------------------------------------------------
void printQuizHeader()
{
  tft.fillScreen(TEXTBG);
  
  tft.setCursor(60, 0);
  tft.println("Quiz!");
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
  printQuestion();
}

void printQuestion()
{
  char c;
  int readA = 0;
  
  tft.setCursor(0, 16);
  
  if(!questionFileOpen)
  {
    questionFile = SD.open("quiz.txt", FILE_READ);
    if(!questionFile)
    {
      tft.println("Could not open quiz.txt");
      return;
    }
    questionFileOpen = true;
  }
 
  while((c = questionFile.read()) != '\n')
    tft.print(c);
    
  tft.setCursor(0, QUIZANS_TOP);
  tft.print("  ");
  
  while(readA != 4)
  {
    c = questionFile.read();
    if(c == '*')
    {
      correctAnswer = readA;
      continue;
    }
    
    if(c == '\n')
    {
      tft.print("\n  ");
      readA++;
      continue;
    }
    
    tft.print(c);
  }
  
  selectAnswer(0);
}

void selectAnswer(int ans)
{
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setCursor(0, QUIZANS_TOP + (FONT_HEIGHT*selectedAnswer));
  tft.print("  ");
  tft.setCursor(0, QUIZANS_TOP + (FONT_HEIGHT*ans));
  tft.print("->");
  tft.setTextColor(ST7735_WHITE);
  
  selectedAnswer = ans;
}

void checkAnswer()
{
  tft.setCursor(40, 120);
  if(selectedAnswer == correctAnswer)
  {
    tft.setTextColor(ST7735_BLACK, ST7735_GREEN);
    tft.print("CORRECT");
  }
  else
  {
    tft.setTextColor(ST7735_BLACK, ST7735_RED);
    tft.print("WRONG");
    tft.setCursor(0, QUIZANS_TOP + (FONT_HEIGHT*correctAnswer));
    tft.print("->");
  }
  tft.setTextColor(ST7735_WHITE);
  
  delay(5000);
  printQuizHeader();
}

void loopQuiz()
{
  char dir = getJoyDir();
  if(dir == JOY_LEFT)
  {
    questionFile.close();
    questionFileOpen = false;
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
  
  if(dir == JOY_UP)
  {
    selectAnswer(selectedAnswer - 1);
  }
  
  if(dir == JOY_DOWN)
  {
    selectAnswer(selectedAnswer + 1);
  }
  
  if(dir == JOY_PRESS)
  {
    checkAnswer();
  }
}

// -------------------------------------------------------
void printUndefinedHeader()
{
  tft.fillScreen(TEXTBG);
  
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
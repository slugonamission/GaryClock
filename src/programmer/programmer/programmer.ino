#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// REMOVE THIS BEFORE GIVING TO GARY
#define SKIP_SHIT

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
#define I2CPTR_TIME 0
#define I2CPTR_LEDS 1
#define I2CPTR_STOPWORLD 2

// I2C vars
uint8_t i2c_regptr = 0;

// Because Adafruit don't actually say the font dimensions, and instead
// just scatter magic bloody numbers everywhere...
#define FONT_WIDTH 6
#define FONT_HEIGHT 8

// Do we have an SD card or not?
int sd_card = 0;

// Startup sequence
char* stringTab[] = {
  "Initialising runtime",
  "Opening SD...",
  "Finding graphic",
  "Reticulating splines",
  "Touch More!",
  NULL
};

#define SDLINE 1
#define GRAPHICLINE 4

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
#define PAGE_LEDS 2
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
void reprintTimeSegment(unsigned char selectedSegment);
void loopTime();
uint8_t timeSelectedSegment = TIMESEG_HOUR;
uint8_t timeCurrent[3] = {0, 0, 0};

// -----------------------------------------
// LED Mode page
void printLEDHeader();
void printLED();
void loopLED();
unsigned char ledMode = 1;
uint16_t ledColours[] = {
  ST7735_BLUE,
  ST7735_CYAN,
  ST7735_MAGENTA,
  ST7735_YELLOW,
  ST7735_RED
};

// -----------------------------------------
// Brightness page
void printBrightnessHeader();
void loopBrightness();
unsigned char curBrightness = 255;

// -----------------------------------------
// Flappy Plumbridge
void printFlappyHeader();
void loopFlappy();
int barHeights[5] = {50, 50, 50, 50, 50};
int firstBarIdx = 0;
int leadingBarSize = 0;
int flapHeight = 0;

// -----------------------------------------
// Credits page
void printCreditsHeader();
void loopCredits();
uint8_t creditsIdx = 0;

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

#define QUIZANS_TOP 70

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
  
  if(numBytes == 0)
    return;
  
  switch(i2c_regptr)
  {
    case I2CPTR_TIME:
      {
        int timePtr = 0;
        while(numBytes && (timePtr != 3))
        {
          timeCurrent[timePtr] = Wire.read();
          numBytes--;
          timePtr++;
        }
      }
      break;
    
    case I2CPTR_LEDS:
      ledMode = Wire.read();
      numBytes--;
      break;
  }
  
  // Blackhole
  while(numBytes--)
    Wire.read();
}

void i2c_trans()
{
  switch(i2c_regptr)
  {
    case I2CPTR_TIME:
      {
        Wire.write(timeCurrent, 3);
      }
      break;
    
    case I2CPTR_LEDS:
      Wire.write(ledMode);
      break;
    case I2CPTR_STOPWORLD:
      if(curPage == PAGE_TIME)
        Wire.write((uint8_t)1);
      else
        Wire.write((uint8_t)0);
      break;
  }
}

void setup()
{
  pinMode(TFT_BACKLIGHT, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  analogWrite(TFT_BACKLIGHT, 255);
  
  tft.initR(INITR_BLACKTAB);
  #ifndef SKIP_SHIT
  tft.fillScreen(ST7735_MAGENTA);
  tft.fillScreen(ST7735_CYAN);
  tft.fillScreen(ST7735_YELLOW);
  tft.fillScreen(ST7735_WHITE);
  tft.fillScreen(ST7735_RED);
  tft.fillScreen(ST7735_GREEN);
  tft.fillScreen(ST7735_BLUE);
  #endif
  tft.fillScreen(TEXTBG);
  tft.setRotation(3);
  
  Wire.begin(I2C_ADDR);
  Wire.onReceive(i2c_recv);
  Wire.onRequest(i2c_trans);
  
  randomSeed(analogRead(1));
  
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
        tft.setCursor(160 - (6*FONT_WIDTH), curStrIdx*FONT_HEIGHT);
        writeFail();
        tft.println();
        tft.print("!!! \"KERNEL\" PANIC !!!");
        while(1)
        {
          tft.invertDisplay(sd_card);
          sd_card = (sd_card + 1) % 2;
          delay(500);
        }
      }
    }
    
    if(curStrIdx != GRAPHICLINE)
      writeOK();
    else
    {
      tft.setCursor(160 - (6*FONT_WIDTH), curStrIdx*FONT_HEIGHT);
      writeFail();
    }
    
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
  
  for(int i = 1; i < 9; i++)
  {
    tft.invertDisplay(i % 2);
    delay(200);
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
  if(curPage == PAGE_LEDS)
    loopLED();
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
       
      case 1:
        printLEDHeader();
        curPage = PAGE_LEDS;
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

void reprintTimeSegment(unsigned char selectedSegment)
{
  if(selectedSegment > 2)
    return;
    
  tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  tft.setTextSize(3);
  
  tft.setCursor(5 + (selectedSegment * 3 * FONT_WIDTH) + (selectedSegment * 3 * 2 * FONT_WIDTH), 30);
  if(timeCurrent[selectedSegment] < 10)
    tft.write('0');
  tft.print(timeCurrent[selectedSegment]);
  
  tft.setTextColor(TEXTCOL);
  tft.setTextSize(1);
}

// TODO: This needs optimising with the larger font...
void printTime()
{
  tft.setCursor(5, 30);
  tft.setTextSize(3);
  tft.setTextColor(TEXTCOL, TEXTBG);
  
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
  tft.setTextSize(1);
  
  if(timeSelectedSegment == TIMESEG_BACK)
    tft.setTextColor(SELTEXTCOL, SELTEXTBG);
  tft.print("<<< Back");
  tft.setTextColor(TEXTCOL);
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
    reprintTimeSegment(timeSelectedSegment);
    //printTime();
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
    reprintTimeSegment(timeSelectedSegment);
    //printTime();
  }
  
  if(dir == JOY_PRESS && timeSelectedSegment == TIMESEG_BACK)
  {
    curPage = PAGE_MENU;
    printMenuHeader();
    printMenu();
  }
}

// -------------------------------------------------

void printLEDHeader()
{
  tft.fillScreen(ST7735_BLACK);
  
  tft.setCursor(50, 0);
  tft.println("LED Config");
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
  tft.setCursor(0, 15);
  
  printLED();
}

void printLED()
{
  tft.setTextSize(12);
  tft.setTextColor(ledColours[ledMode%5], ST7735_BLACK);
  tft.setCursor(50, 15);
  
  tft.print(ledMode);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
}

void loopLED()
{
  char dir = getJoyDir();
  if(dir == JOY_UP)
  {
    ledMode--;
    printLED();
  }
  else if(dir == JOY_DOWN)
  {
    ledMode++;
    printLED();
  }
  else if(dir == JOY_LEFT)
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
  firstBarIdx = 0;
  barHeights[0] = 50; // Mmm...unrolling...
  barHeights[1] = 50;
  barHeights[2] = 50;
  barHeights[3] = 50;
  barHeights[4] = 50;
  leadingBarSize = 10;
  
  flapHeight = 60;
  
  int i = 0;
  int bar = firstBarIdx;
  tft.fillScreen(TEXTBG);
  
  tft.setCursor(35, 0);
  tft.println("Flappy Plumbridge");
  tft.drawFastHLine(0, 10, 160, SELTEXTBG);
  
  // Draw the initial bars...
  for(i = 0; i < 5; i++)
  {
    tft.fillRect(leadingBarSize + (i*30), 15, 10, barHeights[bar], ST7735_GREEN);
    tft.fillRect(leadingBarSize + (i*30), 15 + barHeights[bar] + 30, 10, 145 - (15 + barHeights[bar] + 30), ST7735_CYAN);
    bar = (bar + 1) % 5;
  }
  
  //tft.fillRect(15, 15, 10, 50, ST7735_GREEN);
  //tft.fillRect(15, 85, 10, 43, ST7735_CYAN);
  
  //tft.setCursor(15, 65);
  //tft.println("NOT IMPLEMENTED YET");
}

void loopFlappy()
{
  // Shave 1px off the tail of each bar, and add one to the head of each bar.
  // Shave cycle
  char dir = getJoyDir();
  
  int i = 0;
  int bar = firstBarIdx;
  
  for(i = 0; i < 5; i++)
  {
    tft.drawFastVLine(leadingBarSize + (i*30) + 10, 15, barHeights[bar], ST7735_BLACK);
    tft.drawFastVLine(leadingBarSize + (i*30) + 10, 15 + barHeights[bar] + 30, 145 - (15 + barHeights[bar] + 30), ST7735_BLACK);
    bar = (bar + 1) % 5;
  }
  
  for(i = 0; i < 5; i++)
  {
    if(leadingBarSize > 0 || i != 0)
    {
      tft.drawFastVLine(leadingBarSize + (i*30) - 1, 15, barHeights[bar], ST7735_GREEN);
      tft.drawFastVLine(leadingBarSize + (i*30) - 1, 15 + barHeights[bar] + 30, 145 - (15 + barHeights[bar] + 30), ST7735_CYAN);
    }
    bar = (bar + 1) % 5;
  }  
  
  leadingBarSize--;
  if(leadingBarSize == -11)
  {
    leadingBarSize = 19;
    firstBarIdx = (firstBarIdx + 1) % 5;
    
    // Gen the new bar...
    int nextBar = (firstBarIdx + 4) % 5;
    barHeights[nextBar] = random(5, 80);
  }
  
  // Move the flap
  tft.fillRect(5, flapHeight, 5, 5, ST7735_BLACK);
  
  // Trajectory smoothing? Hah.
  if(dir == JOY_PRESS && flapHeight >= 0)
    flapHeight -= 10;
  else if(flapHeight < 115)
    flapHeight += 2;
  
  tft.fillRect(5, flapHeight, 5, 5, ST7735_RED);
  
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
  // Done in loop()...
  //tft.fillScreen(TEXTBG);
  creditsIdx = 0;
  //drawPBM("neil.pbm", 160, 107, 0, 10);
}

void loopCredits()
{
  char dir = getJoyDir();
  tft.fillScreen(TEXTBG);
  switch(creditsIdx++)
  {
    case 0:
      drawPBM("neil.pbm", 160, 107, 0, 10);
      break;
    case 1:
      drawPBM("cat.pbm", 160, 120, 0, 4);
      break;
    case 2:
      drawPBM("gary.pbm", 160, 107, 0, 10);
      break;
    case 3:
      drawPBM("xilinx.pbm", 160, 47, 0, 40);
      break;
  }  
  
  if(creditsIdx > 1)
    creditsIdx = 0;
  
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

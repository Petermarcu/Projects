/* Welcome to DmxSimple. This library allows you to control DMX stage and
** architectural lighting and visual effects easily from Arduino. DmxSimple
** is compatible with the Tinker.it! DMX shield and all known DIY Arduino
** DMX control circuits.
**
** DmxSimple is available from: http://code.google.com/p/tinkerit/
** Help and support: http://groups.google.com/group/dmxsimple       */

/* To use DmxSimple, you will need the following line. Arduino will
** auto-insert it if you select Sketch > Import Library > DmxSimple. */

#include <DmxSimple.h>

void setup() {
  /* The most common pin for DMX output is pin 3, which DmxSimple
  ** uses by default. If you need to change that, do it here. */
  DmxSimple.usePin(3);

  /* DMX devices typically need to receive a complete set of channels
  ** even if you only need to adjust the first channel. You can
  ** easily change the number of channels sent here. If you don't
  ** do this, DmxSimple will set the maximum channel number to the
  ** highest channel you DmxSimple.write() to. */
  DmxSimple.maxChannel(27);
}

int red[3] = {255,0,0};
int green[3] = {0, 255, 0};
int blue[3] = {0, 0, 255};
int white[3] = {255, 255, 255};
int yellow[3] = {255, 255, 0};
int black[3] = {0, 0, 0};
int randomColor[3] = {255, 255, 255};

int brightness = 100;

int soundersGreen[3] = {60, 255, 0};

int length = 5;

void loop() 
{
  SnowflakeRoutine(length * 4);
  SoundersStarRoutine(length * 2);
  
  SmallStarWithRaysRoutine(length * 2);
  BigStarLittleStarRoutine(length * 4);
  
  PlainStarRoutine(length * 4);
  GrowingPatternRoutine(length * 2);
}


void Template(int t)
{
  if (t == 0)
    return;
    
  clearAll();
  
  for (int z = 0; z < t; z++)
  {    
    brightness = 100;
    
  }
}

void GrowingPatternRoutine(int t)
{
  if (t == 0)
    return;
    
  clearAll();

  brightness = 100;  
  
  for (int z = 0; z < t; z++)
  {
    int lines[3]; 
    SetRandomColor();
    lines[0] = randomColor[0];
    lines[1] = randomColor[1];
    lines[2] = randomColor[2];
    int smallStar[3]; 
    SetRandomColor();
    smallStar[0] = randomColor[0];
    smallStar[1] = randomColor[1];
    smallStar[2] = randomColor[2];
    int largeStar[3]; 
    SetRandomColor();
    largeStar[0] = randomColor[0];
    largeStar[1] = randomColor[1];
    largeStar[2] = randomColor[2];
    
    clearAll();
    setInnerLines(lines);
    delay(1000);
    setLargeAngles(smallStar);
    delay(1000);
    setSmallAngles(largeStar);
    delay(1000);
    setOuterLines(lines);
    delay(1000);
    setSmallPoints(smallStar);
    delay(1000);
    setLargePoints(largeStar);
    delay(3000);
  }
}

void BigStarLittleStarRoutine(int t)
{
  if (t == 0)
    return;
    
  clearAll();

  brightness = 100;
  
  SetRandomColor();
  setSmallStar(randomColor);
  
  for (int z = 0; z < t; z++)
  {
    SetRandomColor();
    setLargeStar(randomColor);
    setLargeAngles(black);
    delay(2000);
    SetRandomColor();
    setSmallStar(randomColor);
    setSmallAngles(black);
    delay(2000);
  }
}

void SmallStarWithRaysRoutine(int t)
{
  if (t == 0)
    return;
    
  clearAll();

  int smallStar[3]; 
  SetRandomColor();
  smallStar[0] = randomColor[0];
  smallStar[1] = randomColor[1];
  smallStar[2] = randomColor[2];

  for (int z = 0; z < t; z++)
  {
    brightness = 100;
    
    setSmallStar(smallStar);
    SetRandomColor();    
    for (int i = 0; i <= 90; i++)
    {
      delay(9);
      brightness = i;
      setOuterLines(randomColor);
    }  
    
    for (int i = 90; i >= 0; i--)
    {
      delay(9);
      brightness = i;
      setOuterLines(randomColor);
    }
  }
}

void PlainStarRoutine(int t)
{
  if (t == 0)
    return;
    
  clearAll();

  int points[3]; 
  SetRandomColor();
  points[0] = randomColor[0];
  points[1] = randomColor[1];
  points[2] = randomColor[2];

  for (int z = 0; z < t; z++)
  {
    brightness = 100;
    setLargeStar(white);
    
    SetRandomColor();
    
    for (int i = 0; i <= 90; i++)
    {
      delay(9);
      brightness = i;
      setSmallPoints(points);
      setLines(randomColor);
    }  
    
    for (int i = 90; i >= 0; i--)
    {
      delay(9);
      brightness = i;
      setSmallPoints(points);
      setLines(randomColor);
    }
    
    delay(100);
  }
}

void SoundersStarRoutine(int t)
{
  if (t == 0)
    return;
    
  clearAll();

  for (int z = 0; z < t; z++)
  {
    brightness = 100;
    setLargeStar(soundersGreen);
    setLines(white);    
    
    for (int i = 0; i <= 90; i++)
    {
      delay(9);
      brightness = i;
      setSmallPoints(blue);
    }  
    
    for (int i = 90; i >= 0; i--)
    {
      delay(9);
      brightness = i;
      setSmallPoints(blue);
    }
  }
}

void SnowflakeRoutine(int t)
{  
  if (t == 0)
    return;
    
  clearAll();
    
  // Loop and return
  for (int z = 0; z < t; z++)
  {
    brightness = 100;
    setLines(white);
    
    for (int i = 0; i <= 90; i++)
    {
      delay(9);
      brightness = i;
      setLargeAngles(blue);
    }  
    
    for (int i = 90; i >= 0; i--)
    {
      delay(9);
      brightness = i;
      setLargeAngles(blue);
    }
    
    delay(100);
  }
}

void clearAll()
{
  for (int i = 1; i <= 24; i++)
  {
    DmxSimple.write(i, 0);
  }
}

void getRandomColor(int* color)
{
  //color = {0,0,0};
}

void setLines(int* color)
{
    setInnerLines(color);
    setOuterLines(color);
}

void setSmallStar(int* color)
{
  setLargeAngles(color);
  setSmallPoints(color);
}

void setLargeStar(int* color)
{
  setSmallAngles(color);
  setLargePoints(color);
}

void setLargeAngles(int* color)
{  
    DmxSimple.write(13, (color[0] * brightness)/100);
    DmxSimple.write(14, (color[1] * brightness)/100);
    DmxSimple.write(15, (color[2] * brightness)/100);
}

void setSmallAngles(int* color)
{  
    DmxSimple.write(16, (color[0] * brightness)/100);
    DmxSimple.write(17, (color[1] * brightness)/100);
    DmxSimple.write(18, (color[2] * brightness)/100);
}

void setInnerLines(int* color)
{  
    DmxSimple.write(19, (color[0] * brightness)/100);
    DmxSimple.write(20, (color[1] * brightness)/100);
    DmxSimple.write(21, (color[2] * brightness)/100);
}

void setOuterLines(int* color)
{  
    DmxSimple.write(22, (color[0] * brightness)/100);
    DmxSimple.write(23, (color[1] * brightness)/100);
    DmxSimple.write(24, (color[2] * brightness)/100);
}

void setSmallPoints(int* color)
{  
    DmxSimple.write(10, (color[0] * brightness)/100);
    DmxSimple.write(11, (color[1] * brightness)/100);
    DmxSimple.write(12, (color[2] * brightness)/100);
}

void setLargePoints(int* color)
{
    DmxSimple.write(1, (color[0] * brightness)/100);
    DmxSimple.write(2, (color[1] * brightness)/100);
    DmxSimple.write(3, (color[2] * brightness)/100);
    
    DmxSimple.write(4, (color[0] * brightness)/100);
    DmxSimple.write(5, (color[1] * brightness)/100);
    DmxSimple.write(6, (color[2] * brightness)/100);
    
    DmxSimple.write(7, (color[0] * brightness)/100);
    DmxSimple.write(8, (color[1] * brightness)/100);
    DmxSimple.write(9, (color[2] * brightness)/100);
}

void SetRandomColor()
{
  randomColor[0] = random(0,255);
  randomColor[1] = random(0,255);
  randomColor[2] = random(0,255);
}

#include "a_lcd.h"

//波形图
void Gui_ShowWave(float *InputWaveData, uint16_t c_len, float begin_x, float begin_y, float end_x, float end_y)
{
  /* 坐标系矩形区域域起点(c_x0,c_y0) 终点(c_x1,c_y1) */
  uint16_t c_x0 = (uint16_t)(1.0 * LCD_X_SIZE * begin_x);
  uint16_t c_y0 = (uint16_t)(1.0 * LCD_Y_SIZE * begin_y);
  uint16_t c_x1 = (uint16_t)(1.0 * LCD_X_SIZE * end_x);
  uint16_t c_y1 = (uint16_t)(1.0 * LCD_Y_SIZE * end_y);
  /* 坐标系标度密度*/
  uint16_t ScaleNum = 5;
  // uint16_t perW = (c_x1 - c_x0) / (ScaleNum - 1);
  uint16_t perH = (c_y1 - c_y0) / (ScaleNum - 1);

  /* 颜色 */
  uint16_t BackgroundColor = BLACK;
  uint16_t WaveColor = WHITE;
  uint16_t WordColor = WHITE;

  uint16_t c_AxisColor = BLUE;

  /*数据的最值和刻度*/
  uint32_t max, min;
  float Scale;

  /* 保存上一次的数据，用于清除源波形 */
  static uint16_t StaticWaveData[1000][2];
  static uint16_t StaticWaveLen;

  uint32_t i, j, k;
  static uint8_t Flag;

  uint8_t text[64];

  uint32_t *WaveData=(uint32_t *)malloc(c_len*sizeof(uint32_t));
  for(i=0;i<c_len;i++)
  {
    WaveData[i]=(uint32_t)(InputWaveData[i]*1000);
  }

  if (!Flag)
  {
    Lcd_Clear(BackgroundColor);

    //   TFT_printf(WordColor, 0, c_x0-16, 0, "V");
    //   TFT_printf(WordColor, 0, h_x0-16, h_y0, "mV");

    //   TFT_printf(WordColor, 0, (uint16_t)(1.0*LCD_X_SIZE*0.05-8), (uint16_t)(1.0*LCD_Y_SIZE*0.1), "THD:");
    //   snprintf(text, 64, "THD:");
    //   Gui_DrawFont_GBK16((uint16_t)(1.0*LCD_X_SIZE*0.05-8), (uint16_t)(1.0*LCD_Y_SIZE*0.1), WordColor, BackgroundColor, text);
  }

  //   TFT_printf(WordColor, 1, (uint16_t)(1.0*LCD_X_SIZE*0.05-8), (uint16_t)(1.0*LCD_Y_SIZE*0.2), "%.3f",THD);
  max = min = WaveData[0];
  for (i = 0; i < c_len; i++)
  {
    if (WaveData[i] > max)
      max = WaveData[i];
    if (WaveData[i] < min)
      min = WaveData[i];
  }
  // max = (uint16_t)max + 2;
  // min = (uint16_t)min;

#if 0
  //新波形清屏输出
  if (Flag)
    for (i = 0; i < StaticWaveLen; i++)
      if (i)
        Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], StaticWaveData[i][0], StaticWaveData[i][1], BackgroundColor);



  for (i = 0; i < c_len; i++)
  {
    uint16_t x = (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0);
    uint16_t y = (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0);
    if (i)
      Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], x, y, WaveColor);
    StaticWaveData[i][0] = x;
    StaticWaveData[i][1] = y;
  }
#elif 0
  // 新波形无清屏输出 V1.0:有缺陷，波形会残缺，新旧波形长度不一致导致清除原波形会覆盖一部分新波形上的点，V2.0:已解决
  static uint16_t x0, y0;
  if (c_len > StaticWaveLen)
  {
    for (i = 0; i < StaticWaveLen; i++)
    {

      if (Flag)
      {
        if (i)
        {
          Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], StaticWaveData[i][0], StaticWaveData[i][1], BackgroundColor);
        }
      }
      if (i)
        Gui_DrawLine(x0, y0, (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0), (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0), WaveColor);
      x0 = (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0);
      y0 = (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0);
    }
    for (i = StaticWaveLen; i < c_len; i++)
    {
      if (i)
        Gui_DrawLine(x0, y0, (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0), (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0), WaveColor);
      x0 = (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0);
      y0 = (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0);
    }
  }
  else
  {
    for (i = 0; i < c_len; i++)
    {
      if (Flag)
      {
        if (i)
        {
          Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], StaticWaveData[i][0], StaticWaveData[i][1], BackgroundColor);
        }
      }
      if (i)
        Gui_DrawLine(x0, y0, (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0), (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0), WaveColor);
      x0 = (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0);
      y0 = (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0);
    }
    for (i = c_len; i < StaticWaveLen; i++)
    {
      if (i)
        Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], StaticWaveData[i][0], StaticWaveData[i][1], BackgroundColor);
    }
  }
  for (i = 0; i < c_len; i++)
  {
    StaticWaveData[i][0] = (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0);
    StaticWaveData[i][1] = (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0);
  }

#else
  // 新波形无清屏输出, V2.0
  static uint16_t x0, y0;
  i = 1;
  j = 0;
  while (1)
  {
    if (i < StaticWaveLen && j < c_len)
    {

      if (StaticWaveData[i - 1][0] > (uint16_t)(1.0 * j / c_len * (c_x1 - c_x0) + c_x0))
      {
        if (j)
          Gui_DrawLine(x0, y0, (uint16_t)(1.0 * j / c_len * (c_x1 - c_x0) + c_x0), (uint16_t)(1.0 * (max - WaveData[j]) / (max - min) * (c_y1 - c_y0) + c_y0), WaveColor);
        x0 = (uint16_t)(1.0 * j / c_len * (c_x1 - c_x0) + c_x0);
        y0 = (uint16_t)(1.0 * (max - WaveData[j]) / (max - min) * (c_y1 - c_y0) + c_y0);
        j++;
      }
      else
      {
        if (Flag)
        {
          if (i)
          {
            Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], StaticWaveData[i][0], StaticWaveData[i][1], BackgroundColor);
          }
        }
        i++;
      }
    }
    else if (i < StaticWaveLen)
    {
      if (Flag)
      {
        if (i)
        {
          Gui_DrawLine(StaticWaveData[i - 1][0], StaticWaveData[i - 1][1], StaticWaveData[i][0], StaticWaveData[i][1], BackgroundColor);
        }
      }
      i++;
    }
    else if (j < c_len)
    {
      if (j)
        Gui_DrawLine(x0, y0, (uint16_t)(1.0 * j / c_len * (c_x1 - c_x0) + c_x0), (uint16_t)(1.0 * (max - WaveData[j]) / (max - min) * (c_y1 - c_y0) + c_y0), WaveColor);
      x0 = (uint16_t)(1.0 * j / c_len * (c_x1 - c_x0) + c_x0);
      y0 = (uint16_t)(1.0 * (max - WaveData[j]) / (max - min) * (c_y1 - c_y0) + c_y0);
      j++;
    }
    else
    {
      break;
    }
  }
  for (i = 0; i < c_len; i++)
  {
    StaticWaveData[i][0] = (uint16_t)(1.0 * i / c_len * (c_x1 - c_x0) + c_x0);
    StaticWaveData[i][1] = (uint16_t)(1.0 * (max - WaveData[i]) / (max - min) * (c_y1 - c_y0) + c_y0);
  }

#endif

  StaticWaveLen = c_len;
  Scale = 1.0 * (max - min) / ScaleNum;
  for (i = 0; i < ScaleNum; i++)
  {
    //  TFT_printf(WordColor, 1, c_x0-36, c_y0 - 8 + i * perH, "%.1f",(float)((max - Scale * i)/4096*3.3));
    snprintf(text, 5, "%.2f", (float)((max - Scale * i) / 1000));
    Gui_DrawFont_GBK16(c_x0 - 12 * 4, c_y0 - 8 + i * perH, WordColor, BackgroundColor, text);
  }

  Gui_DrawLine((c_x0), c_y1, (c_x0), c_y0, c_AxisColor);
  Gui_DrawLine((c_x0), (c_y1 + c_y0) / 2, (c_x1), (c_y1 + c_y0) / 2, c_AxisColor);
  Gui_DrawLine((c_x0), c_y0, (c_x0 - 3), c_y0 + 3, c_AxisColor);
  Gui_DrawLine((c_x0), c_y0, (c_x0 + 3), c_y0 + 3, c_AxisColor);
  Gui_DrawLine((c_x1), (c_y1 + c_y0) / 2, (c_x1 - 3), (c_y1 + c_y0) / 2 - 3, c_AxisColor);
  Gui_DrawLine((c_x1), (c_y1 + c_y0) / 2, (c_x1 - 3), (c_y1 + c_y0) / 2 + 3, c_AxisColor);

  if (!Flag)
  {
    Flag++;
  }

  free(WaveData);

}

// 柱状图
void Gui_DrawHistogram(uint16_t *amplitudeData, uint16_t h_len, float begin_x, float begin_y, float end_x, float end_y)
{
  /* 直方图矩形区域域起点(h_x0,h_y0) 终点(h_x1,h_y1) */
  uint16_t h_x0 = (uint16_t)(1.0 * LCD_X_SIZE * begin_x);
  uint16_t h_y0 = (uint16_t)(1.0 * LCD_Y_SIZE * begin_y);
  uint16_t h_x1 = (uint16_t)(1.0 * LCD_X_SIZE * end_x);
  uint16_t h_y1 = (uint16_t)(1.0 * LCD_Y_SIZE * end_y);

  /* 颜色 */
  uint16_t BackgroundColor = BLACK;
  uint16_t WordColor = WHITE;
  uint16_t HistogramColor = RED;
  uint16_t h_AxisColor = BLUE;

  /*数据的最值和刻度*/
  uint32_t max, min;
  // float  Scale;

  float PERW, PERH;

  static uint16_t StaticAmpData[10];
  static uint16_t StaticAmpLen;
  uint16_t i;
  static uint8_t Flag;

  uint8_t text[64];

  max = amplitudeData[0];
  for (i = 0; i < h_len; i++)
  {
    if (amplitudeData[i] > (uint16_t)max)
      max = amplitudeData[i];
  }
  PERW = 1.0 * (h_x1 - h_x0) / (h_len + 1);
  PERH = 1.0 * (h_y1 - h_y0 - 20) / max;

  if (Flag)
  {
    for (i = 0; i < StaticAmpLen; i++)
    {
      if (StaticAmpData[i] > (uint16_t)(h_y1 - PERH * amplitudeData[i]))
      {
        Gui_FillRectangle((uint16_t)(h_x0 + (i + 1) * PERW - 16), StaticAmpData[i] - 16, h_x0 + (i + 1) * PERW + 16, StaticAmpData[i], BackgroundColor);

        Gui_FillRectangle((uint16_t)(h_x0 + (i + 1) * PERW - PERW / 3), StaticAmpData[i] - 16, (uint16_t)(h_x0 + (i + 1) * PERW + PERW / 3), StaticAmpData[i], BackgroundColor);
        Gui_FillRectangle(((uint16_t)(h_x0 + (i + 1) * PERW - PERW / 3)), (uint16_t)(h_y1 - PERH * amplitudeData[i]), (uint16_t)(h_x0 + (i + 1) * PERW + PERW / 3), StaticAmpData[i] + 1, HistogramColor);
        // TFT_printf(WordColor, 1, (uint16_t)(h_x0+(i+1)*PERW-16), (uint16_t)(h_y1-PERH*amplitudeData[i]-16), "%3d",amplitudeData[i]);
        snprintf(text, 3, "%3d", amplitudeData[i]);
        Gui_DrawFont_GBK16((uint16_t)(h_x0 + (i + 1) * PERW - 16), (uint16_t)(h_y1 - PERH * amplitudeData[i] - 16), WordColor, BackgroundColor, text);
      }
      else if (StaticAmpData[i] == (uint16_t)(h_y1 - PERH * amplitudeData[i]))
      {
        Gui_FillRectangle((uint16_t)(h_x0 + (i + 1) * PERW - 16), StaticAmpData[i] - 16, h_x0 + (i + 1) * PERW + 16, StaticAmpData[i], BackgroundColor);
        // TFT_printf(WordColor, 1, (uint16_t)(h_x0+(i+1)*PERW-16), (uint16_t)(h_y1-PERH*amplitudeData[i]-16), "%3d",amplitudeData[i]);
        snprintf(text, 3, "%3d", amplitudeData[i]);
        Gui_DrawFont_GBK16((uint16_t)(h_x0 + (i + 1) * PERW - 16), (uint16_t)(h_y1 - PERH * amplitudeData[i] - 16), WordColor, BackgroundColor, text);
      }
      else
      {
        Gui_FillRectangle((uint16_t)(h_x0 + (i + 1) * PERW - 16), StaticAmpData[i] - 16, h_x0 + (i + 1) * PERW + 16, StaticAmpData[i], BackgroundColor);
        Gui_FillRectangle((uint16_t)(h_x0 + (i + 1) * PERW - PERW / 3), StaticAmpData[i] - 16, (uint16_t)(h_x0 + (i + 1) * PERW + PERW / 3), (uint16_t)(h_y1 - PERH * amplitudeData[i]), BackgroundColor);
        // TFT_printf(WordColor, 1, (uint16_t)(h_x0+(i+1)*PERW-16), (uint16_t)(h_y1-PERH*amplitudeData[i]-16), "%3d",amplitudeData[i]);
        snprintf(text, 3, "%3d", amplitudeData[i]);
        Gui_DrawFont_GBK16((uint16_t)(h_x0 + (i + 1) * PERW - 16), (uint16_t)(h_y1 - PERH * amplitudeData[i] - 16), WordColor, BackgroundColor, text);
      }
    }
  }
  else
  {
    for (i = 0; i < h_len; i++)
    {
      Gui_FillRectangle((uint16_t)(h_x0 + (i + 1) * PERW - PERW / 3), (uint16_t)(h_y1 - PERH * amplitudeData[i]), (uint16_t)(h_x0 + (i + 1) * PERW + PERW / 3), h_y1, HistogramColor);
      //   TFT_printf(WordColor, 1, (uint16_t)(h_x0+(i+1)*PERW-16), (uint16_t)(h_y1-PERH*amplitudeData[i]-16), "%3d",amplitudeData[i]);
      snprintf(text, 3, "%3d", amplitudeData[i]);
      Gui_DrawFont_GBK16((uint16_t)(h_x0 + (i + 1) * PERW - 16), (uint16_t)(h_y1 - PERH * amplitudeData[i] - 16), WordColor, BackgroundColor, text);
    }
  }
  for (i = 0; i < h_len; i++)
  {
    StaticAmpData[i] = (uint16_t)(h_y1 - PERH * amplitudeData[i]);
  }
  StaticAmpLen = h_len;

  Gui_DrawLine((h_x0), h_y1, (h_x0), h_y0, h_AxisColor);
  Gui_DrawLine((h_x0), h_y1, (h_x1), h_y1, h_AxisColor);
  Gui_DrawLine((h_x1), h_y0, (h_x0), h_y0, h_AxisColor);
  Gui_DrawLine((h_x1), h_y0, (h_x1), h_y1, h_AxisColor);
  if (!Flag)
  {
    Flag++;
  }
}

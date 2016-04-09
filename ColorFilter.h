#ifndef COLOR_FILTER_H
#define COLOR_FILTER_H

/* Initializes the fader. */
void ColorFilter_Init(void);

/* Unloads the filter from memory */
void ColorFilter_Unload(void);

/* 
  Sets the color of the filter.
  values from 0 to 2.
  1 retains original color value.
*/
void ColorFilter_Set(float r, float g, float b);

/*
Sets the fluctuation flag of the filter.
Keep fluctuation_amt between 0 and 1.
*/
void ColorFilter_Fluctuate(int useF, float r_amt, float g_amt, float b_amt);

/* Applies the filter. */
void ColorFilter_Draw(void);

#endif // !COLOR_FILTER_H


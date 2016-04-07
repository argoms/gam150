#ifndef COLOR_FILTER_H
#define COLOR_FILTER_H

/* Initializes the fader. */
void ColorFilter_Init(void);

/* Unloads the filter from memory */
void ColorFilter_Unload(void);

/* Sets the color of the filter. */
void ColorFilter_Set(float r, float g, float b);

/* Applies the filter. */
void ColorFilter_Draw(void);

#endif // !COLOR_FILTER_H


#ifndef FANCY_BACKGROUND_H
#define FANCY_BACKGROUND_H

/* Initializes the background visuals */
void Background_Init(void);

/* Unloads the background visuals from memory */
void Background_Unload(void);

/* Updates the transform information about the sprites. */
void Background_Update();

/* Draw the background. */
void Background_Draw();

#endif // !FANCY_BACKGROUND_H


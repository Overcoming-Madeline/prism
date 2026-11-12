
#include <stdio.h>
#include <stdlib.h>
#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#include "RGFW.h"

static unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

/* fill buffer with a color, clearing anything that was on it */
static void clear(u8* buffer, i32 bufferWidth, i32 w, i32 h, u8 color[4]) {
    /* if all the values are the same */
    if (color[0] == color[1] && color[0] == color[2] && color[0] == color[3]) {
        /* set it all in one function */
        memset(buffer, color[0], (u32)bufferWidth * (u32)h * 4 * sizeof(u8));
        return;
    }

    /* else we'll have to something more complex... */
    RGFW_UNUSED(w);
    /* loop through each *pixel* (not channel) of the buffer */
    u32 x, y;
    for (y = 0; y < (u32)h; y++) {
        for (x = 0; x < (u32)bufferWidth; x++) {
            u32 index = (y * 4 * (u32)bufferWidth) + x * 4;

            /* copy the color to that pixel */
            memcpy(&buffer[index], color, 4 * sizeof(u8));
        }
    }
}

static void drawBitmap(u8* buffer, i32 bufferWidth, u8* bitmap, i32 x, i32 y, i32 w, i32 h) {
    i32 y1 = y;
    for (y = 0; y < h; y++) {
        u32 index = (u32)(y + y1) * (4 * (u32)bufferWidth) + (u32)x * 4;
        memcpy(&buffer[index], &bitmap[(u32)y  * (4 * (u32)w)], (u32)w * 4 * sizeof(u8));
    }
}

static void drawRect(u8* buffer, i32 bufferWidth, i32 x, i32 y, i32 w, i32 h, u8 color[4]) {
    i32 x1 = x;
    i32 y1 = y;

    for(x = x1; x < w + x1; x++) {
        for(y = y1; y < h + y1; y++) {
            u32 index = (u32)y * (4 * (u32)bufferWidth) + (u32)x * 4;
            memcpy(&buffer[index], color, 4 * sizeof(u8));
        }
    }
}

u8* generateRandomColors(int size){
    u8* colArray = (u8*)malloc(size*sizeof(u8));

    for(int i;i < size;i++){
        colArray[i] = 255;
    }

    return colArray;
}

int main(void) {
    RGFW_init("Gradient Generator", 0);
    RGFW_window* win = RGFW_createWindow("Gradient Generator", 0, 0, 500, 500, RGFW_windowCenter | RGFW_windowOpenGL);
    RGFW_window_setExitKey(win, RGFW_keyEscape);

    RGFW_monitor* mon = RGFW_window_getMonitor(win);
	i32 width = 500;
	i32 height = 500;

    puts(generateRandomColors(10));

	if (mon) {
		width = (i32)((float)mon->mode.w * mon->pixelRatio);
		height = (i32)((float)mon->mode.h * mon->pixelRatio);
	}

	u8* buffer = (u8*)RGFW_ALLOC((u32)(width * height * 4));
	RGFW_surface* surface = RGFW_createSurface(buffer, width, height, RGFW_formatRGBA8);


    while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
		RGFW_pollEvents();

        u8 BackGroundColor[4] = {0, 0, 0, 0xFF};
        i32 w, h;

        RGFW_window_getSize(win, &w, &h);
        clear(buffer, width, w, h, BackGroundColor);
        RGFW_window_blitSurface(win, surface);
	}

    RGFW_surface_free(surface);
	RGFW_FREE(buffer);

    RGFW_window_close(win);
    RGFW_deinit();
}


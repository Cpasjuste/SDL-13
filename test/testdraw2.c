
/* Simple program:  draw as many random objects on the screen as possible */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "common.h"

#define NUM_OBJECTS	100

static CommonState *state;
static int num_objects;
static SDL_bool cycle_color;
static SDL_bool cycle_alpha;
static int cycle_direction = 1;
static int current_alpha = 255;
static int current_color = 255;
static SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;

void
DrawPoints(SDL_WindowID window)
{
    int i;
    int x, y;
    int window_w, window_h;

    /* Query the sizes */
    SDL_GetWindowSize(window, &window_w, &window_h);

    SDL_SetRenderDrawBlendMode(blendMode);
    for (i = 0; i < num_objects * 4; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        SDL_SetRenderDrawColor(255, (Uint8) current_color,
                               (Uint8) current_color, (Uint8) current_alpha);

        x = rand() % window_w;
        y = rand() % window_h;
        SDL_RenderPoint(x, y);
    }
    SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_NONE);
}

void
DrawLines(SDL_WindowID window)
{
    int i;
    int x1, y1, x2, y2;
    int window_w, window_h;

    /* Query the sizes */
    SDL_GetWindowSize(window, &window_w, &window_h);

    SDL_SetRenderDrawBlendMode(blendMode);
    for (i = 0; i < num_objects; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        SDL_SetRenderDrawColor(255, (Uint8) current_color,
                               (Uint8) current_color, (Uint8) current_alpha);

        if (i == 0) {
            SDL_RenderLine(0, 0, window_w - 1, window_h - 1);
            SDL_RenderLine(0, window_h - 1, window_w - 1, 0);
            SDL_RenderLine(0, window_h / 2, window_w - 1, window_h / 2);
            SDL_RenderLine(window_w / 2, 0, window_w / 2, window_h - 1);
        } else {
            x1 = rand() % window_w;
            x2 = rand() % window_w;
            y1 = rand() % window_h;
            y2 = rand() % window_h;
            SDL_RenderLine(x1, y1, x2, y2);
        }
    }
    SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_NONE);
}

void
DrawRects(SDL_WindowID window)
{
    int i;
    SDL_Rect rect;
    int window_w, window_h;

    /* Query the sizes */
    SDL_GetWindowSize(window, &window_w, &window_h);

    SDL_SetRenderDrawBlendMode(blendMode);
    for (i = 0; i < num_objects / 4; ++i) {
        /* Cycle the color and alpha, if desired */
        if (cycle_color) {
            current_color += cycle_direction;
            if (current_color < 0) {
                current_color = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_color > 255) {
                current_color = 255;
                cycle_direction = -cycle_direction;
            }
        }
        if (cycle_alpha) {
            current_alpha += cycle_direction;
            if (current_alpha < 0) {
                current_alpha = 0;
                cycle_direction = -cycle_direction;
            }
            if (current_alpha > 255) {
                current_alpha = 255;
                cycle_direction = -cycle_direction;
            }
        }
        SDL_SetRenderDrawColor(255, (Uint8) current_color,
                               (Uint8) current_color, (Uint8) current_alpha);

        rect.w = rand() % (window_h / 2);
        rect.h = rand() % (window_h / 2);
        rect.x = (rand() % window_w) - (rect.w / 2);
        rect.y = (rand() % window_w) - (rect.h / 2);
        SDL_RenderFill(&rect);
    }
    SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_NONE);
}

int
main(int argc, char *argv[])
{
    int i, done;
    SDL_Event event;
    Uint32 then, now, frames;

    /* Initialize parameters */
    num_objects = NUM_OBJECTS;

    /* Initialize test framework */
    state = CommonCreateState(argv, SDL_INIT_VIDEO);
    if (!state) {
        return 1;
    }
    for (i = 1; i < argc;) {
        int consumed;

        consumed = CommonArg(state, i);
        if (consumed == 0) {
            consumed = -1;
            if (SDL_strcasecmp(argv[i], "--blend") == 0) {
                if (argv[i + 1]) {
                    if (SDL_strcasecmp(argv[i + 1], "none") == 0) {
                        blendMode = SDL_BLENDMODE_NONE;
                        consumed = 2;
                    } else if (SDL_strcasecmp(argv[i + 1], "mask") == 0) {
                        blendMode = SDL_BLENDMODE_MASK;
                        consumed = 2;
                    } else if (SDL_strcasecmp(argv[i + 1], "blend") == 0) {
                        blendMode = SDL_BLENDMODE_BLEND;
                        consumed = 2;
                    } else if (SDL_strcasecmp(argv[i + 1], "add") == 0) {
                        blendMode = SDL_BLENDMODE_ADD;
                        consumed = 2;
                    } else if (SDL_strcasecmp(argv[i + 1], "mod") == 0) {
                        blendMode = SDL_BLENDMODE_MOD;
                        consumed = 2;
                    }
                }
            } else if (SDL_strcasecmp(argv[i], "--cyclecolor") == 0) {
                cycle_color = SDL_TRUE;
                consumed = 1;
            } else if (SDL_strcasecmp(argv[i], "--cyclealpha") == 0) {
                cycle_alpha = SDL_TRUE;
                consumed = 1;
            } else if (SDL_isdigit(*argv[i])) {
                num_objects = SDL_atoi(argv[i]);
                consumed = 1;
            }
        }
        if (consumed < 0) {
            fprintf(stderr,
                    "Usage: %s %s [--blend none|mask|blend|add|mod] [--cyclecolor] [--cyclealpha]\n",
                    argv[0], CommonUsage(state));
            return 1;
        }
        i += consumed;
    }
    if (!CommonInit(state)) {
        return 2;
    }

    /* Create the windows and initialize the renderers */
    for (i = 0; i < state->num_windows; ++i) {
        SDL_SelectRenderer(state->windows[i]);
        SDL_SetRenderDrawColor(0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderFill(NULL);
    }

    srand(time(NULL));

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    while (!done) {
        /* Check for events */
        ++frames;
        while (SDL_PollEvent(&event)) {
            CommonEvent(state, &event, &done);
            switch (event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_EXPOSED:
                    SDL_SelectRenderer(event.window.windowID);
                    SDL_SetRenderDrawColor(0xA0, 0xA0, 0xA0, 0xFF);
                    SDL_RenderFill(NULL);
                    break;
                }
                break;
            default:
                break;
            }
        }
        for (i = 0; i < state->num_windows; ++i) {
            SDL_SelectRenderer(state->windows[i]);
            SDL_SetRenderDrawColor(0xA0, 0xA0, 0xA0, 0xFF);
            SDL_RenderFill(NULL);

            DrawRects(state->windows[i]);
            DrawLines(state->windows[i]);
            DrawPoints(state->windows[i]);

            SDL_RenderPresent();
        }
    }

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        double fps = ((double) frames * 1000) / (now - then);
        printf("%2.2f frames per second\n", fps);
    }
    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */

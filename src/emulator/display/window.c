#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION

#include "../../../nuklear/nuklear.h"
#include "nuklear_sdl_binding.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024


//SDL for the machine
SDL_Window * create_window(char * name, int width, int height, int x, int y) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow(name, x, y, width, height,0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    //fix for linux systems
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return window;
}

SDL_Renderer * create_window_with_renderer(char * name, int width, int height, int x, int y) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow(name, x, y, width, height,0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(window == NULL) {
        printf("%s\n", SDL_GetError());
    }
    //fix for linux systems
    SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return renderer;
}

struct nk_context * create_nk_context(SDL_Window * window) {
    struct nk_context * ctx = nk_sdl_init(window);
    int win_width, win_height;
    SDL_GLContext gl_context;

    gl_context = SDL_GL_CreateContext(window);
    SDL_GetWindowSize(window, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);

    glewExperimental = 1;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to setup GLEW\n");
        exit(1);
    }

    return ctx;
}

// void create_nk_window(SDL_Window * window) {


//     /* GUI */
//     struct nk_colorf bg;

//     int running = 1;

//     SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
//     SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
//     SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
//     SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
//     SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


    
//     {
//         struct nk_font_atlas *atlas;
//         nk_sdl_font_stash_begin(&atlas);
//         /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
//         /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
//         /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
//         /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
//         /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
//         /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
//         nk_sdl_font_stash_end();
//         /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
//         /*nk_style_set_font(ctx, &roboto->handle);*/
//     }

//     bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

//     while(running) {
//         SDL_Event evt;
//         nk_input_begin(ctx);
//         while (SDL_PollEvent(&evt)) {
//             if (evt.type == SDL_QUIT) goto cleanup;
//             nk_sdl_handle_event(&evt);
//         } nk_input_end(ctx);
 
//         if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
//             NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
//             NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
//         {
//             enum {EASY, HARD};
//             static int op = EASY;
//             static int property = 20;

//             nk_layout_row_static(ctx, 30, 80, 1);
//             if (nk_button_label(ctx, "button"))
//                 printf("button pressed!\n");
//             nk_layout_row_dynamic(ctx, 30, 2);
//             if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
//             if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
//             nk_layout_row_dynamic(ctx, 22, 1);
//             nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

//             nk_layout_row_dynamic(ctx, 20, 1);
//             nk_label(ctx, "background:", NK_TEXT_LEFT);
//             nk_layout_row_dynamic(ctx, 25, 1);
//             if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx),400))) {
//                 nk_layout_row_dynamic(ctx, 120, 1);
//                 bg = nk_color_picker(ctx, bg, NK_RGBA);
//                 nk_layout_row_dynamic(ctx, 25, 1);
//                 bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
//                 bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
//                 bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
//                 bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
//                 nk_combo_end(ctx);
//             }
//         }
//         nk_end(ctx);
//         /* Draw */
//         SDL_GetWindowSize(window, &win_width, &win_height);
//         glViewport(0, 0, win_width, win_height);
//         glClear(GL_COLOR_BUFFER_BIT);
//         glClearColor(bg.r, bg.g, bg.b, bg.a);
//         /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
//          * with blending, scissor, face culling, depth test and viewport and
//          * defaults everything back into a default state.
//          * Make sure to either a.) save and restore or b.) reset your own state after
//          * rendering the UI. */
//         nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
//         SDL_GL_SwapWindow(window);
//     }

//     cleanup:
//         nk_sdl_shutdown();
//         SDL_GL_DeleteContext(gl_context);
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//     return;
// }


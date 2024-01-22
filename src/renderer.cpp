#include "renderer.h"

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_test_font.h>

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      menu_x(screen_width / 3),
      menu_y(screen_height / 3) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  clear_screen(0x1E, 0x1E, 0x1E, 0xFF);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);
  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);
  update_screen();
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) +
                    " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::clear_screen(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
  SDL_RenderClear(sdl_renderer);
}

void Renderer::update_screen() { SDL_RenderPresent(sdl_renderer); }

void Renderer::show_users(
    const std::vector<std::pair<std::string, std::string>> &data) {
  clear_screen(0x1E, 0x1E, 0x1E, 0xFF);
  std::string text;
  int row{-10};
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (auto &[user, highest_score] : data) {
    text = user + '\t' + highest_score;
    SDLTest_DrawString(sdl_renderer, menu_x, menu_y + (row += 10),
                       text.c_str());
  }
}

void Renderer::update_user_menu(
    const int from, const int to,
    const std::vector<std::pair<std::string, std::string>> &data) {
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  std::string text;
  {
    auto [user_f, score_f] = data[from];
    text = user_f + '\t' + score_f;
    SDLTest_DrawString(sdl_renderer, menu_x, menu_y + from * 10, text.c_str());
  }

  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  auto [user_t, score_t] = data[to];
  text = user_t + '\t' + score_t;
  SDLTest_DrawString(sdl_renderer, menu_x, menu_y + to * 10, text.c_str());
  update_screen();
}

void Renderer::welcome_user(const std::string &message) {
  clear_screen(0x1E, 0x1E, 0x1E, 0xFF);
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  SDLTest_DrawString(sdl_renderer, menu_x, menu_y, message.c_str());
  update_screen();
}

void Renderer::show_input_prompt() {
  clear_screen(0x1E, 0x1E, 0x1E, 0xFF);
  SDLTest_DrawString(sdl_renderer, menu_x, menu_y, "_");
}

void Renderer::update_input_prompt(const std::string &user_input) {
  clear_screen(0x1E, 0x1E, 0x1E, 0xFF);
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDLTest_DrawString(sdl_renderer, menu_x, menu_y, user_input.c_str());
  update_screen();
}
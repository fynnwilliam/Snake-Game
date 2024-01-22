#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <string>
#include <vector>

#include "SDL.h"
#include "snake.h"
class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer &operator=(Renderer &&) = default;
  Renderer(Renderer &&) = default;
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food);
  void UpdateWindowTitle(int score, int fps);
  void show_users(const std::vector<std::pair<std::string, std::string>> &);
  void update_user_menu(
      const int, const int,
      const std::vector<std::pair<std::string, std::string>> &data);
  void welcome_user(const std::string &);
  void show_input_prompt();
  void update_input_prompt(const std::string &);

 private:
  void clear_screen(uint8_t, uint8_t, uint8_t, uint8_t);
  void update_screen();

  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  const int menu_x;
  const int menu_y;
};

#endif
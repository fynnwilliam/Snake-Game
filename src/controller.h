#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

#include "snake.h"

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;
  void HandleInput(int &) const noexcept;
  void HandleInput(bool &, std::string &) const noexcept;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif
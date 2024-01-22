#ifndef USER_DATA_H
#define USER_DATA_H

#include <chrono>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include "controller.h"
#include "renderer.h"

namespace {
using namespace std::chrono_literals;
using namespace std::string_literals;

const auto get_users_from_file = []() {
  std::ifstream file{"users"};
  std::string user, score;
  std::vector<std::pair<std::string, std::string>> data{
      {"please select a name or make one"s, ""s}};

  while (file >> user >> score) data.emplace_back(user, score);

  data.emplace_back("new user"s, ""s);
  data.emplace_back("exit"s, ""s);
  return data;
};

const auto _write_data_to_file = [](const auto &data) {
  auto first = std::begin(data);
  const auto last = std::end(data) - 2;

  std::ofstream file{"users"s};
  while (++first < last) {
    const auto &[user, score] = *first;
    file << user << ' ' << score << '\n';
  }
};

const auto write_data_to_file = [](const int selected_index, auto &data,
                                   const int current_score) {
  if (auto size = data.size();
      selected_index > 0 && selected_index < size - 2) {
    auto &user_score = data[selected_index].second;
    const auto max_score = std::max(std::stoi(user_score), current_score);
    user_score = std::to_string(max_score);
    _write_data_to_file(data);
  }
};

const auto make_new_user = [](bool &running, const Controller &controller,
                              Renderer &renderer) {
  bool toggle = true;
  std::string user_name;

  renderer.show_input_prompt();
  while (running) {
    std::this_thread::sleep_for(190ms);
    renderer.update_input_prompt(
        user_name + std::string{}.append(static_cast<int>(toggle ^= 1), '_'));
    controller.HandleInput(running, user_name);
  }

  return user_name;
};

const auto setup_user = [](const Controller &controller, Renderer &renderer,
                           auto &data) {
  bool running = true;
  int key = 0;
  int from = 1;
  int to = 1;

  const auto data_at = [size = data.size() - 1](const int key, int &from,
                                                int &to) {
    from = to;
    bool move = !(key == -1 && from == 1 || key == 1 && from == size);
    to = from + key * static_cast<int>(move);
  };

  renderer.show_users(data);
  while (running) {
    std::this_thread::sleep_for(120ms);
    controller.HandleInput(key);
    if (key == 2 || key == 4) {
      std::string selected_option = data[to].first;
      if (selected_option == "exit"s || key == 4) {
        return -1;
      } else if (selected_option == "new user"s) {
        selected_option = make_new_user(running, controller, renderer);
        if (running = selected_option.size() > 0) {
          if (selected_option == "esc") {
            renderer.show_users(data);
            continue;
          }
          data.insert(std::end(data) - 2, {selected_option, "0"s});
        } else
          return -1;
      }

      selected_option = "Good to have you, "s + selected_option;
      renderer.welcome_user(selected_option);
      std::this_thread::sleep_for(2'000ms);
      return to;
    }
    data_at(key, from, to);
    renderer.update_user_menu(from, to, data);
  }
  return -1;
};
}  // namespace

#endif  // USER_DATA_H
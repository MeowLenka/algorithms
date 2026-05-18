#include "my_player.hpp"
#include <cmath>
#include <limits>
#include <queue>
#include <vector>

namespace ttt::my_player
{
  static const int WIN_LENGTH = 5;

  void MyPlayer::set_sign(Sign sign) { m_sign = sign; }

  const char *MyPlayer::get_name() const { return m_name; }
  struct Board
  {
    int cols;
    int rows;
    std::vector<Sign> cells;
    Sign get(int x, int y) const
    {
      return cells[y * cols + x];
    }
    void set(int x, int y, Sign sign)
    {
      cells[y * cols + x] = sign;
    }
  };

  struct ClusterInfo
  {
    bool valid;
    int center_x;
    int center_y;
    int size;
  };

  static Point make_point(int x, int y);

  static bool inside(const Board &board, int x, int y);
  static bool is_free(const Board &board, int x, int y);
  static bool is_player_sign(Sign sign);

  static int count_possible_lines_through_cell(const Board &board, int x, int y);

  static int obstacle_penalty_near_cell(const Board &board, int x, int y);

  static ClusterInfo find_largest_free_cluster_center(const Board &board);

  static Sign get_enemy(Sign sign)
  {
    if (sign == Sign::X)
      return Sign::O;
    if (sign == Sign::O)
      return Sign::X;
    return Sign::NONE;
  }
  static bool is_player_sign(Sign sign)
  {
    return sign == Sign::X || sign == Sign::O;
  }
  static bool inside(const Board &board, int x, int y)
  {
    return x >= 0 && x < board.cols && y >= 0 && y < board.rows;
  }
  static bool is_free(const Board &board, int x, int y)
  {
    return inside(board, x, y) && board.get(x, y) == Sign::NONE;
  }
  static Point make_point(int x, int y)
  {
    Point p;
    p.x = x;
    p.y = y;
    return p;
  }

  static Board make_board(const State &state)
  {
    Board board;
    board.cols = state.get_opts().cols;
    board.rows = state.get_opts().rows;
    board.cells.resize(board.cols * board.rows);
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
        board.set(x, y, state.get_value(x, y));
    }
    return board;
  }

  static bool board_empty(const Board &board)
  {
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (is_player_sign(board.get(x, y)))
          return false;
      }
    }
    return true;
  }

  static int count_free_cells(const Board &board)
  {
    int result = 0;
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (board.get(x, y) == Sign::NONE)
          ++result;
      }
    }
    return result;
  }

  static bool has_neighbor(const Board &board, int x, int y, int radius)
  {
    for (int dy = -radius; dy <= radius; ++dy)
    {
      for (int dx = -radius; dx <= radius; ++dx)
      {
        if (dx == 0 && dy == 0)
          continue;
        int nx = x + dx;
        int ny = y + dy;
        if (!inside(board, nx, ny))
          continue;
        if (is_player_sign(board.get(nx, ny)))
          return true;
      }
    }
    return false;
  }

  static std::vector<Point> get_candidate_cells(const Board &board)
  {
    std::vector<Point> result;
    bool empty = board_empty(board);
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (!is_free(board, x, y))
          continue;
        if (empty || has_neighbor(board, x, y, 2))
        {
          result.push_back(make_point(x, y));
        }
      }
    }
    if (!result.empty())
    {
      return result;
    }
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (is_free(board, x, y))
          result.push_back(make_point(x, y));
      }
    }
    return result;
  }

  static bool has_line_from_cell(
      const Board &board,
      int x,
      int y,
      Sign sign)
  {
    const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}};
    for (int d = 0; d < 4; ++d)
    {
      int dx = directions[d][0];
      int dy = directions[d][1];
      int count = 1;
      int nx = x + dx;
      int ny = y + dy;
      while (inside(board, nx, ny) && board.get(nx, ny) == sign)
      {
        ++count;
        nx += dx;
        ny += dy;
      }
      nx = x - dx;
      ny = y - dy;
      while (inside(board, nx, ny) && board.get(nx, ny) == sign)
      {
        ++count;
        nx -= dx;
        ny -= dy;
      }
      if (count >= WIN_LENGTH)
      {
        return true;
      }
    }
    return false;
  }
  
  static bool has_line_after_move(
      const Board &board,
      Point move,
      Sign sign)
  {
    Board copy = board;
    copy.set(move.x, move.y, sign);
    return has_line_from_cell(copy, move.x, move.y, sign);
  }
  static int count_immediate_wins(
      const Board &board,
      Sign sign)
  {
    std::vector<Point> cells = get_candidate_cells(board);
    int result = 0;
    for (Point move : cells)
    {
      if (has_line_after_move(board, move, sign))
      {
        ++result;
      }
    }
    return result;
  }
  static int count_immediate_wins_limited(
      const Board &board,
      Sign sign,
      int limit)
  {
    std::vector<Point> cells = get_candidate_cells(board);
    int result = 0;
    for (Point move : cells)
    {
      if (has_line_after_move(board, move, sign))
      {
        ++result;
        if (result >= limit)
        {
          return result;
        }
      }
    }
    return result;
  }
  // X поставил линию.
  // Это реальная победа X только если O не может последним ходом собрать линию.
  static bool is_real_x_win_after_move(
      const Board &board,
      Point move)
  {
    Board after_x = board;
    after_x.set(move.x, move.y, Sign::X);
    if (!has_line_from_cell(after_x, move.x, move.y, Sign::X))
    {
      return false;
    }
    // Если поле заполнено, O не может сделать последний ход.
    if (count_free_cells(after_x) == 0)
    {
      return true;
    }
    int o_answers = count_immediate_wins_limited(after_x, Sign::O, 1);
    return o_answers == 0;
  }
  // X поставил линию, но O может ответить своей линией.
  // Это не победа X, а гарантированная ничья.
  static bool is_x_draw_after_move(
      const Board &board,
      Point move)
  {
    Board after_x = board;
    after_x.set(move.x, move.y, Sign::X);
    if (!has_line_from_cell(after_x, move.x, move.y, Sign::X))
    {
      return false;
    }
    if (count_free_cells(after_x) == 0)
    {
      return false;
    }
    int o_answers = count_immediate_wins_limited(after_x, Sign::O, 1);
    return o_answers > 0;
  }
  static long long pattern_score(
      int own_count,
      int empty_count,
      int blocked_count)
  {
    if (blocked_count > 0)
    {
      return 0;
    }
    if (own_count >= 5)
    {
      return 100000000LL;
    }
    if (own_count == 4 && empty_count == 1)
    {
      return 3000000LL;
    }
    if (own_count == 3 && empty_count == 2)
    {
      return 80000LL;
    }
    if (own_count == 2 && empty_count == 3)
    {
      return 3000LL;
    }
    if (own_count == 1 && empty_count == 4)
    {
      return 30LL;
    }
    return 0;
  }
  static long long evaluate_board_for_sign(
      const Board &board,
      Sign sign)
  {
    const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}};
    Sign enemy = get_enemy(sign);
    long long score = 0;
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        for (int d = 0; d < 4; ++d)
        {
          int dx = directions[d][0];
          int dy = directions[d][1];
          int end_x = x + (WIN_LENGTH - 1) * dx;
          int end_y = y + (WIN_LENGTH - 1) * dy;
          if (!inside(board, end_x, end_y))
            continue;
          int own_count = 0;
          int empty_count = 0;
          int blocked_count = 0;
          for (int i = 0; i < WIN_LENGTH; ++i)
          {
            int nx = x + i * dx;
            int ny = y + i * dy;
            Sign cell = board.get(nx, ny);
            if (cell == sign)
            {
              ++own_count;
            }
            else if (cell == Sign::NONE)
            {
              ++empty_count;
            }
            else if (cell == enemy)
            {
              ++blocked_count;
            }
            else
            {
              ++blocked_count;
            }
          }
          score += pattern_score(own_count, empty_count, blocked_count);
        }
      }
    }
    return score;
  }
  static int neighbor_bonus(
      const Board &board,
      Point move,
      Sign my_sign)
  {
    int bonus = 0;
    Sign enemy = get_enemy(my_sign);
    for (int dy = -2; dy <= 2; ++dy)
    {
      for (int dx = -2; dx <= 2; ++dx)
      {
        if (dx == 0 && dy == 0)
          continue;
        int x = move.x + dx;
        int y = move.y + dy;
        if (!inside(board, x, y))
          continue;
        Sign cell = board.get(x, y);
        int distance = std::abs(dx) + std::abs(dy);
        int value = distance <= 1 ? 30 : 8;
        if (cell == my_sign)
        {
          bonus += value;
        }
        else if (cell == enemy)
        {
          bonus += value / 2;
        }
      }
    }
    return bonus;
  }
  static int strategic_center_bonus(
      Point move,
      const ClusterInfo &cluster)
  {
    if (!cluster.valid)
    {
      return 0;
    }
    int distance =
        std::abs(move.x - cluster.center_x) +
        std::abs(move.y - cluster.center_y);
    return -distance;
  }
  // Специальный выбор атакующего хода за O.
  // Ищем ход, после которого у O появляется одна или несколько
  // немедленных победных угроз на следующий ход.
  // При этом не выбираем ход, после которого X сразу получает линию.
  static bool find_o_attack_move(
      const Board &board,
      const std::vector<Point> &cells,
      const ClusterInfo &cluster,
      Point &best_move)
  {
    bool found = false;
    long long best_score = std::numeric_limits<long long>::min();
    for (Point move : cells)
    {
      Board after_o = board;
      after_o.set(move.x, move.y, Sign::O);
      // Если после нашего хода X сразу может собрать линию,
      // такой ход слишком опасен.
      int x_wins = count_immediate_wins_limited(after_o, Sign::X, 1);
      if (x_wins > 0)
      {
        continue;
      }
      int o_wins = count_immediate_wins_limited(after_o, Sign::O, 2);
      if (o_wins == 0)
      {
        continue;
      }
      long long score = 0;
      // Две и более угрозы почти всегда сильнее одной.
      if (o_wins >= 2)
      {
        score += 150000000LL;
      }
      else
      {
        score += 30000000LL;
      }
      score += evaluate_board_for_sign(after_o, Sign::O) * 12;
      score -= evaluate_board_for_sign(after_o, Sign::X) * 8;
      score += neighbor_bonus(board, move, Sign::O);
      score += strategic_center_bonus(move, cluster);
      if (!found || score > best_score)
      {
        found = true;
        best_score = score;
        best_move = move;
      }
    }
    return found;
  }
  static long long evaluate_move(
      const Board &board,
      Point move,
      Sign my_sign,
      const ClusterInfo &cluster)
  {
    Sign enemy = get_enemy(my_sign);
    Board after_my_move = board;
    after_my_move.set(move.x, move.y, my_sign);
    // Особое правило:
    // O выигрывает сразу, X выигрывает только если O не может ответить линией.
    if (my_sign == Sign::O)
    {
      if (has_line_from_cell(after_my_move, move.x, move.y, Sign::O))
      {
        return 1000000000LL;
      }
    }
    if (my_sign == Sign::X)
    {
      if (has_line_from_cell(after_my_move, move.x, move.y, Sign::X))
      {
        if (count_free_cells(after_my_move) == 0)
        {
          return 1000000000LL;
        }
        int o_answers = count_immediate_wins_limited(after_my_move, Sign::O, 1);
        if (o_answers == 0)
        {
          return 1000000000LL;
        }
        // Это не победа, а ничья. Хорошо как fallback,
        // но не считаем это абсолютной победой.
        return 10000000LL;
      }
    }
    long long score = 0;
    int enemy_immediate_wins = count_immediate_wins(after_my_move, enemy);
    int my_immediate_wins_next = count_immediate_wins(after_my_move, my_sign);
    if (enemy_immediate_wins > 0)
    {
      if (my_sign == Sign::O)
      {
        score -= 120000000LL * enemy_immediate_wins;
      }
      else
      {
        score -= 50000000LL * enemy_immediate_wins;
      }
    }
    if (my_sign == Sign::O)
    {
      if (my_immediate_wins_next >= 2)
      {
        score += 150000000LL;
      }
      else if (my_immediate_wins_next == 1)
      {
        score += 30000000LL;
      }
    }
    else
    {
      if (my_immediate_wins_next >= 2)
      {
        score += 30000000LL;
      }
      else if (my_immediate_wins_next == 1)
      {
        score += 3000000LL;
      }
    }
    long long attack = evaluate_board_for_sign(after_my_move, my_sign);
    long long defense = evaluate_board_for_sign(after_my_move, enemy);
    if (my_sign == Sign::O)
    {
      // O должен атаковать, но не может игнорировать угрозы X.
      // При слабой защите O слишком часто проигрывает.
      score += attack * 22;
      score -= defense * 24;
    }
    else
    {
      // X должен защищаться сильнее, потому что после линии X у O есть последний ход.
      score += attack * 12;
      score -= defense * 24;
    }
    score += neighbor_bonus(board, move, my_sign);
    score += strategic_center_bonus(move, cluster);
    return score;
  }
  static int count_possible_lines_through_cell(
      const Board &board,
      int x,
      int y)
  {
    const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}};
    int result = 0;
    for (int d = 0; d < 4; ++d)
    {
      int dx = directions[d][0];
      int dy = directions[d][1];
      for (int shift = -(WIN_LENGTH - 1); shift <= 0; ++shift)
      {
        int start_x = x + shift * dx;
        int start_y = y + shift * dy;
        bool valid = true;
        for (int i = 0; i < WIN_LENGTH; ++i)
        {
          int nx = start_x + i * dx;
          int ny = start_y + i * dy;
          if (!inside(board, nx, ny) || board.get(nx, ny) != Sign::NONE)
          {
            valid = false;
            break;
          }
        }
        if (valid)
        {
          ++result;
        }
      }
    }
    return result;
  }
  static int obstacle_penalty_near_cell(
      const Board &board,
      int x,
      int y)
  {
    int penalty = 0;
    for (int dy = -2; dy <= 2; ++dy)
    {
      for (int dx = -2; dx <= 2; ++dx)
      {
        if (dx == 0 && dy == 0)
          continue;
        int nx = x + dx;
        int ny = y + dy;
        if (!inside(board, nx, ny))
          continue;
        Sign cell = board.get(nx, ny);
        if (cell != Sign::NONE && cell != Sign::X && cell != Sign::O)
        {
          int distance = std::abs(dx) + std::abs(dy);
          if (distance <= 1)
          {
            penalty += 20;
          }
          else
          {
            penalty += 7;
          }
        }
      }
    }
    return penalty;
  }
  static ClusterInfo find_largest_free_cluster_center(const Board &board)
  {
    ClusterInfo best_cluster;
    best_cluster.valid = false;
    best_cluster.center_x = 0;
    best_cluster.center_y = 0;
    best_cluster.size = 0;
    std::vector<int> visited(board.cols * board.rows, 0);
    const int directions[8][2] = {
        {1, 0},
        {-1, 0},
        {0, 1},
        {0, -1},
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1}};
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (!is_free(board, x, y))
        {
          continue;
        }
        int start_index = y * board.cols + x;
        if (visited[start_index])
        {
          continue;
        }
        std::queue<Point> queue;
        std::vector<Point> component;
        visited[start_index] = 1;
        queue.push(make_point(x, y));
        long long sum_x = 0;
        long long sum_y = 0;
        while (!queue.empty())
        {
          Point current = queue.front();
          queue.pop();
          component.push_back(current);
          sum_x += current.x;
          sum_y += current.y;
          for (int d = 0; d < 8; ++d)
          {
            int nx = current.x + directions[d][0];
            int ny = current.y + directions[d][1];
            if (!inside(board, nx, ny))
            {
              continue;
            }
            int index = ny * board.cols + nx;
            if (visited[index])
            {
              continue;
            }
            if (!is_free(board, nx, ny))
            {
              continue;
            }
            visited[index] = 1;
            queue.push(make_point(nx, ny));
          }
        }
        int component_size = static_cast<int>(component.size());
        if (component_size == 0)
        {
          continue;
        }
        Point center_cell = component.front();
        long long best_distance = std::numeric_limits<long long>::max();
        long long best_cell_score = std::numeric_limits<long long>::min();
        for (Point cell : component)
        {
          long long dx = static_cast<long long>(cell.x) * component_size - sum_x;
          long long dy = static_cast<long long>(cell.y) * component_size - sum_y;
          long long distance = dx * dx + dy * dy;
          int possible_lines = count_possible_lines_through_cell(
              board,
              cell.x,
              cell.y);
          int obstacle_penalty = obstacle_penalty_near_cell(
              board,
              cell.x,
              cell.y);
          long long cell_score = 0;
          cell_score += possible_lines * 1000LL;
          cell_score -= obstacle_penalty * 20LL;
          if (distance < best_distance ||
              (distance == best_distance && cell_score > best_cell_score))
          {
            best_distance = distance;
            best_cell_score = cell_score;
            center_cell = cell;
          }
        }
        if (!best_cluster.valid || component_size > best_cluster.size)
        {
          best_cluster.valid = true;
          best_cluster.center_x = center_cell.x;
          best_cluster.center_y = center_cell.y;
          best_cluster.size = component_size;
        }
      }
    }
    return best_cluster;
  }
  static Point choose_first_move(
      const Board &board,
      const ClusterInfo &cluster)
  {
    Point best = make_point(0, 0);
    long long best_score = std::numeric_limits<long long>::min();
    for (int y = 0; y < board.rows; ++y)
    {
      for (int x = 0; x < board.cols; ++x)
      {
        if (!is_free(board, x, y))
          continue;
        int possible_lines = count_possible_lines_through_cell(board, x, y);
        if (possible_lines == 0)
          continue;
        int obstacle_penalty = obstacle_penalty_near_cell(board, x, y);
        int distance_to_cluster_center = 0;
        if (cluster.valid)
        {
          distance_to_cluster_center =
              std::abs(x - cluster.center_x) +
              std::abs(y - cluster.center_y);
        }
        long long score = 0;
        score += possible_lines * 1000LL;
        score -= obstacle_penalty * 20LL;
        score -= distance_to_cluster_center;
        if (score > best_score)
        {
          best_score = score;
          best = make_point(x, y);
        }
      }
    }
    return best;
  }
  Point MyPlayer::make_move(const State &state)
  {
    Board board = make_board(state);
    ClusterInfo free_cluster = find_largest_free_cluster_center(board);
    std::vector<Point> cells = get_candidate_cells(board);
    if (cells.empty())
    {
      if (free_cluster.valid)
      {
        return make_point(free_cluster.center_x, free_cluster.center_y);
      }
      return make_point(0, 0);
    }
    if (board_empty(board))
    {
      return choose_first_move(board, free_cluster);
    }
    Point draw_by_x = cells.front();
    bool has_draw_by_x = false;
    // 1. Немедленная победа с учетом особого правила X/O.
    if (m_sign == Sign::O)
    {
      // O выигрывает сразу.
      for (Point move : cells)
      {
        if (has_line_after_move(board, move, Sign::O))
        {
          return move;
        }
      }
    }
    else if (m_sign == Sign::X)
    {
      // X выигрывает только если O не сможет ответить своей линией.
      for (Point move : cells)
      {
        if (is_real_x_win_after_move(board, move))
        {
          return move;
        }
      }
      // Если есть ход X в линию, но O может ответить линией,
      // это гарантированная ничья. Запоминаем как fallback.
      for (Point move : cells)
      {
        if (is_x_draw_after_move(board, move))
        {
          draw_by_x = move;
          has_draw_by_x = true;
          break;
        }
      }
    }
    // 2. Блок немедленной победы соперника.
    // Для X это критично: O выигрывает сразу.
    // Для O тоже блокируем линию X, потому что иначе X получает сильнейший темп,
    // а O часто не успевает реализовать ответ.
    if (m_sign == Sign::X)
    {
      for (Point move : cells)
      {
        if (has_line_after_move(board, move, Sign::O))
        {
          return move;
        }
      }
    }
    else if (m_sign == Sign::O)
    {
      // Сначала обязательно блокируем немедленную линию X.
      for (Point move : cells)
      {
        if (has_line_after_move(board, move, Sign::X))
        {
          return move;
        }
      }
      // Если срочного блока нет, ищем активный атакующий ход за O.
      Point o_attack_move = cells.front();
      if (find_o_attack_move(board, cells, free_cluster, o_attack_move))
      {
        return o_attack_move;
      }
    }
    Point best_move = cells.front();
    long long best_score = std::numeric_limits<long long>::min();
    for (Point move : cells)
    {
      long long score = evaluate_move(board, move, m_sign, free_cluster);
      if (score > best_score)
      {
        best_score = score;
        best_move = move;
      }
    }
    // Если мы играем за X и обычная эвристика видит плохую позицию,
    // но есть ход, который гарантирует ничью, выбираем ничью.
    if (m_sign == Sign::X && has_draw_by_x && best_score < 0)
    {
      return draw_by_x;
    }
    return best_move;
  }
} // namespace ttt::my_player
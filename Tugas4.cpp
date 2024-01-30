#include <iostream>
#include <vector>
#include <limits>

const int BOARD_SIZE = 3;
char HUMAN_PLAYER;
char AI_PLAYER;

struct Move {
  int row, col;
};

char board[BOARD_SIZE][BOARD_SIZE];

void initializeBoard() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i][j] = '-';
    }
  }
}

void printBoard() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      std::cout << board[i][j] << " ";
    }
    std::cout << "\n";
  }
}

bool isBoardFull() {
  int count = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == '-') {
        count++;
      }
    }
  }
  return count == 1;
}

bool isWinningMove(char player) {
  // Check rows
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
      return true;
    }
  }

  // Check columns
  for (int j = 0; j < BOARD_SIZE; j++) {
    if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
      return true;
    }
  }

  // Check diagonals
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
    return true;
  }
  if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
    return true;
  }

  return false;
}

int evaluateBoard() {
  if (isWinningMove(AI_PLAYER)) {
    return 10;
  }
  else if (isWinningMove(HUMAN_PLAYER)) {
    return -10;
  }
  else {
    return 0;
  }
}

int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta) {
  int score = evaluateBoard();

  if (score == 10) {
    return score - depth;
  }
  if (score == -10) {
    return score + depth;
  }
  if (isBoardFull()) {
    return 0;
  }

  if (isMaximizingPlayer) {
    int bestScore = std::numeric_limits<int>::min();
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[i][j] == '-') {
          board[i][j] = AI_PLAYER;
          int currentScore = minimax(depth + 1, false, alpha, beta);
          board[i][j] = '-';
          bestScore = std::max(bestScore, currentScore);
          alpha = std::max(alpha, bestScore);
          if (beta <= alpha) {
            break;
          }
        }
      }
    }
    return bestScore;
  }
  else {
    int bestScore = std::numeric_limits<int>::max();
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[i][j] == '-') {
          board[i][j] = HUMAN_PLAYER;
          int currentScore = minimax(depth + 1, true, alpha, beta);
          board[i][j] = '-';
          bestScore = std::min(bestScore, currentScore);
          beta = std::min(beta, bestScore);
          if (beta <= alpha) {
            break;
          }
        }
      }
    }
    return bestScore;
  }
}

Move getBestMove() {
  int bestScore = std::numeric_limits<int>::min();
  Move bestMove;
  bestMove.row = -1;
  bestMove.col = -1;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (board[i][j] == '-') {
        board[i][j] = AI_PLAYER;
        int currentScore = minimax(0, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
        board[i][j] = '-';
        if (currentScore > bestScore) {
          bestScore = currentScore;
          bestMove.row = i;
          bestMove.col = j;
        }
      }
    }
  }

  return bestMove;
}

void humanMove() {
  int row, col;
  std::cout << "Enter row and column (1-3): ";
  std::cin >> row >> col;
  row--;
  col--;
  if (board[row][col] != '-') {
    std::cout << "Invalid move. Try again." << "\n";
    humanMove();
  }
  else {
    board[row][col] = HUMAN_PLAYER;
  }
}

void aiMove() {
  Move bestMove = getBestMove();
  board[bestMove.row][bestMove.col] = AI_PLAYER;
  std::cout << "AI moves: " << bestMove.row + 1 << ", " << bestMove.col + 1 << "\n";
}

int main() {
  initializeBoard();
  std::cout << "Choose your player (X or O): ";
  std::cin >> HUMAN_PLAYER;
  if (HUMAN_PLAYER == 'X') {
    AI_PLAYER = 'O';
    while (true) {
      printBoard();
      humanMove();
      if (isWinningMove(HUMAN_PLAYER)) {
        std::cout << "You win!" << "\n";
        break;
      }
      if (isBoardFull()) {
        std::cout << "It's a tie!" << "\n";
        break;
      }
      aiMove();
      if (isWinningMove(AI_PLAYER)) {
        std::cout << "AI wins!" << "\n";
        break;
      }
      if (isBoardFull()) {
        std::cout << "It's a tie!" << "\n";
        break;
      }
    }
  }
  else if (HUMAN_PLAYER == 'O') {
    AI_PLAYER = 'X';
    while (true) {
      aiMove();
      if (isWinningMove(AI_PLAYER)) {
        std::cout << "AI wins!" << "\n";
        break;
      }
      if (isBoardFull()) {
        std::cout << "It's a tie!" << "\n";
        break;
      }
      printBoard();
      humanMove();
      if (isWinningMove(HUMAN_PLAYER)) {
        std::cout << "You win!" << "\n";
        break;
      }
      if (isBoardFull()) {
        std::cout << "It's a tie!" << "\n";
        break;
      }
    }
  }
  else {
    std::cout << "Invalid player choice." << "\n";
  }
  printBoard();
  return 0;
}

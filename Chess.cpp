#include <iostream>
#include <vector>

const int BOARD_SIZE = 5;

enum PieceType {
  WHITE_KING,
  BLACK_QUEEN,
  BLACK_KING,
  EMPTY
};

struct Piece {
  PieceType type;
  int row;
  int col;
};

Piece board[BOARD_SIZE][BOARD_SIZE];
Piece w_king, b_king, b_queen;

Piece initPiece(PieceType typein, int row, int col){
  Piece A;
  A.type=typein;
  A.row=row;
  A.col=col;
  return A;
}

void initBoard() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      board[i][j].type = EMPTY;
    }
  }
}

void printBoard() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      switch (board[i][j].type) {
        case WHITE_KING:
          printf("WK ");
          break;
        case BLACK_QUEEN:
          printf("BQ ");
          break;
        case BLACK_KING:
          printf("BK ");
          break;
        case EMPTY:
          printf(" . ");
          break;
      }
    }
    printf("\n");
  }
  printf("\n");
}

bool positionInBoard(int row, int col) {
   return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool positionIsOccupiedBySameSide(PieceType piece, int row, int col) {
  switch (piece) {
    case WHITE_KING:
      return false;
      break;
    case BLACK_QUEEN:
      return board[row][col].type == BLACK_KING;
      break;
    case BLACK_KING:
      return board[row][col].type == BLACK_QUEEN;
      break;
  }
}

std::vector<Piece> generateQueenMoves(){
  std::vector<Piece> moves;
  int row_direction[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
  int column_direction[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
  
  for (int i = 0; i < 8; i++) {
    int newRow = b_queen.row;
    int newCol = b_queen.col;
    while (true) {
      newRow += row_direction[i];
      newCol += column_direction[i];
      if (positionInBoard(newRow, newCol)) {
        if (positionIsOccupiedBySameSide(BLACK_QUEEN, newRow, newCol)) break;
        Piece move = initPiece(BLACK_QUEEN, newRow, newCol);
        moves.push_back(move);
      } else {
        break;
      }
    }
  }
  
  return moves;
}

std::vector<Piece> generateKingMoves(Piece piece){
  std::vector<Piece> moves;
  int row_direction[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
  int column_direction[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
  
  for (int i = 0; i < 8; i++) {
    int newRow = piece.row + row_direction[i];
    int newCol = piece.col + column_direction[i];
    if (positionInBoard(newRow, newCol) && !positionIsOccupiedBySameSide(piece.type, newRow, newCol)) {
      Piece move = initPiece(piece.type, newRow, newCol);
      moves.push_back(move);
    }
  }
  
  return moves;
}

bool positionInCheck(Piece w_king_, Piece b_queen_, Piece b_king_){
  // check by black queen
  bool check = (w_king_.row== b_queen_.row || w_king_.col == b_queen_.col || abs(w_king_.row - b_queen_.row) == abs(w_king_.col - b_queen_.col));

  // check by black king
  check |= (abs(w_king_.row - b_king_.row) <= 1 && abs(w_king_.col - b_king_.col) <= 1);

  return check;
}

bool positionInCheck(int row, int col, Piece b_queen_, Piece b_king_) {
  bool check = row== b_queen_.row || col == b_queen_.col || abs(row - b_queen_.row) == abs(col - b_queen_.col));

  // check by black king
  check |= (abs(row - b_king_.row) <= 1 && abs(col - b_king_.col) <= 1);

  return check;
}

bool whiteKingCanEscapeCheck(Piece w_king_, Piece b_queen_, Piece b_king_) {
  std::vector<Piece> white_king_moves = generateKingMoves(w_king_);
  for (Piece w_king_move : white_king_moves) {
    if (positionInCheck(w_king_move.row, w_king_move.col, b_queen_, b_king_)) return false;
  }

  return true;
}

bool whiteKingCanEatQueen() {
  std::vector<Piece> white_king_moves = generateKingMoves(w_king);
  std::vector<Piece> black_king_moves = generateKingMoves(b_king);
  int row, col;
  bool canEat = false;
  for (Piece w_king_move : white_king_moves) {
    canEat = w_king_move.row == b_queen.row && w_king_move.col == b_queen.col;
    row = w_king_move.row;
    col = w_king_move.col;
    if (canEat) break;
  }

  for (Piece b_king_move : black_king_moves) {
    if (row == b_king_move.row && col == b_king_move.col) return false;
  }

  return canEat;
}

vod updateBoard(Piece boardTest[BOARD_SIZE][BOARD_SIZE], Piece newPiece) {
  Piece temp = boardTest[newPiece.row][newPiece.col];
  boardTest[newPiece.row][newPiece.col] = newPiece;
}

int evaluateBoard(Piece boardTest[BOARD_SIZE][BOARD_SIZE]) {
  Piece w_king_, b_king_, b_queen_;

  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++){
      if (boardTest[i][j].type == WHITE_KING) {
        w_king_ = boardTest[i][j];
      } else if (boardTest[i][j].type == BLACK_KING) {
        b_king_ = boardTest[i][j];
      } else if (boardTest[i][j].type == BLACK_QUEEN) {
        b_queen_ = boardTest[i][j];
      }
    }
  }

  if (positionInCheck(w_king_, b_queen_, b_king_)) {
    if (whiteKingCanEscapeCheck()) return 0;
    if (whiteKingCanEatQueen()) return -1;

    return 1;
  }

  return 0;
}

int minimax(Piece boardTest[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, bool maximizingPlayer) {
  int score = evaluateBoard(boardTest);
  if (score != 0) return score;

  if (maximizingPlayer) {
    int maxEval = -1000000;
    std::vector<Piece> white_king_moves = generateKingMoves(w_king);
    for (Piece w_king_move : white_king_moves) {

      int eval = minimax(alpha, beta, false);
      w_king = temp;
      maxEval = std::max(maxEval, eval);
      alpha = std::max(alpha, eval);
      if (beta <= alpha) break;
    }

    return maxEval;
  } else {
    int minEval = 1000000;
    std::vector<Piece> minimize_player_moves = generateQueenMoves();
    minimize_player_moves += generateKingMoves(b_king);
    for (Piece minimize_player_move : minimize_player_moves) {
      Piece temp = minimize_player_move;
      
      int eval = minimax(alpha, beta, true);
      b_queen = temp;
      minEval = std::min(minEval, eval);
      beta = std::min(beta, eval);
      if (beta <= alpha) break;
    }

    return minEval;
  }
}

int main() {
  initBoard();
  board[0][0] = initPiece(WHITE_KING, 0, 0);
  board[4][4] = initPiece(BLACK_KING, 4, 4);
  board[2][2] = initPiece(BLACK_QUEEN, 2, 2);
  w_king = board[0][0];
  b_king = board[4][4];
  b_queen = board[2][2];
  printBoard();
  int score = minimax(-1000000, 1000000, true);
  printf("Score: %d\n", score);
  if (score == 1) {
    printf("White wins!\n");
  } else if (score == -1) {
    printf("Black wins!\n");
  } else {
    printf("Draw!\n");
  }
  return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

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

Piece WKing, BKing, BQueen;
struct Move {
  int from_row;
  int from_col;
  int to_row;
  int to_col;
};

Piece board[BOARD_SIZE][BOARD_SIZE];

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
          cout << "WK ";
          break;
        case BLACK_QUEEN:
          cout << "BQ ";
          break;
        case BLACK_KING:
          cout << "BK ";
          break;
        case EMPTY:
          cout << ".  ";
          break;
      }
    }
    cout << "\n";
  }
  cout << "\n";
}

bool isValidPosition(int row, int col) {
  return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

bool isOccupied(int row, int col) {
  return board[row][col].type != EMPTY;
}

bool positionIsSafe(int row, int col){
  bool isSafe = (row == BQueen.row || col == BQueen.col || (row - BQueen.row == col - BQueen.col ));
  isSafe |= (row - BKing.row <= 1 && col - BKing.col <= 1);
}

vector<Move> getKingMoves(int row, int col, bool whiteKing) {
  vector<Move> moves;
  for (int i = row - 1; i <= row + 1; i++) {
    for (int j = col - 1; j <= col + 1; j++) {
      bool safe = isValidPosition(i,j) && !isOccupied(i,j);
      if (whiteKing){
        safe &= positionIsSafe(i,j);
      }
      if (safe) {
        moves.push_back({row, col, i, j});
      }
    }
  }
  return moves;
}

vector<Move> getQueenMoves(int row, int col) {
  vector<Move> moves;
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (i != row) {
      moves.push_back({row, col, i, col});
    }
    if (i != col) {
      moves.push_back({row, col, row, i});
    }
  }
  for (int i = -min(row, col); i < min(BOARD_SIZE - row, BOARD_SIZE - col); i++) {
    if (i != 0) {
      moves.push_back({row, col, row + i, col + i});
      moves.push_back({row, col, row - i, col + i});
    }
  }
  return moves;
}

vector<Move> getMoves(int row, int col) {
  switch (board[row][col].type) {
    case WHITE_KING:
    case BLACK_KING:
      return getKingMoves(row, col, board[row][col].type == WHITE_KING);
    case BLACK_QUEEN:
      return getQueenMoves(row, col);
    default:
      return {};
  }
}

bool KingCanMoveSafely(){
  int row = WKing.row;
  int col = WKing.col;
  for (int i = row-1; i <= row+1;i++){
    for (int j=col-1; j <= col+1;j++){
      if (isValidPosition(i,j) && !isOccupied(i,j) && positionIsSafe(i,j)){
        return true;
      }
    }
  }
  return false;
}

bool inCheck(){
  return positionIsSafe(WKing.row, WKing.col);
}

bool kingCanEatQueen(){
  int row = WKing.row;
  int col = WKing.col;
  for (int i = row-1; i <= row+1;i++){
    for (int j=col-1; j <= col+1;j++){
      if (isValidPosition(i,j) && !isOccupied(i,j) && i == BQueen.row && j == BQueen.col){
        return true;
      }
    }
  }
  return false;
}

int evaluateBoard(){
  if (inCheck())
  {
    // checkmate
    if (!KingCanMoveSafely() && !kingCanEatQueen()) return 1;

    // stalemate
    else if (kingCanEatQueen()) return -1;

    // game continue
    else if (KingCanMoveSafely()) return 0;
  }
}


int minimax(int depth, int alpha, int beta, bool maximizingPlayer) {
  if (depth == 0) {
    return evaluateBoard();
  }
  if (maximizingPlayer) {
    int maxEval = INT_MIN;
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[i][j].type == BLACK_QUEEN) {
          vector<Move> moves = getMoves(i, j);
          for (Move move : moves) {
            Piece capturedPiece = board[move.to_row][move.to_col];
            board[move.to_row][move.to_col] = board[move.from_row][move.from_col];
            board[move.from_row][move.from_col].type = EMPTY;
            int eval = minimax(depth - 1, alpha, beta, false);
            board[move.from_row][move.from_col] = board[move.to_row][move.to_col];
            board[move.to_row][move.to_col] = capturedPiece;
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) {
              break;
            }
          }
        }
      }
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[i][j].type == WHITE_KING) {
          vector<Move> moves = getMoves(i, j);
          for (Move move : moves) {
            Piece capturedPiece = board[move.to_row][move.to_col];
            board[move.to_row][move.to_col] = board[move.from_row][move.from_col];
            board[move.from_row][move.from_col].type = EMPTY;
            int eval = minimax(depth - 1, alpha, beta, true);
            board[move.from_row][move.from_col] = board[move.to_row][move.to_col];
            board[move.to_row][move.to_col] = capturedPiece;
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) {
              break;
            }
          }
        }
      }
    }
    return minEval;
  }
}

int main() {
  initBoard();

  int whiteKingRow, whiteKingCol, blackQueenRow, blackQueenCol, blackKingRow, blackKingCol;
  cout << "Enter the initial positions of the pieces (row, col)(0--4):" << "\n";
  cout << "White king: ";
  cin >> whiteKingRow >> whiteKingCol;
  cout << "Black queen: ";
  cin >> blackQueenRow >> blackQueenCol;
  cout << "Black king: ";
  cin >> blackKingRow >> blackKingCol;
  int mateMoves;
  cout << "Enter the number of moves to checkmate: ";
  cin >> mateMoves;

  WKing = initPiece(WHITE_KING, whiteKingRow, whiteKingCol);
  BQueen = initPiece(BLACK_QUEEN, blackQueenRow, blackQueenCol);
  BKing = initPiece(BLACK_KING, blackKingRow, blackKingCol);

  board[whiteKingRow][whiteKingCol] = WKing;
  board[blackQueenRow][blackQueenCol] = BQueen;
  board[blackKingRow][blackKingCol] = BKing;

  printBoard();

  int score = minimax(mateMoves, INT_MIN, INT_MAX, false);
  if (score > 0) {
    cout << "Black team can checkmate the white king ";
  } else {
    cout << "Black team cannot checkmate the white king ";
  }
  cout << "in " << mateMoves << " moves." << "\n";
  return 0;
}

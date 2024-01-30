#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

const int BOARD_SIZE = 5;
const int MAX_DEPTH = 7;

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

Piece initPiece(PieceType typein, int row, int col){
  Piece A;
  A.type=typein;
  A.row=row;
  A.col=col;
  return A;
}


bool positionInBoard(int row, int col) {
   return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

std::vector<Piece> generateQueenMoves(Piece w_king, Piece b_king, Piece b_queen) {
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
        if ((w_king.row == newRow && w_king.col == newCol) || (b_king.row == newRow && b_king.col == newCol)) break;
        Piece move = initPiece(BLACK_QUEEN, newRow, newCol);
        moves.push_back(move);
      } else {
        break;
      }
    }
  }
  
  return moves;  
}

bool whiteKingInCheck(Piece w_king, Piece b_queen, Piece b_king) {
  // check by black queen
  bool check = (w_king.row== b_queen.row || w_king.col == b_queen.col || abs(w_king.row - b_queen.row) == abs(w_king.col - b_queen.col));

  // check by black king
  check |= (abs(w_king.row - b_king.row) <= 1 && abs(w_king.col - b_king.col) <= 1);

  return check;
}

bool blackKingInCheck(Piece b_king, Piece w_king) {
  return (abs(w_king.row - b_king.row) <= 1 && abs(w_king.col - b_king.col) <= 1);
}

std::vector<Piece> generateWhiteKingMoves(Piece w_king, Piece b_king, Piece b_queen) {
  std::vector<Piece> moves;
  int row_direction[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
  int column_direction[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
  for (int i = 0; i < 8; i++) {
    int newRow = w_king.row + row_direction[i];
    int newCol = w_king.col + column_direction[i];
    if (positionInBoard(newRow, newCol)) {
      Piece move = initPiece(WHITE_KING, newRow, newCol);
      if (!whiteKingInCheck(move, b_queen, b_king)) moves.push_back(move);
    }
  }

  return moves;
}

std::vector<Piece> generateBlackKingMoves(Piece w_king, Piece b_king, Piece b_queen) {
  std::vector<Piece> moves;
  int row_direction[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
  int column_direction[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
  for (int i = 0; i < 8; i++) {
    int newRow = b_king.row + row_direction[i];
    int newCol = b_king.col + column_direction[i];
    if (positionInBoard(newRow, newCol)) {
      if (!(newRow == b_queen.row && newCol == b_queen.col)) {
        Piece move = initPiece(BLACK_KING, newRow, newCol);
        if (!blackKingInCheck(move, w_king)) moves.push_back(move);
      }
    }
  }

  return moves;
}

bool whiteKingCanEscapeCheck(Piece w_king, Piece b_queen, Piece b_king) {
  std::vector<Piece> white_king_moves = generateWhiteKingMoves(w_king, b_king, b_queen);

  for (Piece white_king_move : white_king_moves) {
    if (!whiteKingInCheck(white_king_move, b_queen, b_king)) return true;
  }

  return false;
}

bool blackQueenIsProtected(Piece b_queen, Piece b_king){
  return (abs(b_queen.row - b_king.row) <= 1 && abs(b_queen.col - b_king.col) <= 1);

}
bool whiteKingCanEatBlackQueen(Piece w_king, Piece b_queen, Piece b_king) {
  if (blackQueenIsProtected(b_queen, b_king)) return false;

  return (abs(b_queen.row - w_king.row) <= 1 && abs(b_queen.col - w_king.col) <= 1);
}

int evaluateBoard(Piece w_king, Piece b_king, Piece b_queen) {
  if (whiteKingInCheck(w_king, b_queen, b_king)) {
    if (whiteKingCanEatBlackQueen(w_king, b_queen, b_king)) return -1;
    if (whiteKingCanEscapeCheck(w_king, b_queen, b_king)) return 0;

    return 1;
  }

  return 0;
}

int getMinimaxScore(Piece w_king, Piece b_king, Piece b_queen) {
  if (whiteKingInCheck(w_king, b_queen, b_king)) {
    if (whiteKingCanEatBlackQueen(w_king, b_queen, b_king)) return -(MAX_DEPTH + 1);
    if (whiteKingCanEscapeCheck(w_king, b_queen, b_king)) return 0;

    return MAX_DEPTH + 1;
  }

  return 0;
}

void printPiecesPosition(Piece w_king, Piece b_king, Piece b_queen) {
  for (int i=0; i<BOARD_SIZE; i++) {
    for (int j=0; j<BOARD_SIZE; j++) {
      if (w_king.row == i && w_king.col == j) {
        printf("WK ");
      } else if (b_king.row == i && b_king.col == j) {
        printf("BK ");
      } else if (b_queen.row == i && b_queen.col == j) {
        printf("BQ ");
      } else {
        printf(" . ");
      }
    }
    printf("\n");
  }
  printf("\n");
}

int minimax(Piece w_king, Piece b_king, Piece b_queen, int depth, int alpha, int beta, bool maximizingPlayer) {
  int score = getMinimaxScore(w_king, b_king, b_queen);
  if (score == MAX_DEPTH + 1) return score - depth;
  if (score == -(MAX_DEPTH + 1)) return score + depth;
  if (depth == MAX_DEPTH) return 0; 

  if (maximizingPlayer) {
    int maxEval = -1000000;
    
    std::vector<Piece> moves = generateQueenMoves(w_king, b_king, b_queen);
    
    std::vector<Piece> b_king_moves = generateBlackKingMoves(w_king, b_king, b_queen);
    moves.insert(moves.end(), b_king_moves.begin(), b_king_moves.end());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(moves.begin(), moves.end(), gen);
    for (Piece move : moves) {
      int eval;
      if (move.type == BLACK_KING) {
        eval = minimax(w_king, move, b_queen, depth+1, alpha, beta, false);
      } else if (move.type == BLACK_QUEEN) {
        eval = minimax(w_king, b_king, move, depth+1, alpha, beta, false);
      }
      maxEval = std::max(maxEval, eval);
      alpha = std::max(alpha, eval);
      bool prune = beta <= alpha;
      if (prune) break;
    }

    return maxEval;
  } else {
    int minEval = 1000000;
    
    std::vector<Piece> moves = generateWhiteKingMoves(w_king, b_king, b_queen);
    if (moves.size() == 0) return 0;
    for (Piece move : moves) {
      int eval = minimax(move, b_king, b_queen, depth+1, alpha, beta, true);
      minEval = std::min(minEval, eval);
      beta = std::min(beta, eval);
      bool prune = beta <= alpha;
      if (prune) break;
    }

    return minEval;
  }
}

Piece getBestBlackMove(Piece w_king, Piece b_king, Piece b_queen) {
  int best_score = -1000000;
  Piece best_move;
  std::vector<Piece> moves = generateQueenMoves(w_king, b_king, b_queen);
  std::vector<Piece> b_king_moves = generateBlackKingMoves(w_king, b_king, b_queen);
  moves.insert(moves.end(), b_king_moves.begin(), b_king_moves.end());
  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(moves.begin(), moves.end(), gen);
  for (Piece move : moves) {
    int score;
    if (move.type == BLACK_KING) {
      // printPiecesPosition(w_king, move, b_queen);
      score = minimax(w_king, move, b_queen, 0, -1000000, 1000000, false);
    } else if (move.type == BLACK_QUEEN) {
      // printPiecesPosition(w_king, b_king, move);
      score = minimax(w_king, b_king, move, 0, -1000000, 1000000, false);
    }
    if (score > best_score) {
      best_score = score;
      best_move = move;
    }
  }

  return best_move;
}

Piece getBestWhiteMove(Piece w_king, Piece b_king, Piece b_queen) {
  int best_score = 1000000;
  Piece best_move;
  std::vector<Piece> moves = generateWhiteKingMoves(w_king, b_king, b_queen);
  if (moves.size() == 0) {
    printf("Stalemate!\n");
    exit(0);
  }
  if (moves.size() == 1) {
    return moves[0];
  }

  for (Piece move : moves) {
    int score = minimax(move, b_king, b_queen, 0, -1000000, 1000000, true);
    if (score < best_score) {
      best_score = score;
      best_move = move;
    }
  }

  return best_move;
}


int main(){
  int row, col;
  printf("Enter white king position (row, col): ");
  scanf("%d %d", &row, &col);
  Piece white_king = initPiece(WHITE_KING, row, col);
  printf("Enter black king position (row, col): ");
  scanf("%d %d", &row, &col);
  Piece black_king = initPiece(BLACK_KING, row, col);
  printf("Enter black queen position (row, col): ");
  scanf("%d %d", &row, &col);
  Piece black_queen = initPiece(BLACK_QUEEN, row, col);
  printf("Initial board:\n");
  printPiecesPosition(white_king, black_king, black_queen);
  if (evaluateBoard(white_king, black_king, black_queen) == -1) {
    printf("Draw!\n");
    return 0;
  } else if (evaluateBoard(white_king, black_king, black_queen) == 1) {
    printf("Checkmate! Black wins!\n");
    return 0;
  }
  while(true) {
    Piece white_move = getBestWhiteMove(white_king, black_king, black_queen);
    white_king = white_move;
    printf("White king moves to (%d, %d)\n", white_move.row, white_move.col);
    printPiecesPosition(white_king, black_king, black_queen);
    if (evaluateBoard(white_king, black_king, black_queen) == -1) {
      printf("Draw!\n");
      break;
    }
    Piece black_move = getBestBlackMove(white_king, black_king, black_queen);
    if (black_move.type == BLACK_KING) {
      black_king = black_move;
      printf("Black king moves to (%d, %d)\n", black_move.row, black_move.col);
    } else if (black_move.type == BLACK_QUEEN) {
      black_queen = black_move;
      printf("Black queen moves to (%d, %d)\n", black_move.row, black_move.col);
    }

    printPiecesPosition(white_king, black_king, black_queen);
    if (evaluateBoard(white_king, black_king, black_queen) == 1 ) {
      printf("Checkmate! Black wins!\n");
      break;
    }
  }
  return 0;

}

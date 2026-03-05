#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <cctype>
#include <iostream>
#include <set>
#include <string>

using namespace std;

class Piece {

public:
  enum PieceColor { White = 0b01000, Black = 0b10000 };

  enum PieceType {
    None = 0b00000,
    Pawn = 0b00001,
    Bishop = 0b00010,
    Knight = 0b00011,
    Rook = 0b00100,
    Queen = 0b00101,
    King = 0b00110
  };

private:
  string pieceNames[7] = {"",     "pawn",  "bishop", "knight",
                          "rook", "queen", "king"};

  PieceColor color;
  PieceType type;

  static const int TypeMask = 0b00111;
  static const int ColorMask = 0b11000;

public:
  Piece(int code) {
    this->color = Piece::getPieceColor(code);
    this->type = Piece::getPieceType(code);
  }

  static int mapFenToPiece(char piece) {

    PieceColor color = islower(piece) ? PieceColor::Black : PieceColor::White;
    PieceType type;

    piece = std::tolower(piece);

    switch (piece) {
    case 'p':
      type = PieceType::Pawn;
      break;
    case 'r':
      type = PieceType::Rook;
      break;
    case 'b':
      type = PieceType::Bishop;
      break;
    case 'q':
      type = PieceType::Queen;
      break;
    case 'k':
      type = PieceType::King;
      break;
    case 'n':
      type = PieceType::Knight;
      break;
    }

    return type | color;
  }

  static PieceType getPieceType(int piece) {
    return static_cast<PieceType>(piece & TypeMask);
  }

  static PieceColor getPieceColor(int piece) {
    return static_cast<PieceColor>(piece & ColorMask);
  }

  static string getPiecePath(int piece) {
    int color = getPieceColor(piece);
    PieceType type = getPieceType(piece);

    string path = "assets/pieces/";

    if (color == PieceColor::Black) {
      path.append("black");
    } else {
      path.append("white");
    }

    path.append("-");

    switch (type) {
    case None:
      break;
    case Pawn:
      path.append("pawn");
      break;
    case Bishop:
      path.append("bishop");
      break;
    case Knight:
      path.append("knight");
      break;
    case Rook:
      path.append("rook");
      break;
    case Queen:
      path.append("queen");
      break;
    case King:
      path.append("king");
      break;
    }

    path.append(".png");

    return path;
  }

  static void renderPiece(SDL_Renderer *renderer, int x, int y, int size,
                          string path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());

    if (texture == nullptr) {
      cout << IMG_GetError() << endl;

      return;
    }

    SDL_Rect rect = {x, y, size, size};

    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_RenderPresent(renderer);
  }
};

class Board {
private:
  const int SQUARES = 8;

  // defining square colors!
  const SDL_Color black = {173, 95, 35, 255};
  const SDL_Color white = {251, 245, 222, 255};

  int board[8][8];

public:
  Board(string fen) { fenStringParser(fen); }

  void fenStringParser(string fen) {
    // a / symbol acts a seperator btw rows
    // small alphabets -> black
    // capital -> white
    // general notation -> xPiecey

    // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR

    set<char> pieces = {'p', 'r', 'b', 'q', 'k', 'n'};

    int row = 0, col = 0;

    for (char c : fen) {

      if (c == '/') {
        row++;
        col = 0;
        continue;
      }

      char baseSymbol = std::tolower(c);

      if (pieces.count(baseSymbol)) {
        int p = Piece::mapFenToPiece(c);

        board[row][col] = p;

        col++;
      } else {
        col += baseSymbol - '0';
      }
    }
  }

  void renderBoard(SDL_Renderer *renderer, int w, int h) {

    // we need to some margin to adjust (10% margin)
    // we can say that our board will be atmost 80% the size of the screen
    // 10% margin each size
    int boardWidth = w * 0.9;
    int boardHeight = h * 0.9;

    // the issue is these may not work perfectly for 8 pieces so we need to find
    // since we're making a well, chess board, duh
    // we need to final the closest lower multiple of 8
    // to get that we can divide both width and height by 8 and subtract the
    // remainder to the biggest smaller multiple of 8
    boardWidth = boardWidth - boardWidth % 8;
    boardHeight = boardHeight - boardHeight % 8;

    // now that we have our board size, we can finally calculate the checker
    // size but wait, we fucked up, the board has to be square, this one doesn't
    // guarantee a square board, so we'll need to pick the min of these two
    boardWidth = min(boardHeight, boardHeight);
    boardHeight = boardWidth;

    int pl = 0.05 * w;
    int pt = 0.05 * h;

    // well the above calc doesn't center the board
    // to center we can prolly get half of what's remaining off the window size
    pl = (w - boardWidth) / 2;
    pt = (h - boardHeight) / 2;

    // perfect, much cleaner
    // Okay now that we have our final board size, we can prolly try rendering
    // an empty board

    SDL_Rect rect = {pl, pt, boardWidth, boardHeight};

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);

    // finally! now this we just 1 square, we need 64 smaller squares
    // there is another issue, we need to define 2 colors, not just one
    // I'll just google a perfect pair of colors in rgba format cuz SDL2 doesn't
    // support hex colors T_T, to nothingness goes my webd skills

    // first off we start by defining size of each block, which will be well
    // board size / 8 because we've alrdy made sure that board is a multiple of
    // 8
    int blockHeight = boardWidth / 8;
    int blockWidth = blockHeight;

    for (int i = 0; i < SQUARES; i++) {

      // first off we start by defining the position, size is same for all
      // now comes the nicer part, i'll have to define position of each block,
      // now there are 8 blocks in each row and there are 8 rows, that'll
      // require two loops, one for column and one for rows each loop iterating
      // 8 times defining how much distance we'll need from

      for (int j = 0; j < SQUARES; j++) {
        // the top left. Oh! I almost forgot i'll have to consider the margin I
        // initially gave to the board itself.
        // that'll add the size of each block times number of blocks that we're
        // before this block, Oh my! I'm gonna get confused thinking about rows
        // and columns here
        int blockY = pt + i * blockWidth;
        int blockX = pl + j * blockHeight;

        SDL_Rect rect = {blockX, blockY, blockWidth, blockHeight};

        // well its better than the if block
        SDL_Color currentBlockColor = (i + j) % 2 ? black : white;

        SDL_SetRenderDrawColor(renderer, currentBlockColor.r,
                               currentBlockColor.g, currentBlockColor.b, 255);

        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
        // well it worked the first try :()

        if (board[i][j] == 0)
          continue;

        string path = Piece::getPiecePath(board[i][j]);

        Piece::renderPiece(renderer, blockX, blockY, blockHeight, path);
      }
    }
  }

  void printBoard() {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        cout << board[i][j] << " ";
      }

      cout << endl;
    }
  }
};

class Game {
private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  Board *board;
  int width;
  int height;

public:
  Game(string fen, int width, int height) {
    board = new Board(fen);
    this->width = width;
    this->height = height;
  }

  bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      cout << SDL_GetError() << endl;

      return false;
    }

    window = SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == nullptr) {
      cout << SDL_GetError() << endl;

      return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_WINDOW_SHOWN);

    if (renderer == nullptr) {
      cout << SDL_GetError() << endl;

      SDL_DestroyWindow(window);

      return false;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
      cout << IMG_GetError() << endl;

      SDL_DestroyWindow(window);
      SDL_DestroyRenderer(renderer);

      return false;
    }

    return true;
  }

  void render() {
    int w, h;

    // get the size of window to determine the board size
    SDL_GetWindowSize(window, &w, &h);

    board->renderBoard(renderer, w, h);
  }

  void eventLoop() {
    bool running = true;

    while (running) {
      SDL_Event e;

      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          running = false;
          break;
        }
      }
    }
  }

  void close() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    SDL_Quit();
  }
};

int main() {

  int HEIGHT = 720;
  int WIDTH = 720;

  Game *game =
      new Game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", WIDTH, HEIGHT);

  if (!game->init()) {
    cout << "Something went wrong" << endl;
    return 1;
  };

  game->render();

  game->eventLoop();

  game->close();

  return 0;
}
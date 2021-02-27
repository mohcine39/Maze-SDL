#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#define NMAX 40
#define MMAX 40
int Maze[NMAX][MMAX];
int N = NMAX - 1, M = MMAX - 1;
const int inf = 1e9;
int r[NMAX * MMAX + 1];
int sz[NMAX * MMAX + 1];

void zeros() {
  int i, j;
  for (i = 0 ;i < N; i++) {
    for (j = 0; j < M; j++) {
     Maze[i][j] = 0;
    }
  }
}
void show() {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      printf("%2d ", Maze[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

//------------------------- Defining the Disjoint-set data structure----------//
int find(int a) {
  if (r[a] == a) return a;
  return find(r[a]);
}
void _union(int a, int b) {
  int ra = find(a), rb = find(b);
  if (ra == rb) return;
  if (sz[ra] > sz[rb]) sz[ra] += sz[rb], r[rb] = ra;
  else sz[rb] += sz[ra], r[ra] = rb;
}

void merge(int x, int y) {
  int i = rand() % 4;
  switch (i) {
  case 0:
    // right
    if (y + 2 < M - 1 && find(x * N + y) !=find(x * N + y + 2)) {
      Maze[x][y + 1]++;
      _union(x * N + y, x * N + y + 2);
    }
    break;
  case 1:
    // left
    if (y - 2 > 0 && find(x * N + y) != find(x * N + y - 2)) {
      Maze[x][y - 1]++;
      _union(x * N + y, x * N + y - 2);
    }
    break;
  case 2:
    // up
    if (x - 2 > 0 && find(x * N + y) != find((x - 2) * N + y)) {
      Maze[x - 1][y]++;
      _union(x * N + y, (x - 2) * N + y);
    }
    break;
  case 3:
    // down
    if (x + 2 < N - 1 && find(x * N + y) != find((x + 2) * N + y)) {
      Maze[x + 1][y]++;
      _union(x * N + y, (x + 2) * N + y);
    }
    break;
  }
}

void generate() {
  int i, j, x, y;
  int cells = 0;
  zeros();
  for (i = 1; i < N; i += 2) {
    for (j = 1; j < M; j += 2) {
      Maze[i][j] = 1;
      cells++;
      r[i * N + j] = i * N + j;
      sz[i * N + j] = 1;
    }
  }
  while (sz[find(N + 1)] < cells) { // There is a path from entry to all cells if sz == cells,
    x = 2 * (rand() % (N / 2)) + 1;
    y = 2 * (rand() % (M / 2)) + 1;
    merge(x, y);
  }
  Maze[N - 2][0] = Maze[1][M - 1] = 1;
}

//--------------Maze solver using DFS---------//
void DFS(int x, int y, int len) {
  if (!(x >= 0 && x < N && y >= 0 && y < M && Maze[x][y] == 1)) return;
  Maze[x][y] = len;
  DFS(x + 1, y, len + 1);
  DFS(x, y + 1, len + 1);
  DFS(x - 1, y, len + 1);
  DFS(x, y - 1, len + 1);
}

//-----------------------Show maze using SDL ----------------------

void print(SDL_Renderer * rend, SDL_Rect rectangle) {
  int i, j;
  SDL_SetRenderDrawColor(rend, 134, 47, 47, SDL_ALPHA_OPAQUE);
  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      rectangle.x = j * 20;
      rectangle.y = i * 20;
      rectangle.w = 20;
      rectangle.h = 20;
      if (Maze[i][j] == 0) {
        SDL_RenderFillRect(rend, & rectangle);
      }
    }
  }
  SDL_RenderPresent(rend);
}
//-------------------- Clear maze's map ---------------//
void clear(SDL_Renderer * renderer, SDL_Rect rectangle) {
  int i, j;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      rectangle.x = 20 * i;
      rectangle.y = 20 * j;
      rectangle.w = 20;
      rectangle.h = 20;
      if (Maze[j][i] > 0) {
        Maze[j][i] = 1;
        SDL_RenderFillRect(renderer, & rectangle);
        SDL_RenderPresent(renderer);

      }
    }
  }
}
//--------------------------------Show solution using SDL----------------------------//

void solution(int i, int j, SDL_Renderer * renderer, SDL_Rect rectangle) {
  int up, down, right, left, X, Y, a, b;
  DFS(j, i, 2);
  SDL_SetRenderDrawColor(renderer, 78, 135, 35, SDL_ALPHA_OPAQUE);
  a = -1;
  b = N - 2;
  rectangle.x = 20 * a;
  rectangle.y = 20 * b;
  rectangle.w = 20;
  rectangle.h = 20;
  while (a != i || b != j) {
    up = (b - 1 > 0 && Maze[b - 1][a] ? Maze[b - 1][a] : inf);
    left = (a - 1 > 0 && Maze[b][a - 1] ? Maze[b][a - 1] : inf);
    right = (a + 1 < N && Maze[b][a + 1] ? Maze[b][a + 1] : inf);
    down = (b + 1 < N && Maze[b + 1][a] ? Maze[b + 1][a] : inf);
    if (left < right && left < up && left < down) { //left
      X = b;
      Y = a - 1;
    }
    if (right < left && right < up && right < down) { //right
      X = b;
      Y = a + 1;
    }
    if (down < right && down < up && down < left) { //down
      X = b + 1;
      Y = a;
    }
    if (up < right && up < left && up < down) { //up
      X = b - 1;
      Y = a;
    }
    Maze[X][Y] = inf; // (X,Y) visited
    a = Y, b = X;
    rectangle.x = 20 * Y;
    rectangle.y = 20 * X;
    rectangle.w = 20;
    rectangle.h = 20;
    SDL_RenderFillRect(renderer, & rectangle);
    SDL_Delay(100);
    SDL_RenderPresent(renderer);
  }
  clear(renderer, rectangle);
}





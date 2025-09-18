// file: tetris_minimal_full.c
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define TILE 24
#define WIN_W (WIDTH * TILE)
#define WIN_H (HEIGHT * TILE)

int field[HEIGHT][WIDTH] = {0};

// I と O ブロック
int pieces[2][4][4] = {
    {   // I
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    {   // O
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    }
};

int current = 0, px, py;
SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;

// 衝突判定
int collision(int nx, int ny, int shape[4][4]) {
    for (int i=0;i<4;i++) for (int j=0;j<4;j++) {
        if (shape[i][j]) {
            int x = nx + j;
            int y = ny + i;
            if (x<0 || x>=WIDTH || y>=HEIGHT) return 1;
            if (y>=0 && field[y][x]) return 1;
        }
    }
    return 0;
}

// 固定
void place(int nx, int ny, int shape[4][4]) {
    for (int i=0;i<4;i++) for (int j=0;j<4;j++) {
        if (shape[i][j]) {
            int x = nx + j;
            int y = ny + i;
            if (y>=0 && y<HEIGHT && x>=0 && x<WIDTH) field[y][x] = 1;
        }
    }
}

// 4x4 回転
void rotate(int shape[4][4]) {
    int tmp[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            tmp[j][3-i] = shape[i][j];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            shape[i][j] = tmp[i][j];
}

// ライン消去
void clear_lines() {
    for (int y=HEIGHT-1; y>=0; y--) {
        int full = 1;
        for (int x=0;x<WIDTH;x++) {
            if (field[y][x]==0) { full = 0; break; }
        }
        if (full) {
            // 上の行を下げる
            for (int yy=y; yy>0; yy--) {
                for(int x=0;x<WIDTH;x++) field[yy][x] = field[yy-1][x];
            }
            for(int x=0;x<WIDTH;x++) field[0][x] = 0;
            y++; // 同じ行を再チェック
        }
    }
}

// 描画
void draw() {
    SDL_SetRenderDrawColor(ren, 0,0,0,255);
    SDL_RenderClear(ren);

    // 盤面
    for(int y=0;y<HEIGHT;y++){
        for(int x=0;x<WIDTH;x++){
            if(field[y][x]) SDL_SetRenderDrawColor(ren,200,50,50,255);
            else SDL_SetRenderDrawColor(ren,30,30,30,255);
            SDL_Rect r = {x*TILE,y*TILE,TILE-1,TILE-1};
            SDL_RenderFillRect(ren,&r);
        }
    }

    // 現在ピース
    for(int i=0;i<4;i++) for(int j=0;j<4;j++){
        if(pieces[current][i][j]){
            int x=px+j;
            int y=py+i;
            if(y>=0 && y<HEIGHT && x>=0 && x<WIDTH){
                SDL_SetRenderDrawColor(ren,50,200,50,255);
                SDL_Rect r = {x*TILE,y*TILE,TILE-1,TILE-1};
                SDL_RenderFillRect(ren,&r);
            }
        }
    }

    SDL_RenderPresent(ren);
}

int main() {
    srand((unsigned)time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        fprintf(stderr,"SDL_Init Error: %s\n",SDL_GetError());
        return 1;
    }

    win = SDL_CreateWindow("Minimal Tetris Full", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, 0);
    ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    current = rand()%2;
    px = WIDTH/2-2;
    py = -1;

    Uint32 last = SDL_GetTicks();
    int running=1;

    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=0;
            if(e.type==SDL_KEYDOWN){
                if(e.key.keysym.sym==SDLK_LEFT && !collision(px-1,py,pieces[current])) px--;
                if(e.key.keysym.sym==SDLK_RIGHT && !collision(px+1,py,pieces[current])) px++;
                if(e.key.keysym.sym==SDLK_DOWN && !collision(px,py+1,pieces[current])) py++;
                if(e.key.keysym.sym==SDLK_UP){
                    int tmp[4][4];
                    for(int i=0;i<4;i++) for(int j=0;j<4;j++) tmp[i][j]=pieces[current][i][j];
                    rotate(pieces[current]);
                    if(collision(px,py,pieces[current])){
                        for(int i=0;i<4;i++) for(int j=0;j<4;j++) pieces[current][i][j]=tmp[i][j];
                    }
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if(now - last > 500){
            last=now;
            if(!collision(px,py+1,pieces[current])) py++;
            else{
                place(px,py,pieces[current]);
                clear_lines();
                current = rand()%2;
                px = WIDTH/2-2;
                py = -1;
                if(collision(px,py,pieces[current])){
                    printf("Game Over\n");
                    running=0;
                }
            }
        }

        draw();
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

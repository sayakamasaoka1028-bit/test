# C言語テトリスゲーム

このリポジトリは、C言語と SDL2 を使ったテトリスゲームです。  
学習・実験用に作成したものです。

## ファイル一覧

- `テトリス_sdl.c`  
  SDL2 を使った最小サンプル。ウィンドウ表示と簡易ブロック落下。
- `テトリス_ミニマル.c`  
  回転や衝突判定、ライン消去の簡易ロジックを追加。
- `テトリス_ミニマル_フル.c`  
  7種類のピース、ライン消去、スコア計算など本格的なロジックを実装。

## 必要環境

- C言語コンパイラ（gcc）
- SDL2 開発ライブラリ（Ubuntu 例: `sudo apt install libsdl2-dev libsdl2-mixer-dev`）

## 実行方法（例: Ubuntu）

```bash
# コンパイル
gcc テトリス_sdl.c -o tetris_sdl `sdl2-config --cflags --libs`

# 実行
./tetris_sdl

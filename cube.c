#include <stdio.h>

/* cubeという構造体を定義 */
typedef struct {
    int cp[8]; /* Corner Permutation コーナーパーツの位置 */
    int co[8]; /* Corner Orientation コーナーパーツの向き */
    int ep[12]; /* Edge Permutation エッジパーツの位置 */
    int eo[12]; /* Edge Orientation エッジパーツの向き */
} cube;

/* プロトタイプ宣言 */
void apply_move(cube*, cube);

/* メイン関数 */
int main(void) {
    printf("hello, world\n");
    return 0;
}

/* キューブを動かす関数 */
void apply_move(cube *state, cube move) {
    cube new_state;
    for (int i = 0; i < 8; i++) {
        new_state.cp[i] = state->cp[move.cp[i]];
        new_state.co[i] = (state->co[move.cp[i]] + move.co[i]) % 3;
    }
    for (int i = 0; i < 12; i++) {
        new_state.ep[i] = state->ep[move.ep[i]];
        new_state.eo[i] = (state->eo[move.ep[i]] + move.eo[i]) % 2;
    }
    *state = new_state;
    return;
}

#include <stdio.h>

/* cubeという構造体を定義 */
typedef struct {
    int cp[8]; /* Corner Permutation コーナーパーツの位置 */
    int co[8]; /* Corner Orientation コーナーパーツの向き */
    int ep[12]; /* Edge Permutation エッジパーツの位置 */
    int eo[12]; /* Edge Orientation エッジパーツの向き */
} cube;

/* メイン関数 */
int main(void) {
    printf("hello, world\n");
    return 0;
}

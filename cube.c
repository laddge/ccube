#include <stdio.h>

/* cubeという構造体を定義 */
typedef struct {
    int cp[8]; /* Corner Permutation コーナーパーツの位置 */
    int co[8]; /* Corner Orientation コーナーパーツの向き */
    int ep[12]; /* Edge Permutation エッジパーツの位置 */
    int eo[12]; /* Edge Orientation エッジパーツの向き */
} cube;

/* 解法を格納する構造体を定義 */
typedef struct {
    int array[64]; /* 解法の配列 */
    int len; /* 解法の長さ */
    int solved; /* 完成したか */
} sol;

/* プロトタイプ宣言 */
void apply_move(cube*, int);
int is_solved(cube*);
int prune(int, cube*);
int cp2i(cube);
int co2i(cube);
int ud_ep2i(cube);
int e_ep2i(cube);
int eo2i(cube);
void i2cp(int, cube*);
void i2co(int, cube*);
void i2ud_ep(int, cube*);
void i2e_ep(int, cube*);
void i2eo(int, cube*);
int search(cube*, int, sol*);
void solve(cube*, sol*);
void print_state(cube);

/* メイン関数 */
int main(void) {
    printf("hello, world\n");
    return 0;
}

/* キューブを動かす関数 */
void apply_move(cube *state, int movei) {
    /* 操作の一覧 */
    cube moves[] = {
        /* U */
        {{3, 0, 1, 2, 4, 5, 6, 7},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 7, 4, 5, 6, 8, 9, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* U2 */
        {{2, 3, 0, 1, 4, 5, 6, 7},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 6, 7, 4, 5, 8, 9, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* U' */
        {{1, 2, 3, 0, 4, 5, 6, 7},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 5, 6, 7, 4, 8, 9, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* D */
        {{0, 1, 2, 3, 5, 6, 7, 4},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 8},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* D2 */
        {{0, 1, 2, 3, 6, 7, 4, 5},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 8, 9},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* D' */
        {{0, 1, 2, 3, 7, 4, 5, 6},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 2, 3, 4, 5, 6, 7, 11, 8, 9, 10},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* L */
        {{4, 1, 2, 0, 7, 5, 6, 3},
        {2, 0, 0, 1, 1, 0, 0, 2},
        {11, 1, 2, 7, 4, 5, 6, 0, 8, 9, 10, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* L2 */
        {{7, 1, 2, 4, 3, 5, 6, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {3, 1, 2, 0, 4, 5, 6, 11, 8, 9, 10, 7},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* L' */
        {{3, 1, 2, 7, 0, 5, 6, 4},
        {2, 0, 0, 1, 1, 0, 0, 2},
        {7, 1, 2, 11, 4, 5, 6, 3, 8, 9, 10, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* R */
        {{0, 2, 6, 3, 4, 1, 5, 7},
        {0, 1, 2, 0, 0, 2, 1, 0},
        {0, 5, 9, 3, 4, 2, 6, 7, 8, 1, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* R2 */
        {{0, 6, 5, 3, 4, 2, 1, 7},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 2, 1, 3, 4, 9, 6, 7, 8, 5, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* R' */
        {{0, 5, 1, 3, 4, 6, 2, 7},
        {0, 1, 2, 0, 0, 2, 1, 0},
        {0, 9, 5, 3, 4, 1, 6, 7, 8, 2, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* F */
        {{0, 1, 3, 7, 4, 5, 2, 6},
        {0, 0, 1, 2, 0, 0, 2, 1},
        {0, 1, 6, 10, 4, 5, 3, 7, 8, 9, 2, 11},
        {0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0}},

        /* F2 */
        {{0, 1, 7, 6, 4, 5, 3, 2},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 3, 2, 4, 5, 10, 7, 8, 9, 6, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* F' */
        {{0, 1, 6, 2, 4, 5, 7, 3},
        {0, 0, 1, 2, 0, 0, 2, 1},
        {0, 1, 10, 6, 4, 5, 2, 7, 8, 9, 3, 11},
        {0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0}},

        /* B */
        {{1, 5, 2, 3, 0, 4, 6, 7},
        {1, 2, 0, 0, 2, 1, 0, 0},
        {4, 8, 2, 3, 1, 5, 6, 7, 0, 9, 10, 11},
        {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}},

        /* B2 */
        {{5, 4, 2, 3, 1, 0, 6, 7},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 2, 3, 8, 5, 6, 7, 4, 9, 10, 11},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},

        /* B' */
        {{4, 0, 2, 3, 5, 1, 6, 7},
        {1, 2, 0, 0, 2, 1, 0, 0},
        {8, 4, 2, 3, 0, 5, 6, 7, 1, 9, 10, 11},
        {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}},
    };
    cube new_state;
    cube move = moves[movei];
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

/* 完成したか判定する関数 */
int is_solved(cube *state) {
    for (int i = 0; i < 8; i++) {
        if (state->cp[i] != i) return 0;
        if (state->co[i] != 0) return 0;
    }
    for (int i = 0; i < 12; i++) {
        if (state->ep[i] != i) return 0;
        if (state->eo[i] != 0) return 0;
    }
    return 1;
}

/* 枝刈りするための関数 */
int prune(int depth, cube *state) {
    /* 揃っているコーナーの数を数える */
    int solved_corners = 0;
    for (int i = 0; i < 8; i++) {
        solved_corners += state->cp[i] == i && state->co[i] == 0;
    }
    /* 揃っているエッジの数を数える */
    int solved_edges = 0;
    for (int i = 0; i < 12; i++) {
        solved_edges += state->ep[i] == i && state->eo[i] == 0;
    }
    /* それ以上探索しても無意味な場合、1を返す */
    if (depth == 1 && (solved_corners < 4 || solved_edges < 8)) {
        return 1;
    }
    if (depth == 2 && solved_edges < 4) {
        return 1;
    }
    if (depth == 3 && solved_edges < 2) {
        return 1;
    }
    return 0;
}

/* ベクトルをインデックスに変換 */
int cp2i(cube state) {
    int index = 0;
    for (int i = 0; i < 8; i++) {
        index *= 8 - i;
        for (int j = i + 1; j < 8; j++) {
            if (state.cp[i] > state.cp[j]) {
                index++;
            }
        }
    }
    return index;
}

int co2i(cube state) {
    int index = 0;
    for (int i = 0; i < 7; i++) {
        index *= 3;
        index += state.co[i];
    }
    return index;
}

int ud_ep2i(cube state) {
    int index = 0;
    for (int i = 0; i < 8; i++) {
        index *= 8 - i;
        for (int j = i + 1; j < 8; j++) {
            if (state.ep[i + 4] > state.ep[j + 4]) {
                index++;
            }
        }
    }
    return index;
}

int e_ep2i(cube state) {
    int index = 0;
    for (int i = 0; i < 4; i++) {
        index *= 4 - i;
        for (int j = i + 1; j < 4; j++) {
            if (state.ep[i] > state.ep[j]) {
                index++;
            }
        }
    }
    return index;
}

int eo2i(cube state) {
    int index = 0;
    for (int i = 0; i < 7; i++) {
        index *= 3;
        index += state.eo[i];
    }
    return index;
}

/* インデックスをベクトルに変換 */
void i2cp(int index, cube *state) {
    for (int i = 6; i > -1; i--) {
        state->cp[i] = index % (8 - i);
        index /= 8 - i;
        for (int j = i + 1; j < 8; j++) {
            if (state->cp[j] >= state->cp[i]) {
                state->cp[j]++;
            }
        }
    }
}

void i2co(int index, cube *state) {
    int sum_co = 0;
    for (int i = 6; i > -1; i--) {
        state->co[i] = index % 3;
        index /= 3;
        sum_co += state->co[i];
    }
    state->co[7] = (3 - sum_co % 3) % 3;
}

void i2ud_ep(int index, cube *state) {
    for (int i = 6; i > -1; i--) {
        state->ep[i + 4] = index % (8 - i);
        index /= 8 - i;
        for (int j = i + 1; j < 8; j++) {
            if (state->ep[j + 4] >= state->ep[i + 4]) {
                state->ep[j + 4]++;
            }
        }
    }
}

void i2e_ep(int index, cube *state) {
    for (int i = 2; i > -1; i--) {
        state->ep[i] = index % (4 - i);
        index /= 4 - i;
        for (int j = i + 1; j < 4; j++) {
            if (state->ep[j] >= state->ep[i]) {
                state->ep[j]++;
            }
        }
    }
}

void i2eo(int index, cube *state) {
    int sum_eo = 0;
    for (int i = 10; i > -1; i--) {
        state->eo[i] = index % 2;
        index /= 2;
        sum_eo += state->co[i];
    }
    state->eo[11] = (2 - sum_eo % 2) % 2;
}

/* 探索する関数 */
int search(cube *state, int depth, sol *solution) {
    /* 探索終了の判定 */
    if (depth == 0) {
        if (is_solved(state)) {
            return 1;
        }
        return 0;
    }

    /* 枝刈り */
    if (prune(depth, state)) {
        return 0;
    }

    /* 探索 */
    for (int i = 0; i < 18; i++) {
        solution->array[solution->len] = i;
        solution->len++;
        cube moved = *state;
        apply_move(&moved, i);
        if (search(&moved, depth - 1, solution)) {
            return 1;
        }
        solution->len--;
    }
    return 0;
}

/* 中心の処理をする関数 */
void solve(cube *state, sol *solution) {
    /* 20手まで掘る */
    for (int depth = 0; depth <= 20; depth++) {
        if (search(state, depth, solution)) {
            solution->solved = 1;
            return;
        }
    }
    solution->solved = 0;
}

/* 表示用関数 */
void print_state(cube state) {
    printf("cp: [%d", state.cp[0]);
    for (int i = 0; i < 7; i++) {
        printf(", %d", state.cp[i + 1]);
    }
    printf("]\n");
    printf("co: [%d", state.co[0]);
    for (int i = 0; i < 7; i++) {
        printf(", %d", state.co[i + 1]);
    }
    printf("]\n");
    printf("ep: [%d", state.ep[0]);
    for (int i = 0; i < 11; i++) {
        printf(", %d", state.ep[i + 1]);
    }
    printf("]\n");
    printf("eo: [%d", state.eo[0]);
    for (int i = 0; i < 11; i++) {
        printf(", %d", state.eo[i + 1]);
    }
    printf("]\n");

    return;
}

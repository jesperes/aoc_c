#include <aoc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EDGES 5

typedef int node_id_t;

typedef struct {
    node_id_t id;
    int out_edges[MAX_EDGES];
    int num_edges;
} vertex_t;

#define V_start 0
#define V_ju 1
#define V_kj 2
#define V_ma 3
#define V_rv 4
#define V_vc 5
#define V_xe 6
#define V_zd 7
#define V_MP 8
#define V_QD 9
#define V_UB 10
#define V_YZ 11
#define V_end 12

#define V_SMALL_SIZE 8
#define V_SIZE 13

#define IS_SMALL(x) (x <= 7)

vertex_t vertices[] = {{V_start, {V_ma, V_xe, V_MP}, 3},
                       {V_ju, {V_MP}, 1},
                       {V_kj, {V_QD, V_rv}, 2},
                       {V_ma, {V_rv, V_UB, V_MP, V_xe}, 4},
                       {V_rv, {V_YZ, V_MP, V_kj, V_ma, V_UB}, 5},
                       {V_vc, {V_MP}, 1},
                       {V_xe, {V_MP, V_UB, V_ma}, 3},
                       {V_zd, {V_YZ, V_end, V_UB}, 3},
                       {V_MP, {V_vc, V_rv, V_xe, V_ju, V_ma}, 5},
                       {V_QD, {V_kj}, 1},
                       {V_UB, {V_end, V_rv, V_ma, V_zd, V_xe}, 5},
                       {V_YZ, {V_rv, V_zd, V_end}, 3},
                       {V_end, {}, 0}};

typedef struct {
    uint8_t freq[V_SMALL_SIZE];
} path_t;

void mark_visited(path_t *path, int node) { path->freq[node]++; }

bool allow_small_cave_visit(path_t *path, int node, int part) {
    if (part == 1) {
        return path->freq[node] == 0;
    } else {
        int max = 0;
        for (int i = 0; i < V_SMALL_SIZE; i++) {
            int f = path->freq[i];
            max = (f > max) ? f : max;
        }

        if (max >= 2 && path->freq[node] >= 1)
            return false;
        else
            return true;
    }
}

void copy(path_t *from, path_t *to) { memcpy(to, from, sizeof(path_t)); }

int64_t copytime = 0;

int find_all_paths(node_id_t node, path_t *path, int depth, int part) {
    vertex_t vertex = vertices[node];
    path_t newpath = {0};
    int numpaths = 0;

    if (node == V_end)
        return numpaths + 1;

    if (IS_SMALL(node)) {
        if (!allow_small_cave_visit(path, node, part)) {
            return numpaths;
        }

        copy(path, &newpath);
        mark_visited(&newpath, node);
    } else {
        copy(path, &newpath);
    }

    for (int i = 0; i < vertex.num_edges; i++) {
        numpaths +=
            find_all_paths(vertex.out_edges[i], &newpath, depth + 1, part);
    }

    return numpaths;
}

aoc_result_t day12(char *ptr, int len) {
    aoc_result_t result = {0};
    path_t ft = {0};

    result.p1 = find_all_paths(V_start, &ft, 0, 1);
    result.p2 = find_all_paths(V_start, &ft, 0, 2);

    return result;
}

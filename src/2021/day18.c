#include "aoc.h"
#include "utils.h"
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

enum Direction { Left = false, Right = true };

#define OTHER(dir) (!(dir))

enum TokenId { //
    TokenInteger,
    TokenComma,
    TokenLeftBracket,
    TokenRightBracket
};

typedef struct {
    enum TokenId id;
    int value;
} token_t;

typedef struct {
    token_t *tokens;
    int capacity;
    int len;
} token_list_t;

struct node {
    bool is_leaf;
    struct node *parent;
    union {
        struct {
            // The location of this node in the token array
            int value;
        } leaf;
        struct {
            struct node *left;
            struct node *right;
        } internal;
    } type;
};

typedef struct node node_t;

void print_token(token_t *token) {
    switch (token->id) {
    case TokenInteger:
        printf("Integer(%d)\n", token->value);
        break;
    case TokenLeftBracket:
        printf("LBracket\n");
        break;
    case TokenRightBracket:
        printf("RBracket\n");
        break;
    case TokenComma:
        printf("Comma\n");
        break;
    default:
        assert(0);
        return;
    }
}

void print_tokens(token_list_t *token_list) {
    printf("%d tokens:\n", token_list->len);
    for (int i = 0; i < token_list->len; i++) {
        print_token(&token_list->tokens[i]);
    }
}

token_list_t lexer(char *p, int len) {
    token_list_t token_list;
    token_list.capacity = 10;
    token_list.tokens = calloc(token_list.capacity, sizeof(token_t));
    int i = 0;
    char *q = p;

    while (true) {
        if (q - p >= len) {
            token_list.len = i;
            return token_list;
        }
        if (i >= token_list.capacity) {
            token_list.capacity *= 2;
            token_list.tokens = realloc(token_list.tokens,
                                        token_list.capacity * sizeof(token_t));
        }

        token_t token = {0};
        char c = *q;

        switch (c) {
        case '[':
            token.id = TokenLeftBracket;
            q++;
            break;
        case ',':
            token.id = TokenComma;
            q++;
            break;
        case ']':
            token.id = TokenRightBracket;
            q++;
            break;
        default:
            assert(isdigit(*q));
            token.id = TokenInteger;
            token.value = strtol(q, &q, 10);
            assert(*q == ',' || *q == ']');
            break;
        }

        token_list.tokens[i++] = token;
    }

    assert(0);
    return token_list;
}

void print_node(node_t *node) {
    if (node == NULL) {
        printf("NULL NODE\n");
    } else if (node->is_leaf) {
        printf("INT[%p]: value=%d parent=%p\n", node, node->type.leaf.value,
               node->parent);
    } else {
        printf("PAIR[%p]: parent=%p left=%p right=%p\n", node, node->parent,
               node->type.internal.left, node->type.internal.right);
    }
}

node_t *do_parse(token_list_t *token_list, node_t *parent, int i,
                 int *next_token_idx) {
    node_t *node = calloc(1, sizeof(node_t));
    node->parent = parent;

    switch (token_list->tokens[i].id) {
    case TokenInteger:
        node->is_leaf = true;
        node->type.leaf.value = token_list->tokens[i].value;
        if (next_token_idx)
            *next_token_idx = i + 1;
        break;

    case TokenLeftBracket: {
        int idx;
        node->is_leaf = false;
        node->type.internal.left = do_parse(token_list, node, i + 1, &idx);
        node->type.internal.right = do_parse(token_list, node, idx + 1, &idx);
        if (next_token_idx)
            *next_token_idx = idx + 1;
        break;
    }

    default:
        assert(0);
    }

    // print_node(node);
    return node;
}

node_t *parse(char *str, int len) {
    if (len < 0)
        len = strlen(str);

    token_list_t token_list = lexer(str, len);
    node_t *root = do_parse(&token_list, NULL, 0, NULL);
    free(token_list.tokens);
    return root;
}

void do_print_nodes(node_t *node) {
    if (node->is_leaf) {
        printf("%d", node->type.leaf.value);
    } else {
        printf("[");
        do_print_nodes(node->type.internal.left);
        printf(",");
        do_print_nodes(node->type.internal.right);
        printf("]");
    }
}

void print_nodes(node_t *node) {
    do_print_nodes(node);
    printf("\n");
}

void free_nodes(node_t *node) {
    if (!node->is_leaf) {
        free_nodes(node->type.internal.left);
        free_nodes(node->type.internal.right);
    }

    free(node);
}

node_t *find_explodable_node(node_t *node, int depth) {
    if (node->is_leaf) {
        return NULL;
    } else if (depth >= 4) {
        return node;
    } else {
        node_t *left =
            find_explodable_node(node->type.internal.left, depth + 1);

        if (left != NULL)
            return left;

        return find_explodable_node(node->type.internal.right, depth + 1);
    }
}

node_t *find_leaf(node_t *node, enum Direction direction) {
    if (node->is_leaf) {
        return node;
    } else {
        return find_leaf((direction == Right) ? node->type.internal.right
                                              : node->type.internal.left,
                         direction);
    }
}

// Find a node's in-order traversal sibling in the given
// direction
node_t *find_inorder_sibling(node_t *node, enum Direction direction) {
    if (node->parent == NULL) {
        // We have reached the top without being able to find a sibling, this
        // means that we started with the left-most or right-most leaf.
        return NULL;
    }

    if (node == node->parent->type.internal.right) {
        if (direction == Right) {
            return find_inorder_sibling(node->parent, direction);
        } else {
            return find_leaf(node->parent->type.internal.left,
                             OTHER(direction));
        }
    } else if (node == node->parent->type.internal.left) {
        if (direction == Right) {
            return find_leaf(node->parent->type.internal.right,
                             OTHER(direction));
        } else {
            return find_inorder_sibling(node->parent, direction);
        }
    }
    assert(0);
    return NULL;
}

void explode(node_t *node) {
    // exploding nodes should be a pair of integers
    assert(!node->is_leaf);
    assert(node->type.internal.left->is_leaf);
    assert(node->type.internal.right->is_leaf);

    node_t *left_sibling = find_inorder_sibling(node, Left);
    if (left_sibling != NULL) {
        assert(left_sibling->is_leaf);
        left_sibling->type.leaf.value +=
            node->type.internal.left->type.leaf.value;
    }
    node_t *right_sibling = find_inorder_sibling(node, Right);
    if (right_sibling != NULL) {
        assert(right_sibling->is_leaf);
        right_sibling->type.leaf.value +=
            node->type.internal.right->type.leaf.value;
    }

    // Replace this node with a leaf node with value 0.
    free(node->type.internal.left);
    node->type.internal.left = NULL;
    free(node->type.internal.right);
    node->type.internal.right = NULL;
    node->is_leaf = true;
    node->type.leaf.value = 0;
}

bool maybe_explode(node_t *node) {
    node_t *explodable = find_explodable_node(node, 0);
    if (explodable != NULL) {
        // printf("exploding {%d,%d}\n",
        //        explodable->type.internal.left->type.leaf.value,
        //        explodable->type.internal.right->type.leaf.value);
        explode(explodable);
        return true;
    } else {
        return false;
    }
}

node_t *find_splittable_node(node_t *node) {
    if (node->is_leaf) {
        if (node->type.leaf.value >= 10)
            return node;
        else
            return NULL;
    } else {
        node_t *left = find_splittable_node(node->type.internal.left);
        if (left != NULL)
            return left;

        return find_splittable_node(node->type.internal.right);
    }
}

void split(int x, int *left, int *right) {
    double half = ((double)x) / 2;
    *left = (int)floor(half);
    *right = (int)ceil(half);
}

bool maybe_split(node_t *node) {
    node_t *splittable = find_splittable_node(node);
    if (splittable != NULL) {
        assert(splittable->is_leaf);
        int value = splittable->type.leaf.value;
        int left;
        int right;
        split(value, &left, &right);
        // printf("splitting %d -> {%d,%d}\n", value, left, right);
        splittable->is_leaf = false;
        splittable->type.internal.left = malloc(sizeof(node_t));
        splittable->type.internal.left->parent = splittable;
        splittable->type.internal.left->is_leaf = true;
        splittable->type.internal.left->type.leaf.value = left;
        splittable->type.internal.right = malloc(sizeof(node_t));
        splittable->type.internal.right->parent = splittable;
        splittable->type.internal.right->is_leaf = true;
        splittable->type.internal.right->type.leaf.value = right;
        return true;
    } else {
        return false;
    }
}

void reduce(node_t *root) {
    while (true) {
        // printf("reducing ");
        // print_nodes(root);
        if (maybe_explode(root)) {
            continue;
        }
        if (maybe_split(root)) {
            continue;
        }
        break;
    }
}

node_t *add(node_t *left, node_t *right) {
    node_t *node = malloc(sizeof(node_t));
    node->is_leaf = false;
    node->parent = NULL;
    node->type.internal.left = left;
    node->type.internal.right = right;
    left->parent = node;
    right->parent = node;

    // printf("Adding: ");
    // print_nodes(left);
    // printf("      + ");
    // print_nodes(right);
    reduce(node);
    // printf("      = ");
    // print_nodes(node);
    return node;
}

int magnitude(node_t *node) {
    if (node->is_leaf) {
        return node->type.leaf.value;
    } else {
        return magnitude(node->type.internal.left) * 3 +
               magnitude(node->type.internal.right) * 2;
    }
}

int add_numbers(char *input, int len) {
    int numlines = 0;
    char **lines = split_input_to_lines(input, len, &numlines);
    node_t *a = parse(lines[0], strlen(lines[0]));
    for (int i = 1; i < numlines; i++) {
        node_t *b = parse(lines[i], strlen(lines[i]));
        a = add(a, b);
    }
    free_lines(lines, numlines);
    int m = magnitude(a);
    // print_nodes(a);
    free_nodes(a);
    // printf("magnitude = %d\n", m);
    return m;
}

int find_max_magnitude(char *input, int len) {
    int numlines = 0;
    char **lines = split_input_to_lines(input, len, &numlines);
    int max_magnitude = INT_MIN;

    for (int i = 0; i < numlines; i++) {
        for (int j = 0; j < numlines; j++) {
            if (i == j)
                continue;

            node_t *a = parse(lines[i], -1);
            node_t *b = parse(lines[j], -1);
            node_t *sum = add(a, b);
            int m = magnitude(sum);
            if (m > max_magnitude)
                max_magnitude = m;
        }
    }

    return max_magnitude;
}

aoc_result_t day18(char *input, int len) {
    aoc_result_t result = {0};
    result.p1 = add_numbers(input, len);
    result.p2 = find_max_magnitude(input, len);
    return result;
}

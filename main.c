#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RED 0
#define BLACK 1

typedef struct Node {
    int key, color;
    struct Node *l, *r, *p;
} Node;

typedef struct RBTree {
    Node *root, *NIL;
} RBTree;

void rb_inorder(RBTree *T, Node *x);
void rb_preorder(RBTree *T, Node *x);
void rb_print(RBTree *T);

void left_rotate(RBTree *T, Node *x);
void rightt_rotate(RBTree *T, Node *x);

void rb_init(RBTree *T);
int black_height(RBTree *T, Node *x);
bool check_bh(RBTree *T, Node *x);
bool check_color(RBTree *T, Node *x);
bool rb_verify(RBTree *T);
bool rb_find(RBTree *T, int val);
void rb_insert(RBTree *T, int val);
void rb_insert_fixup(RBTree *T, Node *x);

void rb_inorder(RBTree *T, Node *x) {
    if (!x || x == T->NIL) return;
    rb_inorder(T, x->l);
    printf("%d(%c) ", x->key, x->color?'b':'r');
    rb_inorder(T, x->r);
}
void rb_preorder(RBTree *T, Node *x) {
    if (!x || x == T->NIL) return;
    printf("%d(%c) ", x->key, x->color?'b':'r');
    rb_preorder(T, x->l);
    rb_preorder(T, x->r);
}
void rb_print(RBTree *T) {
    printf("inorder: ");
    rb_inorder(T, T->root);
    printf("\npreorder: ");
    rb_preorder(T, T->root);
    printf("\n");
}



void left_rotate(RBTree *T, Node *x) {
    Node *y = x->p;
    y->r = x->l;
    x->p = y->p;
    if (y->p == T->NIL) T->root = x;
    else if (y == y->p->l) y->p->l = x;
    else y->p->r = x;
    y->p = x;
    x->l->p = y;
    x->l = y;
}
void right_rotate(RBTree *T, Node *x) {
    Node *y = x->p;
    y->l = x->r;
    x->p = y->p;
    if (y->p == T->NIL) T->root = x;
    else if (y == y->p->l) y->p->l = x;
    else y->p->r = x;
    y->p = x;
    x->r->p = y;
    x->r = y;
}



void rb_init(RBTree *T) {
    T->NIL = (Node*)malloc(sizeof(Node));
    T->root = T->NIL;
    T->NIL->color = BLACK;
    T->NIL->key = -1;
    T->NIL->p = T->NIL->l = T->NIL->r = NULL;
}

int black_height(RBTree *T, Node *x) {
    // include x itself.
    if (x == T->NIL) return 0;
    return x->color==BLACK?1:0 + black_height(T, x->l);
}

bool check_bh(RBTree *T, Node *x) {
    if (x == T->NIL) return true;
    if (black_height(T, x->l) != black_height(T, x->r)) return false;
    return check_bh(T, x->l) & check_bh(T, x->r);
}

bool check_color(RBTree *T, Node *x) {
    if (x == T->NIL) return true;
    if (x->p && x->color == RED && x->p->color == RED) return false;
    return check_color(T, x->l) & check_color(T, x->r);
}

bool rb_verify(RBTree *T) {
    if (T->root == T->NIL) return true;
    if (T->root->color != BLACK) return false;
    if (!check_bh(T, T->root)) return false;
    if (!check_color(T, T->root)) return false;
    return true;
}

bool rb_find(RBTree *T, int val) {
    if (T->root == T->NIL) return false;
    Node *x = T->root;
    while (x != T->NIL) {
        if (val > x->key) x = x->r;
        else if (val < x->key) x = x->l;
        else return true;
    }
    return false;
}

void rb_insert(RBTree *T, int val) {
    if (rb_find(T, val)) return;

    Node *x = (Node*)malloc(sizeof(Node));
    x->color = RED;
    x->l = x->r = T->NIL;
    x->key = val;

    if (T->root == T->NIL) {
        T->root = x;
        T->root->color = BLACK;
        T->root->p = T->NIL;
        return;
    }

    Node *z = T->root, *y = T->root;
    while (z != T->NIL) {
        if (val > z->key) {
            y = z;
            z = z->r;
        }
        else {
            y = z;
            z = z->l;
        }
    }
    if (val < y->key) y->l = x;
    else y->r = x;
    x->p = y;
    rb_insert_fixup(T, x);
    if (T->root->color == RED)
        T->root->color = BLACK;
}

void rb_insert_fixup(RBTree *T, Node *x) {
    while (x->p->color == RED) {
        if (x->p == x->p->p->l) {
            if (x->p->p->r->color == RED) {  // case 1
                x->p->color = BLACK;
                x->p->p->color = RED;
                x->p->p->r->color = BLACK;
                x = x->p->p;
            }
            else {
                if (x == x->p->r) {  // case 2
                    left_rotate(T, x);
                    x = x->l;
                }
                else {  // case 3
                    x->p->color = BLACK;
                    x->p->p->color = RED;
                    right_rotate(T, x->p);
                }
            }
        }
        else {
            if (x->p->p->l->color == RED) {  // case 1
                x->p->color = BLACK;
                x->p->p->color = RED;
                x->p->p->l->color = BLACK;
                x = x->p->p;
            }
            else {
                if (x == x->p->l) {  // case 2
                    right_rotate(T, x);
                    x = x->r;
                }
                else {  // case 3
                    x->p->color = BLACK;
                    x->p->p->color = RED;
                    left_rotate(T, x->p);
                }
            }
        }
    }
}
                            


int main() {
    RBTree T;
    rb_init(&T);
    int x;
    while (scanf("%d", &x) != EOF) {
        // printf("inserting %d...\n", x);
        rb_insert(&T, x);
        // rb_print(&T);
        if (!rb_verify(&T))
            printf("ERROR!!\n");
        // printf("\n");
    }
}
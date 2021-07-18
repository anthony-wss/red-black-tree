#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RED 0
#define BLACK 1
#define Color int

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
void right_rotate(RBTree *T, Node *x);


void rb_init(RBTree *T);

int black_height(RBTree *T, Node *x);
bool check_bh(RBTree *T, Node *x);
bool check_color(RBTree *T, Node *x);
bool rb_verify(RBTree *T);

Node* rb_find(RBTree *T, int val);

void rb_insert(RBTree *T, int val);
void rb_insert_fixup(RBTree *T, Node *x);

void rb_transplant(RBTree *T, Node *u, Node *v);
Node *rb_minimum(RBTree *T, Node *x);
void rb_delete(RBTree *T, int val);
void rb_delete_fixup(RBTree *T, Node *x);



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
    Node *y = x->r;
    x->r = y->l;
    if (y != T->NIL)
        y->p = x->p;
    if (x->p == T->NIL) T->root = y;
    else if (x == x->p->l) x->p->l = y;
    else x->p->r = y;
    x->p = y;
    if(y->l != T->NIL)
        y->l->p = x;
    y->l = x;
}
void right_rotate(RBTree *T, Node *x) {
    Node *y = x->l;
    x->l = y->r;
    if (y != T->NIL)
        y->p = x->p;
    if (x->p == T->NIL) T->root = y;
    else if (x == x->p->l) x->p->l = y;
    else x->p->r = y;
    x->p = y;
    if (y->r != T->NIL)
        y->r->p = x;
    y->r = x;
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


Node* rb_find(RBTree *T, int val) {
    if (T->root == T->NIL) return T->NIL;
    Node *x = T->root;
    while (x != T->NIL) {
        if (val > x->key) x = x->r;
        else if (val < x->key) x = x->l;
        else return x;
    }
    return T->NIL;
}


void rb_insert(RBTree *T, int val) {
    if (rb_find(T, val) != T->NIL) return;

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
                    left_rotate(T, x->p);
                    x = x->l;
                }
                else {  // case 3
                    x->p->color = BLACK;
                    x->p->p->color = RED;
                    right_rotate(T, x->p->p);
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
                    right_rotate(T, x->p);
                    x = x->r;
                }
                else {  // case 3
                    x->p->color = BLACK;
                    x->p->p->color = RED;
                    left_rotate(T, x->p->p);
                }
            }
        }
    }
}


void rb_transplant(RBTree *T, Node *u, Node *v) {
    // replace subtree rooted at u by subtree v
    if (u->p == T->NIL)
        T->root = v;
    else if (u == u->p->l)
        u->p->l = v;
    else
        u->p->r = v;
    v->p = u->p;
}

Node *rb_minimum(RBTree *T, Node *x) {
    // return the minimum node in subtree rooted at x
    if (x->l == T->NIL) return x;
    return rb_minimum(T, x->l);
}

void rb_delete(RBTree *T, int val) {
    Node *z = rb_find(T, val), *x, *y;
    if (z == T->NIL) return;
    // printf("z=%d, z.p=%d\n", z->key, z->p->key);
    Color y_original_color = z->color;
    if (z->l == T->NIL) {
        x = z->r;
        rb_transplant(T, z, z->r);
    }
    else if (z->r == T->NIL) {
        x = z->l;
        rb_transplant(T, z, z->l);
    }
    else {
        y = rb_minimum(T, z->r);
        y_original_color = y->color;
        x = y->r;
        x->p = y;  // x might be NIL
        if (z->r == y) {
            rb_transplant(T, z, y);
            y->l = z->l;
            if (y->l != T->NIL)
                y->l->p = y;
        }
        else {
            rb_transplant(T, y, y->r);
            y->r = z->r;  /* (solved) weird */
            z->r->p = y;
            rb_transplant(T, z, y);
            y->l = z->l;
            if (y->l != T->NIL)
                y->l->p = y;
        }
        y->color = z->color;
    }
    // rb_print(T);
    // printf("x=%d, x.p=%d\n", x->key, x->p->key);
    if (y_original_color == BLACK)
        rb_delete_fixup(T, x);
}

void rb_delete_fixup(RBTree *T, Node *x) {
    // printf("x=%d, x.p=%d\n", x->key, x->p->key);
    while (x->p != T->NIL && x->color == BLACK) {
        if (x == x->p->l) {
            Node *w = x->p->r;
            if (w->color == RED) {  // case 1
                w->color = x->p->color;
                x->p->color = RED;
                left_rotate(T, x->p);
            }
            else if (w->l->color == BLACK && w->r->color == BLACK) {  // case 2
                w->color = RED;
                x = x->p;
            }
            else if (w->l->color == RED && w->r->color == BLACK) {  // case 3
                w->color = RED;
                w->l->color = BLACK;
                right_rotate(T, w);
            }
            else {  // case 4
                w->r->color = BLACK;
                w->color = x->p->color;
                x->p->color = BLACK;
                left_rotate(T, x->p);
                x = T->root;
            }
        }
        else {
            Node *w = x->p->l;
            if (w->color == RED) {  // case 1
                w->color = x->p->color;
                x->p->color = RED;
                right_rotate(T, x->p);
            }
            else if (w->l->color == BLACK && w->r->color == BLACK) {  // case 2
                w->color = RED;
                x = x->p;
            }
            else if (w->r->color == RED && w->l->color == BLACK) {  // case 3
                w->color = RED;
                w->r->color = BLACK;
                left_rotate(T, w);
            }
            else {  // case 4
                w->l->color = BLACK;
                w->color = x->p->color;
                x->p->color = BLACK;
                right_rotate(T, x->p);
                x = T->root;
            }
        }
    }
    if(x->color == RED)
        x->color = BLACK;
}



int main() {
    RBTree T;
    rb_init(&T);
    int x, y;
    while (scanf("%d%d", &x, &y) != EOF) {
        if (x == 1) {
            // printf("inserting %d...\n", y);
            rb_insert(&T, y);
        }
        else if (x == 2) {
            // printf("deleting %d...\n", y);
            rb_delete(&T, y);
        }
        // rb_print(&T);
        // if (!rb_verify(&T)) {
        //     printf("ERROR!!\n");
        //     return 0;
        // }
        // printf("\n");
    }
}
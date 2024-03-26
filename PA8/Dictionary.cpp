#include "Dictionary.h"
#define RED   2
#define BLACK 1

//* Helper Functions(Optional)
void Dictionary::inOrderString(std::string &s, Node *R) const {
    if (R != nil) {
        inOrderString(s, R->left);
        s.append(R->key).append(" : ").append(std::to_string(R->val)).append("\n");
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderString(std::string &s, Node *R) const {
    if (R != nil) {
        s.append(R->key);
        if (R->color == RED) {
            s.append(" (RED)");
        }
        s.append("\n");
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node *R, Node *N) {
    if (R != N) {
        Node *x = root;
        Node *y = nil;
        while (x != nil) {
            y = x;
            if (R->key > x->key) {
                x = x->right;
            } else if (R->key < x->key) {
                x = x->left;
            } else {
                x->val = R->val;
                return;
            }
        }
        Node *z = new Node(R->key, R->val);
        z->parent = y;
        if (y == nil) {
            root = z;
            root->left = nil;
            root->right = nil;
            num_pairs++;
        }
        else if (R->key > y->key) {
            y->right = z;
            z->left = nil;
            z->right = nil;
            num_pairs++;
        }
        else if (R->key < y->key) {
            y->left = z;
            z->left = nil;
            z->right = nil;
            num_pairs++;
        }
        z->color = R->color;
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::postOrderDelete(Node *R) {
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

Dictionary::Node *Dictionary::search(Node *R, keyType k) const {
    if (R == nil || R->key == k) {
        return R;
    } else if (k > R->key) {
        return search(R->right, k);
    } else {
        return search(R->left, k);
    }
}

Dictionary::Node *Dictionary::findMin(Node *R) {
    if (R == nil) {
        return nil;
    } else if (R->left == nil) {
        return R;
    } else {
        return findMin(R->left);
    }
}

Dictionary::Node *Dictionary::findMax(Node *R) {
    if (R == nil) {
        return nil;
    } else if (R->right == nil) {
        return R;
    } else {
        return findMax(R->right);
    }
}

Dictionary::Node *Dictionary::findPrev(Node *N) {
    if (N == nil) {
        return nil;
    }
    else if (N->left != nil) {
        return findMax(N->left);
    } else {
        Node *parent = N->parent;
        while (parent != nil && N == parent->left) {
            N = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

Dictionary::Node *Dictionary::findNext(Node *N) {
    if (N == nil) {
        return nil;
    }
    else if (N->right != nil) {
        return findMin(N->right);
    } else {
        Node *parent = N->parent;
        while (parent != nil && N == parent->right) {
            N = parent;
            parent = parent->parent;
        }
        return parent;
    }
}

void Dictionary::transplant(Node *u, Node *v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nil) {
        v->parent = u->parent;
    }
}

//* RBT Helper Functions(Optional)
void Dictionary::LeftRotate(Node *N) {
    Node *Y = N->right;

    N->right = Y->left;
    if (Y->left != nil) {
        Y->left->parent = N;
    }

    Y->parent = N->parent;
    if (N->parent == nil) {
        root = Y;
    } else if (N == N->parent->left) {
        N->parent->left = Y;
    } else {
        N->parent->right = Y;
    }
    Y->left = N;
    N->parent = Y;
}

void Dictionary::RightRotate(Node *N) {
    Node *Y = N->left;

    N->left = Y->right;
    if (Y->right != nil) {
        Y->right->parent = N;
    }

    Y->parent = N->parent;
    if (N->parent == nil) {
        root = Y;
    } else if (N == N->parent->right) {
        N->parent->right = Y;
    } else {
        N->parent->left = Y;
    }
    Y->right = N;
    N->parent = Y;
}

void Dictionary::RB_InsertFixUp(Node *N) {
    while (N->parent->color == RED) {
        if (N->parent == N->parent->parent->left) {
            Node *Y = N->parent->parent->right;
            if (Y->color == RED) {
                N->parent->color = BLACK;
                Y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if (N == N->parent->right) {
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        } else {
            Node *Y = N->parent->parent->left;
            if (Y->color == RED) {
                N->parent->color = BLACK;
                Y->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if (N == N->parent->left) {
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Node *u, Node *v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void Dictionary::RB_DeleteFixUp(Node *N) {
    while (N != root && N->color == BLACK) {
        if (N == N->parent->left) {
            Node *W = N->parent->right;
            if (W->color == RED) {
                W->color = BLACK;
                N->parent->color = RED;
                LeftRotate(N->parent);
                W = N->parent->right;
            }
            if (W->left->color == BLACK && W->right->color == BLACK) {
                W->color = RED;
                N = N->parent;
            } else {
                if (W->right->color == BLACK) {
                    W->left->color = BLACK;
                    W->color = RED;
                    RightRotate(W);
                    W = N->parent->right;
                }
                W->color = N->parent->color;
                N->parent->color = BLACK;
                W->right->color = BLACK;
                LeftRotate(N->parent);
                N = root;
            }
        } else {
            Node *W = N->parent->left;
            if (W->color == RED) {
                W->color = BLACK;
                N->parent->color = RED;
                RightRotate(N->parent);
                W = N->parent->left;
            }
            if (W->right->color == BLACK && W->left->color == BLACK) {
                W->color = RED;
                N = N->parent;
            } else {
                if (W->left->color == BLACK) {
                    W->right->color = BLACK;
                    W->color = RED;
                    LeftRotate(W);
                    W = N->parent->left;
                }
                W->color = N->parent->color;
                N->parent->color = BLACK;
                W->left->color = BLACK;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = BLACK;
}

void Dictionary::RB_Delete(Node *N) {
    Node *Y = N;
    Node *X;
    int y_original_color = Y->color;
    if (N->left == nil) {
        X = N->right;
        RB_Transplant(N, N->right);
    } else if (N->right == nil) {
        X = N->left;
        RB_Transplant(N, N->left);
    } else {
        Y = findMin(N->right);
        y_original_color = Y->color;
        X = Y->right;
        if (Y->parent == N) {
            N->parent = Y;
        } else {
            RB_Transplant(Y, Y->right);
            Y->right = N->right;
            Y->right->parent = Y;
        }
        RB_Transplant(N, Y);
        Y->left = N->left;
        Y->left->parent = Y;
        Y->color = N->color;
    }
    if (y_original_color == BLACK) {
        RB_Delete(X);
    }
}

//* Class Constructors & Destructors

Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = NULL;
    left = NULL;
    right = NULL;
    color = BLACK;
}

Dictionary::Dictionary() {
    nil = new Node("NIL NODE", 0);
    root = nil;
    current = nil;
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary &D) {
    nil = new Node("NIL NODE", 0);
    root = nil;
    current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
    delete nil;
    postOrderDelete(root);
}

//* Access functions

int Dictionary::size() const {
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    if (search(root, k) == nil) {
        return false;
    }
    return true;
}

valType &Dictionary::getValue(keyType k) const {
    if (!contains(k)) {
        throw std::logic_error("Dictionary: getValue(): key is not in the Dictionary");
    }
    Node *node = search(root, k);
    return node->val;
}

bool Dictionary::hasCurrent() const {
    if (current != nil) {
        return true;
    }
    return false;
}

keyType Dictionary::currentKey() const {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: currentKey(): current is not defined");
    }
    return current->key;
}

valType &Dictionary::currentVal() const {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: currentVal(): current is not defined");
    }
    return current->val;
}

//* Manipulation procedures

void Dictionary::clear() {
    postOrderDelete(root);
    root = nil;
    current = nil;
    num_pairs = 0;
}

void Dictionary::setValue(keyType k, valType v) {
    Node *prim = root;
    Node *sec = nil;
    while (prim != nil) {
        sec = prim;
        if (k > prim->key) {
            prim = prim->right;
        } else if (k < prim->key) {
            prim = prim->left;
        } else {
            prim->val = v;
            return;
        }
    }
    Node *tri = new Node(k, v);
    tri->parent = sec;
    if (sec == nil) {
        root = tri;
        root->left = nil;
        root->right = nil;
        num_pairs++;

    }
    else if (k < sec->key) {
        sec->left = tri;
        tri->left = nil;
        tri->right = nil;
        num_pairs++;
    }
    else if (k > sec->key) {
        sec->right = tri;
        tri->left = nil;
        tri->right = nil;
        num_pairs++;

    }
    tri->color = RED;
    RB_InsertFixUp(tri);
}

void Dictionary::remove(keyType k) {
    if (!contains(k)) {
        throw std::logic_error("Dictionary: remove(): key is not in Dictionary");
    }
    Node *z = search(root, k);

    if (z == current) {
        current = nil;
    }

    Node *y = z;
    Node *x;
    int y_original_color = y->color;
    if (z->left == nil) {
        x = z->right;
        RB_Transplant(z, z->right);
        delete z;
    }
    else if (z->right == nil) {
        x = z->left;
        RB_Transplant(z, z->left);
        delete z;
    }
    else {
        y = findMin(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
        delete z;
    }
    if (y_original_color == BLACK) {
        RB_DeleteFixUp(x);
    }
    num_pairs--;
}

void Dictionary::begin() {
    current = findMin(root);
}

void Dictionary::end() {
    current = findMax(root);
}

void Dictionary::next() {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: next(): current is nil");
    }
    current = findNext(current);
}

void Dictionary::prev() {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: prev(): current is nil");
    }
    current = findPrev(current);
}

//* Other Functions

std::string Dictionary::to_string() const {
    std::string s;
    inOrderString(s, root);
    return s;
}

std::string Dictionary::pre_string() const {
    std::string s;
    preOrderString(s, root);
    return s;
}

bool Dictionary::equals(const Dictionary &D) const {
    std::string s = D.to_string();
    std::string s1 = to_string();
    return s == s1;
}

//* Overloaded Operators

std::ostream &operator<<(std::ostream &stream, Dictionary &D) {
    stream << D.to_string();
    return stream;
}

bool operator==(const Dictionary &A, const Dictionary &B) {
    return A.equals(B);
}

Dictionary &Dictionary::operator=(const Dictionary &D) {
    if (this != &D) {
        clear();
        preOrderCopy(D.root, D.root);
    }
    return *this;
}

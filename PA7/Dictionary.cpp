/***
*Rutujit(RJ) Mallikarjuna
*rmallika
*2024 Winter CSE 101 PA7
*Dictionary.cpp
* Implements a Dictionary ADT based on a Binary Search Tree, providing functionalities for inserting key-value pairs, removing them, searching for a value given a key, and traversing the tree in both in-order and pre-order to generate string representations of the stored data.
***/
#include "Dictionary.h"

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

//* Class Constructors & Destructors 
Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = NULL;
    left = NULL;
    right = NULL;
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
//* Helper Functions 
void Dictionary::preOrderCopy(Node *R, Node *N) {
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

void Dictionary::preOrderString(std::string &s, Node *R) const {
    if (R != nil) {
        s.append(R->key).append("\n");
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::inOrderString(std::string &s, Node *R) const {
    if (R != nil) {
        inOrderString(s, R->left);
        s.append(R->key).append(" : ").append(std::to_string(R->val)).append("\n");
        inOrderString(s, R->right);
    }
}

void Dictionary::postOrderDelete(Node *R) {
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
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

valType &Dictionary::currentVal() const {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: currentVal(): current is not defined");
    }
    return current->val;
}

keyType Dictionary::currentKey() const {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: currentKey(): current is not defined");
    }
    return current->key;
}

//* Manipulation procedures
void Dictionary::begin() {
    current = findMin(root);
}

void Dictionary::end() {
    current = findMax(root);
}

void Dictionary::prev() {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: prev(): current is nil");
    }
    current = findPrev(current);
}

void Dictionary::next() {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: next(): current is nil");
    }
    current = findNext(current);
}

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
}

void Dictionary::remove(keyType k) {
    if (!contains(k)) {
        throw std::logic_error("Dictionary: remove(): key is not in Dictionary");
    }
    Node *node = search(root, k);
    if (node == current) {
        current = nil;
    }
    if (node->left == nil) {
        transplant(node, node->right);
    }
    else if (node->right == nil) {
        transplant(node, node->left);
    }
    else {
        Node *prim = findMin(node->right);
        if (prim->parent != node) {
            transplant(prim, prim->right);
            prim->right = node->right;
            prim->right->parent = prim;
        }
        transplant(node, prim);
        prim->left = node->left;
        prim->left->parent = prim;
    }
    delete node;
    num_pairs--;
}

//* Other Functions
std::string Dictionary::pre_string() const {
    std::string store;
    preOrderString(store, root);
    return store;
}

std::string Dictionary::to_string() const {
    std::string store;
    inOrderString(store, root);
    return store;
}

bool Dictionary::equals(const Dictionary &D) const {
    std::string store = D.to_string();
    std::string store1 = to_string();
    return store == store1;
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
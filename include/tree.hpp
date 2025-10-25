
template<typename KeyT, typename T>
struct Node {
    public:
        Node *left   = nullptr;
        Node *right  = nullptr;
        Node *parent = nullptr;
        int height = 0;
        T key_;

    public: // constructor
        Node(KeyT key) : key_(key) {}

};

template <typename KeyT, typename Comp>
class AVLTree {
    private:
        Node *top_node;

    public:
        void insert(KeyT key);
        Node* lower_bound(KeyT key);
        Node* upper_bound(KeyT key);

    public: // dump
        void dumpTree(Node *node);

    public: // constructor
        AVLTree() {
        }
};

template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::insert(KeyT key) {
    Node n = Node(key);
    Node *changable = top_node;
    while (changable->left || changable->right) {
        if (Comp(n, *changable) > 0) {
            if (changable->right) {
                changable = changable->right;
            } else {
                changable->right = &n;
                n.parent = changable;
                break;
            }
        } else {
            if (changable->right) {
                changable = changable->left;
            } else {
                changable->left = &n;
                n.parent = changable;
                break;
            }
        }
    }

    // balance_tree();
}

template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::dumpTree(Node *node) {
    if (node->)
}



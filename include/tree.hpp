
template<typename KeyT>
struct Node {
    public:
        Node<KeyT> *left   = nullptr;
        Node<KeyT> *right  = nullptr;
        Node<KeyT> *parent = nullptr;
        int height = 0;
        KeyT key_;

    public: // constructor
        Node(KeyT key) : key_(key) {}

    public:
        void setParent(Node *node) {
            parent = node;
        }

};

template <typename KeyT, typename Comp>
class AVLTree {
    private:
        Node<KeyT> *top_node;

    public:
        void insert(KeyT key);
        Node<KeyT>* lower_bound(KeyT key);
        Node<KeyT>* upper_bound(KeyT key);

    public: // dump
        void dumpTree(Node<KeyT> *node);

    public: // constructor
        AVLTree() {
        }
};

template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::insert(KeyT key) {
    Node n = Node(key);
    Node<KeyT> *changable = top_node;
    while (changable->left || changable->right) {
        if (Comp(n.key_, (*changable).key_) > 0) {
            if (changable->right) {
                changable = changable->right;
                n.height++;
            } else {
                changable->right = &n;
                n.setParent(changable); // use setParent instead
                break;
            }
        } else {
            if (changable->right) {
                changable = changable->left;
                n.height++;
            } else {
                changable->left = &n;
                n.setParent(changable); // use setParent instead
                break;
            }
        }
    }

    // balance_tree();
}
//
// template <typename KeyT, typename Comp>
// void AVLTree<KeyT, Comp>::dumpTree(Node *node) {
//     if (node->)
// }



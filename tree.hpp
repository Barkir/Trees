
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

    public: // constructor
        AVLTree() {
        }
};

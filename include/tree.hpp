#include <fstream>
#include <iostream>
#include <memory>

const size_t DEF_SIZE=1024;
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"


#define TREE_DEBUG
#ifdef TREE_DEBUG
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code)
#endif

template<typename KeyT>
struct Node {
    public:
        std::unique_ptr<Node<KeyT>> left;
        std::unique_ptr<Node<KeyT>> right;
        Node* parent;
        int height;
        KeyT key_;

    public: // constructor
        Node(KeyT key) : left(nullptr), right(nullptr), parent(nullptr), height(0), key_(key) {}

    public:
        void setParent(Node *node) {
            parent = node;
        }

};

template <typename KeyT, typename Comp>
class AVLTree {
    private:
        std::unique_ptr<Node<KeyT>> top_node;
        Comp comp;
        void dumpTreeRecursive(Node<KeyT> *node, std::ofstream& file, int depth=0);

        unsigned int node_color = 0xE0D0FF;

    public:
        void insert(KeyT key);
        Node<KeyT>* lower_bound(KeyT key);
        Node<KeyT>* upper_bound(KeyT key);

    public: // dump
        void dumpTree(Node<KeyT> *node, std::string filename);

    public: // constructor
        AVLTree(Comp InComp) : comp(InComp), top_node(nullptr) {

        }

    public:
        Node<KeyT> *getTopNode() {
            return top_node.get();
        }
};

template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::insert(KeyT key) {

    if (top_node == nullptr) {
        top_node = std::make_unique<Node<KeyT>>(key);
    }
    ON_DEBUG(fprintf(stderr, GREEN "inserting key %d...\n" RESET, key));


    auto new_node = std::make_unique<Node<KeyT>>(key);
    ON_DEBUG(fprintf(stderr, YELLOW "created Node n with adr \t\t%p\n" RESET, new_node.get()));

    Node<KeyT> *changable = top_node.get();
    ON_DEBUG(fprintf(stderr, GREEN "starting to insert from top_node \t%p with field %d\n" RESET, top_node.get(), top_node->key_));

    while (true) {
        if (comp(new_node->key_, (*changable).key_) > 0) {
            ON_DEBUG(fprintf(stderr, MAGENTA "\t WENT RIGHT COMP CASE\n" RESET));
            if (changable->right) {
                ON_DEBUG(fprintf(stderr, MAGENTA "\t WENT RIGHT SUBTREE \t %p with field %d\n" RESET, changable->right.get(), changable->right->key_));
                changable = changable->right.get();
                new_node->height++;
            } else {
                ON_DEBUG(fprintf(stderr, MAGENTA "\t RIGHT SUBTREE IS NULL. SETTING IT\n" RESET));

                new_node->setParent(changable);
                changable->right = std::move(new_node);
                break;
            }
        } else {
            ON_DEBUG(fprintf(stderr, BLUE "WENT LEFT COMPCASE\n" RESET));
            if (changable->left) {
                ON_DEBUG(fprintf(stderr, BLUE "WENT LEFT SUBTREE \t %p with field %d\n" RESET, changable->left.get(), changable->left->key_));
                changable = changable->left.get();
                new_node->height++;
            } else {
                ON_DEBUG(fprintf(stderr, BLUE "\t LEFT SUBTREE IS NULL. SETTING IT\n" RESET));
                new_node->setParent(changable); // use setParent instead
                changable->left = std::move(new_node);
                break;
            }
        }
    }

    // balance_tree();
}

template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::dumpTree(Node<KeyT> *node, std::string filename) {
    std::ofstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "error opening file!" << "\n";
    }

    inputFile << "digraph\n{\n";
    dumpTreeRecursive(node, inputFile, 0);
    inputFile << "}\n";

    std::string command = "dot " + filename + " -T png -o " + filename + ".png";

    inputFile.close();
    system(command.c_str());
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::dumpTreeRecursive(Node<KeyT> *node, std::ofstream& file, int depth) {
    if (!node)
        return;

    file << "node" << node << " [shape = Mrecord; label = \"{" << node->key_ << " | adr = " << node << "}\"; style=filled; fillcolor=\"#" << std::hex << node_color << "\"];\n";

    if (node->left.get()) {
        file << "node" << node << " -> " << "node" << node->left.get() << "\n";
    }

    if(node->right.get()) {
        file << "node" << node << " -> " << "node" << node->right.get() << "\n";
    }
    dumpTreeRecursive(node->left.get(), file, depth+1);
    dumpTreeRecursive(node->right.get(), file, depth+1);
}



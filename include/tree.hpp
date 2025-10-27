#include <fstream>
#include <iostream>
#include <memory>

const size_t DEF_SIZE=1024;
const std::string IMG_DIR = "../images/";

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

        int32_t countBalanceFactor(Node<KeyT>* node);
        int32_t getTreeHeight(Node<KeyT>* node, size_t depth);

        void swapNodes(Node<KeyT>* node1, Node<KeyT>* node2);
        void performLL(Node<KeyT>* node);
        void performRR(Node<KeyT>* node);
        void performLR(Node<KeyT>* node);
        void performRL(Node<KeyT>* node);


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
        return;
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

                Node<KeyT>* changable_parent = changable->parent;
                if (changable_parent) {
                    if (changable_parent->right && changable_parent->right.get() == changable) {
                        if (countBalanceFactor(changable_parent) < -1)
                            performRR(changable_parent);
                    } else if (changable_parent->left && changable_parent->left.get() == changable) {
                        if (countBalanceFactor(changable_parent) < -1)
                            performLR(changable_parent);
                    }
                }

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

                Node<KeyT>* changable_parent = changable->parent;
                if (changable_parent) {
                    if (changable_parent->left && changable_parent->left.get() == changable) {
                        if (countBalanceFactor(changable_parent) > 1)
                            performLL(changable_parent);
                    } else if (changable_parent->right && changable_parent->right.get() == changable) {
                        if (countBalanceFactor(changable_parent) > 1)
                            performRL(changable_parent);
                    }
                }

                break;
            }
        }
    }

    // balance_tree();
}

std::string refactorFilename(std::string filename) {
    return IMG_DIR + filename;
}


template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::dumpTree(Node<KeyT> *node, std::string filename) {

    std::string new_filename = refactorFilename(filename);
    std::ofstream inputFile(new_filename);

    if (!inputFile.is_open()) {
        std::cerr << "error opening file!" << "\n";
    }

    inputFile << "digraph\n{\n";
    dumpTreeRecursive(node, inputFile, 0);
    inputFile << "}\n";

    std::string command = "dot " + new_filename + " -T png -o " + new_filename + ".png";

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

template<typename KeyT, typename Comp>
int32_t AVLTree<KeyT, Comp>::countBalanceFactor(Node<KeyT>* node) {
    size_t left_height  = node->left.get()  ? getTreeHeight(node->left.get(), 0)  : 0;
    size_t right_height = node->right.get() ? getTreeHeight(node->right.get(), 0) : 0;
    fprintf(stdout, RED "\t counting balance factor of node %p - %d\n" RESET, node, left_height - right_height);
    return left_height - right_height;
}

template<typename KeyT, typename Comp>
int32_t AVLTree<KeyT, Comp>::getTreeHeight(Node<KeyT>* node, size_t depth) {
    if (node->left) {
        // ON_DEBUG(std::cout << "height: going left" << " ");
        depth = getTreeHeight(node->left.get(), depth+1);
        // ON_DEBUG(std::cout << "depth = " << depth << "\n");
    } else if (node->right) {
        // ON_DEBUG(std::cout << "height: goint right" << " ");
        depth = getTreeHeight(node->right.get(), depth+1);
        // ON_DEBUG(std::cout << "depth = " << depth << "\n");
    }

    return depth;
}

// ==========================================================
//  This is a rotation functions block
//  All of the rotations start from the node
//  where balance_factor > 1 || balance_factor < -1
//  make sure you use it correctly!
// ==========================================================

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performLL(Node<KeyT>*node) {
    fprintf(stdout, YELLOW "performing LL on %p\n" RESET, node);
    Node<KeyT> *new_right = node;
    Node<KeyT> *new_left  = node->left->left.get();
    node = node->left.get();
    node->right.reset(new_right);
    node->left.reset(new_left);
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performRR(Node<KeyT>* node) {
    fprintf(stdout, YELLOW "performing RR on %p\n" RESET, node);
    Node<KeyT>* new_left = node;
    Node<KeyT>* new_right = node->right->right.get();
    node = node->right.get();
    node->right.reset(new_right);
    node->left.reset(new_left);
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performRL(Node<KeyT>* node) {
    fprintf(stdout, YELLOW "performing RL on %p\n" RESET, node);
    swapNodes(node->right.get(), node->right->left.get());
    node->right->right.reset(node->right->left.get());
    node->right->left.reset(nullptr);
    performLL(node);
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performLR(Node<KeyT>* node) {
    fprintf(stdout, YELLOW "performing LR on %p\n" RESET, node);
    swapNodes(node->left.get(), node->left->right.get());
    node->left->right.reset(node->left->left.get());
    node->right->left.reset(nullptr);
    performRR(node);
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::swapNodes(Node<KeyT>* node1, Node<KeyT>* node2) {
    Node<KeyT>* temp = node1;
    node1 = node2;
    node2 = temp;
}

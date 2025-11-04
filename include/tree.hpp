#include <fstream>
#include <iostream>
#include <memory>

const size_t DEF_SIZE=1024;
const std::string IMG_DIR = "../images/";

enum RotationMode {
    LEFT_CHILD,
    RIGHT_CHILD
};


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
        int32_t height;
        KeyT key_;

    public: // constructor
        Node(KeyT key) : left(nullptr), right(nullptr), parent(nullptr), height(0), key_(key) {}

    public:
        void setParent(Node *node) {
            parent = node;
        }

        void makeIndependent() {
            if (left)   left.release();
            if (right)  right.release();
            if (parent) parent  = nullptr;
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
        using nodeType = std::unique_ptr<Node<KeyT>>&;
        using pathVec = std::vector<std::unique_ptr<Node<KeyT>>*>;

        void insert(KeyT key);
        Node<KeyT>* lowerBound(KeyT key); // first not less then key
        Node<KeyT>* upperBound(KeyT key); // first greater then key



    public: // helping functions
        int32_t countBalanceFactor(Node<KeyT>* node);
        int32_t getTreeHeight(Node<KeyT>* node);
        void printNode(Node<KeyT>* node);

    public: // all about balancing
        void balance(pathVec path);
        void performLL(std::unique_ptr<Node<KeyT>>& node);
        void performRR(std::unique_ptr<Node<KeyT>>& node);
        void performLR(std::unique_ptr<Node<KeyT>>& node);
        void performRL(std::unique_ptr<Node<KeyT>>& node);


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

    std::unique_ptr <Node<KeyT>>* changable = &top_node;
    pathVec path; // path of pointers to unique_ptr;

    // ON_DEBUG(fprintf(stderr, GREEN "starting to insert from top_node \t%p with field %d\n" RESET, top_node.get(), top_node->key_));

    while (true) {
        path.push_back(changable);
        if (comp(key, (*changable)->key_)) {
            ON_DEBUG(fprintf(stderr, MAGENTA "\t WENT RIGHT COMP CASE\n" RESET));
            if ((*changable)->right) {
                ON_DEBUG(fprintf(stderr, MAGENTA "\t WENT RIGHT SUBTREE \t %p with field %d\n" RESET, (*changable)->right.get(), (*changable)->right->key_));
                changable = &(*changable)->right;
            } else {
                ON_DEBUG(fprintf(stderr, MAGENTA "\t RIGHT SUBTREE IS NULL. SETTING IT\n" RESET));
                (*changable)->right = std::make_unique<Node<KeyT>>(key);
                (*changable)->right->setParent((*changable).get());
                ON_DEBUG(fprintf(stderr, MAGENTA "\t RIGHT SUBTREE IS SET. VALUE=%d\n" RESET, (*changable)->right->key_));
                break;
            }
        } else {
            ON_DEBUG(fprintf(stderr, BLUE "WENT LEFT COMPCASE\n" RESET));
            if ((*changable)->left) {
                ON_DEBUG(fprintf(stderr, BLUE "WENT LEFT SUBTREE \t %p with field %d\n" RESET, (*changable)->left.get(), (*changable)->left->key_));
                changable = &(*changable)->left;
            } else {
                ON_DEBUG(fprintf(stderr, BLUE "\t LEFT SUBTREE IS NULL. SETTING IT\n" RESET));
                (*changable)->left = std::make_unique<Node<KeyT>>(key);
                (*changable)->left->setParent((*changable).get());
                ON_DEBUG(fprintf(stderr, MAGENTA "\t LEFT SUBTREE IS SET. VALUE=%d\n" RESET, (*changable)->left->key_));
                break;
            }
        }
    }
    balance(path);
}

std::string refactorFilename(std::string filename) {
    return IMG_DIR + filename;
}


template <typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::dumpTree(Node<KeyT> *node, std::string filename) {

    fprintf(stdout, "======================================\n");
    fprintf(stdout, "dumping tree\t%p\n", node);
    fprintf(stdout, "======================================\n");
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
    fprintf(stdout, YELLOW "\ttop (%p)\n" RESET, node);
    file << "node" << node << " [shape = Mrecord; label = \"{" << node->key_ << " | adr = " << node << " | height =" << node->height <<"}\"; style=filled; fillcolor=\"#" << std::hex << node_color << std::dec << "\"];\n";

    if (node->left.get()) {
        fprintf(stdout, MAGENTA "left (%p)\n" RESET, node->left.get());
        file << "node" << node << " -> " << "node" << node->left.get() << "\n";
        dumpTreeRecursive(node->left.get(), file, depth+1);
    }
    if (node->right.get()) {
        fprintf(stdout, MAGENTA "\t\tright (%p)\n" RESET, node->right.get());
        file << "node" << node << " -> " << "node" << node->right.get() << "\n";
        dumpTreeRecursive(node->right.get(), file, depth+1);
    }
}

template<typename KeyT, typename Comp>
int32_t AVLTree<KeyT, Comp>::countBalanceFactor(Node<KeyT>* node) {
    size_t left_height  = node->left.get()  ? getTreeHeight(node->left.get())  : 0;
    size_t right_height = node->right.get() ? getTreeHeight(node->right.get()) : 0;
    fprintf(stdout, RED "\t counting balance factor of node %p - %d\n" RESET, node, (left_height - right_height));
    return (left_height - right_height);
}

template<typename KeyT, typename Comp>
int32_t AVLTree<KeyT, Comp>::getTreeHeight(Node<KeyT>* node) {
    if (!node) {
        return 0;
    }

    int left_height = getTreeHeight(node->left.get());
    int right_height = getTreeHeight(node->right.get());


    return 1 + std::max(left_height, right_height);
}

// ==========================================================
//  This is a rotation functions block
//  All of the rotations start from the node
//  where balance_factor > 1 || balance_factor < -1
//  make sure you use it correctly!
// ==========================================================

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::printNode(Node<KeyT>* node) {
    fprintf(stdout, MAGENTA "=========================\n");
    fprintf(stdout, "dumping node\t%p\n", node);
    fprintf(stdout, "=========================\n" RESET);
    fprintf(stdout, RED "\t\ttop(%p)\t<-\t" RESET BLUE "parent(%p)\n" RESET, node, node->parent);
    fprintf(stdout, YELLOW "left(%p)\t\t\tright(%p)\n" RESET, node->left.get(), node->right.get());
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::balance(pathVec path) {
    // going from the bottom of the tree to the top
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        std::unique_ptr<Node<KeyT>>* node_ptr = *it;
        int bf = countBalanceFactor(node_ptr->get());
        if (bf > 1) {
            int bf_left = countBalanceFactor((*node_ptr)->left.get());

            if      (bf_left > 0) { performLL(*node_ptr); }
            else if (bf_left < 0) { performLR(*node_ptr); }

        } else if (bf < -1) {
            int bf_right = countBalanceFactor((*node_ptr)->right.get());

            if      (bf_right < 0) { performRR(*node_ptr); }
            else if (bf_right > 0) { performRL(*node_ptr); }
        }

        (*node_ptr)->height = getTreeHeight((*node_ptr).get());
        if ((*node_ptr)->left)  (*node_ptr)->left->height = getTreeHeight((*node_ptr)->left.get());
        if ((*node_ptr)->right) (*node_ptr)->right->height = getTreeHeight((*node_ptr)->right.get());
    }

}

// = = = = = = = = = = = = = = = = = = = = =
// ┌──────────────────────────────────────┐
// │         ROTATION ENGINE              │
// │  Spinning nodes                      │
// │  to keep the tree perfectly balanced │
// └──────────────────────────────────────┘
// = = = = = = = = = = = = = = = = = = = = =


template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performLL(std::unique_ptr<Node<KeyT>>& node) {
    fprintf(stdout, YELLOW "performing LL on %p\n", node.get());
    // plain algorithm
    // 1. node->left moves to the top
    // 2. node moves to the top->right
    // 3. node->left->left moves to top->left

    printNode(node.get());
    Node<KeyT>* parent = node->parent;

    bool isLeft = false;
    if (parent)
        isLeft = parent->left == node ? true : false;

    std::unique_ptr <Node<KeyT>> top = std::move(node->left);
    top->right = std::move(node);
    top->right->setParent(top.get());

    if (parent == nullptr){
        top->setParent(nullptr);
        top_node = std::move(top);
        // printNode(top_node.get());
        // printNode(top_node->right.get());
        // printNode(top_node->left.get());
    } else {
        top->setParent(parent);

        if (isLeft) parent->left = std::move(top);
        else parent->right = std::move(top);

        // printNode(top.get());
        // printNode(top->right.get());
        // printNode(top->left.get());
    }
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performRR(std::unique_ptr<Node<KeyT>>& node) {
    fprintf(stdout, YELLOW "performing RR on %p\n" RESET, node.get());


    printNode(node.get());
    Node<KeyT>* parent = node->parent;

    bool isLeft = false;
    if (parent)
        isLeft = parent->left == node ? true : false;

    fprintf(stdout, "parent=%p\n", node->parent);
    std::unique_ptr <Node<KeyT>> top = std::move(node->right);
    top->left = std::move(node);
    top->left->setParent(top.get());

    if (parent == nullptr){
        top->setParent(nullptr);
        top_node = std::move(top);

        // printNode(top_node.get());
        // printNode(top_node->right.get());
        // printNode(top_node->left.get());
    } else {
        top->setParent(parent);

        if (isLeft) parent->left = std::move(top);
        else parent->right = std::move(top);

        printNode(parent);
        // printNode(top.get());
        // printNode(top->right.get());
        // printNode(top->left.get());
    }
}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performRL(std::unique_ptr<Node<KeyT>>& node) {
    fprintf(stdout, YELLOW "performing RL on %p\n" RESET, node.get());

    std::unique_ptr<Node<KeyT>> toSwap = std::move(node->right->left);
    toSwap->right = std::move(node->right);
    toSwap->right->setParent(toSwap.get());
    node->right = std::move(toSwap);
    node->right->setParent(node.get());
    performRR(node);

}

template<typename KeyT, typename Comp>
void AVLTree<KeyT, Comp>::performLR(std::unique_ptr <Node<KeyT>>& node) {
    fprintf(stdout, YELLOW "performing LR on %p\n" RESET, node.get());

    std::unique_ptr<Node<KeyT>> toSwap = std::move(node->left->right);
    toSwap->left = std::move(node->left);
    toSwap->left->setParent(toSwap.get());
    node->left = std::move(toSwap);
    node->left->setParent(node.get());
    performLL(node);
}

template<typename KeyT, typename Comp>
Node<KeyT>* AVLTree<KeyT, Comp>::lowerBound(KeyT key) {
    fprintf(stdout, "finding first not less than %d\n", key);
    Node<KeyT>* current = top_node.get();
    Node<KeyT>* checker = nullptr;

    while (current) {
        if (comp(current->key_, key)) {
            checker = current;
            current = current->left.get();
        } else {
            current = current->right.get();
        }
    }

    if (checker) fprintf(stdout, "lowerBound = %d\n", checker->key_);
    return checker;
}


template<typename KeyT, typename Comp>
Node<KeyT>* AVLTree<KeyT, Comp>::upperBound(KeyT key) {
    fprintf(stdout, "finding first greater than %d\n", key);

    Node<KeyT>* current = top_node.get();
    Node<KeyT>* checker = nullptr;

    while (current) {
        if (!comp(current->key_, key)) {
            checker = current;
            current = current->right.get();
        } else {
            current = current->left.get();
        }
    }

    if (checker) fprintf(stdout, "upperBound = %d\n", checker->key_);
    return checker;
}

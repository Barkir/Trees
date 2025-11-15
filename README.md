<div align="center">
AVL Tree Implementation
</div>


## Tree Structure
Here's how **AVL Tree** structure looks like

<details open>

```cpp
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
```
</details>

### Rotations for AVL Tree

how to read rotations?
- **first** is for the child, **second** is for the subtree

| Rotation Type | Semantics | Descriptipon |
|---------------|-----------|--------------|
|   LL          |           | occurs when inserting _left_ child into the _left_ subtree && balance_factor > +1            |
|   RR          |           | occurs when inserting _right_ child into the _right_ subtree && balance_factor < -1              |
|   LR          |           | occurs when inserting _left_ child into the _right_ subtree making it **left-heavy**              |
|   RL          |           | occurs when inserting _right_ child into the _left_ subtree making it **right-heavy**              |

### Using graphviz for better debug
![image1](./images/sup.png)

command line flag for dumping image:
```
./avl_tree --dump
```

### Testing

You can test correct answers for `range queries` using `avl_tree_regular` target
###### Typical usecase (with dump)
```
./avl_tree_regular < ../tests/01.dat --dump
```

You can also test performance using apporpriate targets for that
There are two benchmarks available
1. `avl_tree_timebench` - provides time info for custom `AVLTree` implementation
2. `set_timebench` - provides time info for `std::set` from standard C++ library


## Dealing with bad performance
- [this](./tests/test.dat) test got me in trouble.

I need to find a bottleneck to solve it.

This is what I use in command line:
1. `perf record ./avl_tree_timebench < ../tests/test.dat `
2. `hotspot`

The flame graph I saw in hotspot
![img1](./images/perf.png)

<div style="border-left: 5px solid #f44336; padding: 10px; margin: 10px 0; background: #fff;">
  🚨 <strong>Bottleneck:</strong> This function is the performance bottleneck.
</div>

<div style="border-left: 5px solid #ffc107; padding: 12px; margin: 10px 0; background: #fff
">
💡 <strong>Idea:</strong> Contain tree height inside a node.
</div>


##### checkpoints
| # | description| done/undone|
|---|------------|-----------|
| 1 |Write LL, LR, RR, RL functions for AVL tree class|     ☑️      |
| 2 | write CLI for tests            |     ☑️      |

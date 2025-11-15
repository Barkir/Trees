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

<div style="background: linear-gradient(135deg, #ffebee, #ffcdd2, #ef9a9a); border-left: 5px solid #c62828; padding: 12px; margin: 10px 0; font-style: italic; border-radius: 4px; box-shadow: 0 0 8px rgba(198, 40, 40, 0.2); position: relative; overflow: hidden;">
  <div style="position: absolute; top: 0; left: 0; width: 100%; height: 4px; background: linear-gradient(90deg, transparent, #ff5252, transparent); animation: bottleneckFlow 2s linear infinite;"></div>
  🚨 <strong>Bottleneck:</strong> getTreeHeight
</div>

<style>
@keyframes bottleneckFlow { 0% { transform: translateX(-100%); } 100% { transform: translateX(100%); } }
</style>

<div style="background: #fff8e1; border-left: 5px solid #ffc107; padding: 12px; margin: 10px 0; font-style: italic; box-shadow: 0 0 8px rgba(255, 193, 7, 0.3); border-radius: 4px; animation: pulse 2s infinite;">
💡 <strong>Idea:</strong> Contain tree height inside a node.
</div>

<style>
@keyframes pulse { from { box-shadow: 0 0 8px rgba(255, 193, 7, 0.3); } to { box-shadow: 0 0 16px rgba(255, 193, 7, 0.6); } }
</style>


##### checkpoints
| # | description| done/undone|
|---|------------|-----------|
| 1 |Write LL, LR, RR, RL functions for AVL tree class|     ☑️      |
| 2 | write CLI for tests            |     ☑️      |

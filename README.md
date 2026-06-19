# 2-3 Tree Implementation in C

## Overview

This project implements a **2-3 Tree** in the C programming language.

A 2-3 Tree is a balanced search tree where each node can contain either:

* one key (2-node)
* two keys (3-node)

All leaves remain at the same depth, ensuring logarithmic complexity for insertion, deletion, and search operations.

The implementation supports:

* insertion
* deletion
* search
* node splitting
* node merging
* sibling borrowing
* performance benchmarking

---

## Features

* Dynamic 2-3 Tree structure
* Automatic balancing
* Insert operation
* Delete operation
* Search operation
* Node splitting during insertion
* Borrowing from siblings during deletion
* Parent-node merging during deletion
* File-based testing
* Execution time measurement

---

## Data Structure

Each node is represented by:

```c
typedef struct node {
    int value1;
    int value2;
    int middlevalue;

    struct node *leftchild;
    struct node *leftmiddle;
    struct node *rightmiddle;
    struct node *rightchild;
    struct node *parent;

    int hadsecond;
} NODE;
```

### Node Types

#### 2-Node

Contains one key:

```text
[value1]
```

#### 3-Node

Contains two keys:

```text
[value1 | value2]
```

where:

```text
value1 < value2
```

---

## Tree Properties

The implementation maintains all standard 2-3 Tree rules:

* Every internal node has either 2 or 3 children.
* All leaves are located at the same depth.
* Keys remain sorted.
* Tree height grows logarithmically.
* Automatic balancing after insertions and deletions.

---

## Supported Operations

### Insert

```c
insertion(&root, value);
```

When a node overflows:

1. A temporary third value is inserted.
2. The node is split.
3. The middle key moves upward.
4. Splitting may propagate to the root.

Functions:

```c
split()
twowayssplit()
findposition()
```

---

### Search

```c
search(value, root);
```

Returns:

* pointer to the node containing the value
* NULL if not found

Search follows the standard 2-3 Tree traversal rules.

---

### Delete

```c
deletion(&root, value);
```

Deletion handles:

* deleting from leaf nodes
* deleting internal nodes
* replacing values with predecessors
* borrowing from siblings
* merging nodes
* root replacement

Functions:

```c
basicdelete()
parentborrow2node()
parentborrow3node()
siblingborrow2node()
siblingborrow3node()
```

---

## Node Splitting

When a node already contains two values and a third value is inserted:

```text
[value1 | middle | value2]
```

the node is split into:

```text
      [middle]
      /      \
[value1]   [value2]
```

Function:

```c
twowayssplit()
```

---

## Borrowing and Merging

During deletion, underflow may occur.

The implementation resolves underflow by:

### Borrowing

Redistributing values from a sibling node.

Functions:

```c
siblingborrow2node()
siblingborrow3node()
```

### Merging

Combining nodes using a parent key.

Functions:

```c
parentborrow2node()
parentborrow3node()
```

---

## Input Format

The program reads commands from:

```text
generator.txt
```

Example:

```text
insert 50
insert 20
insert 80
search 20
delete 50
```

Supported commands:

| Command  | Description    |
| -------- | -------------- |
| insert X | Insert value X |
| search X | Search value X |
| delete X | Delete value X |

---

## Output

Execution time is appended to:

```text
graph.txt
```

Example:

```text
23 - 0.003527
```

This output can later be used for performance comparisons with other tree structures.

---

## Compilation

Compile using GCC:

```bash
gcc main.c -o twothree
```

Recommended optimization:

```bash
gcc -O2 main.c -o twothree
```

---

## Running

```bash
./twothree
```

Before execution make sure:

```text
generator.txt
```

exists in the same directory as the executable.

---

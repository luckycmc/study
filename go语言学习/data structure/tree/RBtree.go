package main

const(
	 // RED 红树设为true
	 RED bool = true
	 // BLACK 黑树设为false
	 BLACK bool = false
)
const (
    // 左旋
    LEFTROTATE bool = true
    // 右旋
    RIGHTROTATE bool = false
)

// RBNode 红黑树
type RBNode struct {
    value               int64
    color               bool
    left, right, parent *RBNode
}

type RBTree struct {
    root *RBNode
}
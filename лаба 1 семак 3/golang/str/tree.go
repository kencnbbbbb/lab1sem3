package str

import (
	"fmt"
	"strings"
)

type TreeNode struct {
	Data        int
	Left, Right *TreeNode
}
type Tree struct{ Root *TreeNode }

func (t *Tree) Insert(value int) {
	node := &TreeNode{Data: value}
	if t.Root == nil {
		t.Root = node
		return
	}
	queue := []*TreeNode{t.Root}
	for i := 0; i < len(queue); i++ {
		current := queue[i]
		if current.Left == nil {
			current.Left = node
			return
		}
		if current.Right == nil {
			current.Right = node
			return
		}
		queue = append(queue, current.Left, current.Right)
	}
}
func (t *Tree) Find(value int) bool {
	if t.Root == nil {
		return false
	}
	queue := []*TreeNode{t.Root}
	for i := 0; i < len(queue); i++ {
		current := queue[i]
		if current.Data == value {
			return true
		}
		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
	return false
}
func (t *Tree) IsComplete() bool {
	queue := []*TreeNode{t.Root}
	gap := false
	for i := 0; i < len(queue); i++ {
		current := queue[i]
		if current == nil {
			gap = true
			continue
		}
		if gap {
			return false
		}
		queue = append(queue, current.Left, current.Right)
	}
	return true
}
func (t *Tree) Read() string {
	if t.Root == nil {
		return ""
	}
	var out strings.Builder
	queue := []*TreeNode{t.Root}
	for i := 0; i < len(queue); i++ {
		current := queue[i]
		fmt.Fprintf(&out, " %d", current.Data)
		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
	return out.String()
}
func (t *Tree) Print() {
	if t.Root == nil {
		fmt.Println("(empty)")
		return
	}
	type item struct {
		node *TreeNode
		path string
	}
	queue := []item{{t.Root, "root"}}
	for i := 0; i < len(queue); i++ {
		current := queue[i]
		fmt.Printf("%s = %d\n", current.path, current.node.Data)
		if current.node.Left != nil {
			queue = append(queue, item{current.node.Left, current.path + "->left"})
		}
		if current.node.Right != nil {
			queue = append(queue, item{current.node.Right, current.path + "->right"})
		}
	}
}

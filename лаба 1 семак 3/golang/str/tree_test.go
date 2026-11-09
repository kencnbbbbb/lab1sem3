package str

import "testing"

func TestCompleteShapes(t *testing.T) {
	tree := Tree{}
	if !tree.IsComplete() {
		t.Fatal("empty tree")
	}
	for i := 0; i < 100; i++ {
		tree.Insert(i)
		if !tree.IsComplete() || !tree.Find(i) {
			t.Fatal(i)
		}
	}
	if tree.Find(-1) {
		t.Fatal("missing value found")
	}
	tree.Root = &TreeNode{Data: 1, Right: &TreeNode{Data: 2}}
	if tree.IsComplete() {
		t.Fatal("right-only child")
	}
	tree.Root = &TreeNode{Data: 1, Left: &TreeNode{Data: 2}, Right: &TreeNode{Data: 3, Left: &TreeNode{Data: 4}}}
	if tree.IsComplete() {
		t.Fatal("gap")
	}
	tree.Root = &TreeNode{Data: 1, Left: &TreeNode{Data: 2}}
	if !tree.IsComplete() {
		t.Fatal("left-only last child")
	}
}

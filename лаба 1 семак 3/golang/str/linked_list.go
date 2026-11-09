package str

import "strings"

type Node struct {
	Data string
	Next *Node
	Prev *Node
}

type LinkedList struct {
	Head *Node
	Tail *Node
}

func NewLinkedList() LinkedList {
	return LinkedList{}
}

func (l *LinkedList) PushFront(v string) {
	n := &Node{Data: v, Next: l.Head}
	if l.Head == nil {
		l.Head = n
		l.Tail = n
		return
	}
	l.Head.Prev = n
	l.Head = n
}

func (l *LinkedList) PushBack(v string) {
	n := &Node{Data: v, Prev: l.Tail}
	if l.Head == nil {
		l.Head = n
		l.Tail = n
		return
	}
	l.Tail.Next = n
	l.Tail = n
}

func (l *LinkedList) PushBefore(nextEl, v string) {
	if l.Head == nil {
		return
	}
	if nextEl == l.Head.Data {
		l.PushFront(v)
		return
	}
	cur := l.Head
	for cur != nil && cur.Data != nextEl {
		cur = cur.Next
	}
	if cur == nil {
		return
	}
	prevEl := cur.Prev
	newNode := &Node{Data: v, Next: cur, Prev: prevEl}
	prevEl.Next = newNode
	cur.Prev = newNode
}

func (l *LinkedList) PushAfter(prevEl, v string) {
	if l.Tail == nil {
		return
	}

	cur := l.Head
	for cur != nil && cur.Data != prevEl {
		cur = cur.Next
	}
	if cur == nil {
		return
	}
	if cur == l.Tail {
		l.PushBack(v)
		return
	}
	nextEl := cur.Next
	newNode := &Node{Data: v, Next: nextEl, Prev: cur}
	cur.Next = newNode
	nextEl.Prev = newNode
}

func (l *LinkedList) PopFront() string {
	if l.Head == nil {
		return ""
	}
	val := l.Head.Data
	if l.Head == l.Tail {
		l.Head = nil
		l.Tail = nil
		return val
	}
	l.Head = l.Head.Next
	l.Head.Prev = nil
	return val
}

func (l *LinkedList) PopBack() string {
	if l.Tail == nil {
		return ""
	}
	val := l.Tail.Data
	if l.Head == l.Tail {
		l.Head = nil
		l.Tail = nil
		return val
	}
	l.Tail = l.Tail.Prev
	l.Tail.Next = nil
	return val
}

func (l *LinkedList) PopBefore(nextEl string) string {
	cur := l.Head
	for cur != nil && cur.Data != nextEl {
		cur = cur.Next
	}
	if cur == nil || cur == l.Head {
		return ""
	}
	del := cur.Prev
	if del == l.Head {
		return l.PopFront()
	}
	val := del.Data
	prevEl := del.Prev
	prevEl.Next = cur
	cur.Prev = prevEl
	return val
}

func (l *LinkedList) PopAfter(prevEl string) string {
	cur := l.Head
	for cur != nil && cur.Data != prevEl {
		cur = cur.Next
	}
	if cur == nil || cur == l.Tail {
		return ""
	}
	del := cur.Next
	if del == l.Tail {
		return l.PopBack()
	}
	val := del.Data
	nextEl := del.Next
	cur.Next = nextEl
	nextEl.Prev = cur
	return val
}

func (l *LinkedList) DelByValue(val string) {
	if l.Head == nil {
		return
	}
	cur := l.Head
	for cur != nil && cur.Data != val {
		cur = cur.Next
	}
	if cur == nil {
		return
	}
	if cur == l.Head {
		l.PopFront()
		return
	}
	if cur == l.Tail {
		l.PopBack()
		return
	}
	cur.Prev.Next = cur.Next
	cur.Next.Prev = cur.Prev
}

func (l *LinkedList) Find(val string) int {
	cur := l.Head
	count := 0
	for cur != nil && cur.Data != val {
		cur = cur.Next
		count++
	}
	if cur == nil {
		return -1
	}
	return count
}

func (l *LinkedList) ReadHT() string {
	var sb strings.Builder
	cur := l.Head
	for cur != nil {
		sb.WriteString(" ")
		sb.WriteString(cur.Data)
		cur = cur.Next
	}
	return sb.String()
}

func (l *LinkedList) ReadTH() string {
	var sb strings.Builder
	cur := l.Tail
	for cur != nil {
		sb.WriteString(" ")
		sb.WriteString(cur.Data)
		cur = cur.Prev
	}
	return sb.String()
}

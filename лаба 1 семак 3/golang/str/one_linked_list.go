package str

import "strings"

type OneNode struct {
	Data string
	Next *OneNode
}

type OneLinkedList struct {
	Head *OneNode
	Tail *OneNode
}

func NewOneLinkedList() OneLinkedList {
	return OneLinkedList{}
}

func (l *OneLinkedList) PushFront(v string) {
	n := &OneNode{Data: v}
	if l.Head == nil {
		l.Head = n
		l.Tail = n
		return
	}
	n.Next = l.Head
	l.Head = n
}

func (l *OneLinkedList) PushBack(v string) {
	n := &OneNode{Data: v}
	if l.Head == nil {
		l.Head = n
		l.Tail = n
		return
	}
	l.Tail.Next = n
	l.Tail = n
}

func (l *OneLinkedList) PushAfter(prevVal, v string) {
	if l.Head == nil {
		return
	}
	prev := l.Head
	for prev != nil && prev.Data != prevVal {
		prev = prev.Next
	}
	if prev == nil {
		return
	}
	n := &OneNode{Data: v, Next: prev.Next}
	prev.Next = n
	if prev == l.Tail {
		l.Tail = n
	}
}

func (l *OneLinkedList) PushBefore(nextVal, v string) {
	if l.Head == nil {
		return
	}
	if l.Head.Data == nextVal {
		l.PushFront(v)
		return
	}
	prev := l.Head
	for prev != nil && prev.Next != nil && prev.Next.Data != nextVal {
		prev = prev.Next
	}
	if prev == nil || prev.Next == nil {
		return
	}
	n := &OneNode{Data: v, Next: prev.Next}
	prev.Next = n
}

func (l *OneLinkedList) PopFront() string {
	if l.Head == nil {
		return ""
	}
	val := l.Head.Data
	l.Head = l.Head.Next
	if l.Head == nil {
		l.Tail = nil
	}
	return val
}

func (l *OneLinkedList) PopBack() string {
	if l.Head == nil {
		return ""
	}
	val := l.Tail.Data
	if l.Head == l.Tail {
		l.Head = nil
		l.Tail = nil
		return val
	}
	cur := l.Head
	for cur.Next != l.Tail {
		cur = cur.Next
	}
	cur.Next = nil
	l.Tail = cur
	return val
}

func (l *OneLinkedList) PopAfter(prevVal string) string {
	if l.Head == nil {
		return ""
	}
	prev := l.Head
	for prev != nil && prev.Data != prevVal {
		prev = prev.Next
	}
	if prev == nil || prev.Next == nil {
		return ""
	}
	del := prev.Next
	val := del.Data
	prev.Next = del.Next
	if del == l.Tail {
		l.Tail = prev
	}
	return val
}

func (l *OneLinkedList) PopBefore(nextVal string) string {
	if l.Head == nil {
		return ""
	}
	if l.Head.Data == nextVal {
		return ""
	}
	if l.Head.Next != nil && l.Head.Next.Data == nextVal {
		return l.PopFront()
	}
	prev := l.Head
	for prev != nil && prev.Next != nil && prev.Next.Next != nil && prev.Next.Next.Data != nextVal {
		prev = prev.Next
	}
	if prev == nil || prev.Next == nil || prev.Next.Next == nil {
		return ""
	}
	del := prev.Next
	val := del.Data
	prev.Next = del.Next
	return val
}

func (l *OneLinkedList) DelByValue(val string) {
	if l.Head == nil {
		return
	}
	if l.Head.Data == val {
		l.PopFront()
		return
	}
	cur := l.Head
	for cur.Next != nil && cur.Next.Data != val {
		cur = cur.Next
	}
	if cur.Next != nil {
		temp := cur.Next
		cur.Next = temp.Next
		if temp == l.Tail {
			l.Tail = cur
		}
	}
}

func (l *OneLinkedList) FindByValue(val string) int {
	if l.Head == nil {
		return -1
	}
	count := 0
	cur := l.Head
	for cur != nil && cur.Data != val {
		cur = cur.Next
		count++
	}
	if cur != nil {
		return count
	}
	return -1
}

func (l *OneLinkedList) ReadHT() string {
	if l.Head == nil {
		return ""
	}
	var sb strings.Builder
	cur := l.Head
	for cur != l.Tail {
		sb.WriteString(" ")
		sb.WriteString(cur.Data)
		cur = cur.Next
	}
	sb.WriteString(" ")
	sb.WriteString(cur.Data)
	return sb.String()
}

func (l *OneLinkedList) ReadTH() string {
	if l.Head == nil {
		return ""
	}
	var temp []string
	cur := l.Head
	for cur != l.Tail {
		temp = append(temp, cur.Data)
		cur = cur.Next
	}
	temp = append(temp, cur.Data)
	var sb strings.Builder
	for i := len(temp) - 1; i >= 0; i-- {
		sb.WriteString(" ")
		sb.WriteString(temp[i])
	}
	return sb.String()
}

package str

type Stack struct {
	List LinkedList
}

func NewStack() Stack {
	return Stack{List: NewLinkedList()}
}

func (s *Stack) PushBack(v string) {
	s.List.PushBack(v)
}

func (s *Stack) Pop() string {
	return s.List.PopBack()
}

func (s *Stack) Read() string {
	return s.List.ReadTH()
}

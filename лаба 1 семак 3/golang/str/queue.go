package str

type Queue struct {
	List LinkedList
}

func NewQueue() Queue {
	return Queue{List: NewLinkedList()}
}

func (q *Queue) PushBack(v string) {
	q.List.PushBack(v)
}

func (q *Queue) Pop() string {
	return q.List.PopFront()
}

func (q *Queue) Read() string {
	return q.List.ReadHT()
}

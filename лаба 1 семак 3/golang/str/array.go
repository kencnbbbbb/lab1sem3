package str

import "strings"

type Array struct {
	Data []string
	Cap  int
}

func NewArray(capacity int) Array {
	if capacity < 16 {
		capacity = 16
	}
	return Array{
		Data: make([]string, 0, capacity),
		Cap:  capacity,
	}
}

func (a *Array) PushBack(v string) {
	a.Data = append(a.Data, v)
}

func (a *Array) Insert(idx int, v string) {
	if idx < 0 || idx > len(a.Data) {
		return
	}
	a.Data = append(a.Data, "")
	copy(a.Data[idx+1:], a.Data[idx:])
	a.Data[idx] = v
}

func (a *Array) Get(idx int) (string, bool) {
	if idx < 0 || idx >= len(a.Data) {
		return "", false
	}
	return a.Data[idx], true
}

func (a *Array) Set(idx int, v string) {
	if idx < 0 || idx >= len(a.Data) {
		return
	}
	a.Data[idx] = v
}

func (a *Array) Erase(idx int) {
	if idx < 0 || idx >= len(a.Data) {
		return
	}
	copy(a.Data[idx:], a.Data[idx+1:])
	a.Data = a.Data[:len(a.Data)-1]
}

func (a *Array) Length() int {
	return len(a.Data)
}

func (a *Array) Read() string {
	var sb strings.Builder
	for _, v := range a.Data {
		sb.WriteString(" ")
		sb.WriteString(v)
	}
	return sb.String()
}

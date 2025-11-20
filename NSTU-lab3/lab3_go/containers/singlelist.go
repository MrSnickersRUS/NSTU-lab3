package containers

import (
	"encoding/binary"
	"fmt"
	"io"
)

// SingleList - односвязный список
type SingleList struct {
	head  *singleNode
	count int
}

type singleNode struct {
	data int
	next *singleNode
}

// NewSingleList создаёт новый односвязный список
func NewSingleList() *SingleList {
	return &SingleList{
		head:  nil,
		count: 0,
	}
}

// AddHead добавляет элемент в начало
func (sl *SingleList) AddHead(value int) {
	newNode := &singleNode{data: value, next: sl.head}
	sl.head = newNode
	sl.count++
}

// AddTail добавляет элемент в конец
func (sl *SingleList) AddTail(value int) {
	newNode := &singleNode{data: value, next: nil}

	if sl.head == nil {
		sl.head = newNode
	} else {
		current := sl.head
		for current.next != nil {
			current = current.next
		}
		current.next = newNode
	}
	sl.count++
}

// Remove удаляет первое вхождение элемента
func (sl *SingleList) Remove(value int) bool {
	if sl.head == nil {
		return false
	}

	if sl.head.data == value {
		sl.head = sl.head.next
		sl.count--
		return true
	}

	current := sl.head
	for current.next != nil {
		if current.next.data == value {
			current.next = current.next.next
			sl.count--
			return true
		}
		current = current.next
	}
	return false
}

// Contains проверяет наличие элемента
func (sl *SingleList) Contains(value int) bool {
	current := sl.head
	for current != nil {
		if current.data == value {
			return true
		}
		current = current.next
	}
	return false
}

// Size возвращает количество элементов
func (sl *SingleList) Size() int {
	return sl.count
}

// Clear очищает список
func (sl *SingleList) Clear() {
	sl.head = nil
	sl.count = 0
}

// Print выводит список
func (sl *SingleList) Print() string {
	if sl.head == nil {
		return "[]"
	}

	result := "["
	current := sl.head
	first := true
	for current != nil {
		if !first {
			result += " -> "
		}
		result += fmt.Sprintf("%d", current.data)
		first = false
		current = current.next
	}
	result += "]"
	return result
}

// SaveToBinary сохраняет список в бинарном формате
func (sl *SingleList) SaveToBinary(w io.Writer) error {
	if err := binary.Write(w, binary.LittleEndian, int32(sl.count)); err != nil {
		return err
	}

	current := sl.head
	for current != nil {
		if err := binary.Write(w, binary.LittleEndian, int32(current.data)); err != nil {
			return err
		}
		current = current.next
	}
	return nil
}

// LoadFromBinary загружает список из бинарного формата
func (sl *SingleList) LoadFromBinary(r io.Reader) error {
	sl.Clear()

	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		var value int32
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		sl.AddTail(int(value))
	}
	return nil
}

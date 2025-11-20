package containers

import (
	"encoding/binary"
	"fmt"
	"io"
)

// DoubleList - двусвязный список
type DoubleList struct {
	head  *doubleNode
	tail  *doubleNode
	count int
}

type doubleNode struct {
	data int
	next *doubleNode
	prev *doubleNode
}

// NewDoubleList создаёт новый двусвязный список
func NewDoubleList() *DoubleList {
	return &DoubleList{
		head:  nil,
		tail:  nil,
		count: 0,
	}
}

// AddHead добавляет элемент в начало
func (dl *DoubleList) AddHead(value int) {
	newNode := &doubleNode{data: value, next: dl.head, prev: nil}

	if dl.head != nil {
		dl.head.prev = newNode
	} else {
		dl.tail = newNode
	}
	dl.head = newNode
	dl.count++
}

// AddTail добавляет элемент в конец
func (dl *DoubleList) AddTail(value int) {
	newNode := &doubleNode{data: value, next: nil, prev: dl.tail}

	if dl.tail != nil {
		dl.tail.next = newNode
	} else {
		dl.head = newNode
	}
	dl.tail = newNode
	dl.count++
}

// Remove удаляет первое вхождение элемента
func (dl *DoubleList) Remove(value int) bool {
	current := dl.head

	for current != nil {
		if current.data == value {
			if current.prev != nil {
				current.prev.next = current.next
			} else {
				dl.head = current.next
			}

			if current.next != nil {
				current.next.prev = current.prev
			} else {
				dl.tail = current.prev
			}

			dl.count--
			return true
		}
		current = current.next
	}
	return false
}

// Contains проверяет наличие элемента
func (dl *DoubleList) Contains(value int) bool {
	current := dl.head
	for current != nil {
		if current.data == value {
			return true
		}
		current = current.next
	}
	return false
}

// Size возвращает количество элементов
func (dl *DoubleList) Size() int {
	return dl.count
}

// Clear очищает список
func (dl *DoubleList) Clear() {
	dl.head = nil
	dl.tail = nil
	dl.count = 0
}

// Print выводит список
func (dl *DoubleList) Print() string {
	if dl.head == nil {
		return "[]"
	}

	result := "["
	current := dl.head
	first := true
	for current != nil {
		if !first {
			result += " <-> "
		}
		result += fmt.Sprintf("%d", current.data)
		first = false
		current = current.next
	}
	result += "]"
	return result
}

// SaveToBinary сохраняет список в бинарном формате
func (dl *DoubleList) SaveToBinary(w io.Writer) error {
	if err := binary.Write(w, binary.LittleEndian, int32(dl.count)); err != nil {
		return err
	}

	current := dl.head
	for current != nil {
		if err := binary.Write(w, binary.LittleEndian, int32(current.data)); err != nil {
			return err
		}
		current = current.next
	}
	return nil
}

// LoadFromBinary загружает список из бинарного формата
func (dl *DoubleList) LoadFromBinary(r io.Reader) error {
	dl.Clear()

	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		var value int32
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		dl.AddTail(int(value))
	}
	return nil
}

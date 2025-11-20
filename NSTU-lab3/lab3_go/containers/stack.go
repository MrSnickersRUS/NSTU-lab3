package containers

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io"
)

// Stack - стек (LIFO)
type Stack struct {
	data     []int
	capacity int
	top      int
}

// NewStack создаёт новый стек
func NewStack(initialCapacity int) *Stack {
	if initialCapacity <= 0 {
		initialCapacity = 10
	}
	return &Stack{
		data:     make([]int, initialCapacity),
		capacity: initialCapacity,
		top:      -1,
	}
}

// Push добавляет элемент на вершину стека
func (s *Stack) Push(value int) {
	if s.top+1 >= s.capacity {
		s.resize(s.capacity * 2)
	}
	s.top++
	s.data[s.top] = value
}

// Pop удаляет и возвращает элемент с вершины
func (s *Stack) Pop() (int, error) {
	if s.top < 0 {
		return 0, fmt.Errorf("стек пуст")
	}
	value := s.data[s.top]
	s.top--
	return value, nil
}

// Peek возвращает элемент с вершины без удаления
func (s *Stack) Peek() (int, error) {
	if s.top < 0 {
		return 0, fmt.Errorf("стек пуст")
	}
	return s.data[s.top], nil
}

// Size возвращает количество элементов
func (s *Stack) Size() int {
	return s.top + 1
}

// Clear очищает стек
func (s *Stack) Clear() {
	s.top = -1
}

// Print выводит стек
func (s *Stack) Print() string {
	if s.top < 0 {
		return "[]"
	}
	result := "["
	for i := 0; i <= s.top; i++ {
		if i > 0 {
			result += ", "
		}
		result += fmt.Sprintf("%d", s.data[i])
	}
	result += "]"
	return result
}

// resize увеличивает ёмкость стека
func (s *Stack) resize(newCapacity int) {
	newData := make([]int, newCapacity)
	copy(newData, s.data[:s.top+1])
	s.data = newData
	s.capacity = newCapacity
}

// SaveToBinary сохраняет стек в бинарном формате
func (s *Stack) SaveToBinary(w io.Writer) error {
	size := s.Size()
	if err := binary.Write(w, binary.LittleEndian, int32(size)); err != nil {
		return err
	}
	for i := 0; i < size; i++ {
		if err := binary.Write(w, binary.LittleEndian, int32(s.data[i])); err != nil {
			return err
		}
	}
	return nil
}

// LoadFromBinary загружает стек из бинарного формата
func (s *Stack) LoadFromBinary(r io.Reader) error {
	s.Clear()

	var size int32
	if err := binary.Read(r, binary.LittleEndian, &size); err != nil {
		return err
	}

	for i := 0; i < int(size); i++ {
		var value int32
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		s.Push(int(value))
	}
	return nil
}

// ToJSON сериализует стек в JSON
func (s *Stack) ToJSON() (string, error) {
	data := s.data[:s.top+1]
	bytes, err := json.Marshal(data)
	if err != nil {
		return "", err
	}
	return string(bytes), nil
}

// FromJSON десериализует стек из JSON
func (s *Stack) FromJSON(jsonStr string) error {
	s.Clear()
	var data []int
	if err := json.Unmarshal([]byte(jsonStr), &data); err != nil {
		return err
	}
	for _, val := range data {
		s.Push(val)
	}
	return nil
}

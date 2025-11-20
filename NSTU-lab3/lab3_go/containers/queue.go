package containers

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io"
)

// Queue - очередь (FIFO)
type Queue struct {
	data     []int
	capacity int
	head     int
	tail     int
	size     int
}

// NewQueue создаёт новую очередь
func NewQueue(initialCapacity int) *Queue {
	if initialCapacity <= 0 {
		initialCapacity = 10
	}
	return &Queue{
		data:     make([]int, initialCapacity),
		capacity: initialCapacity,
		head:     0,
		tail:     0,
		size:     0,
	}
}

// Enqueue добавляет элемент в конец очереди
func (q *Queue) Enqueue(value int) {
	if q.size >= q.capacity {
		q.resize(q.capacity * 2)
	}
	q.data[q.tail] = value
	q.tail = (q.tail + 1) % q.capacity
	q.size++
}

// Dequeue удаляет и возвращает элемент из начала очереди
func (q *Queue) Dequeue() (int, error) {
	if q.size == 0 {
		return 0, fmt.Errorf("очередь пуста")
	}
	value := q.data[q.head]
	q.head = (q.head + 1) % q.capacity
	q.size--
	return value, nil
}

// Peek возвращает элемент из начала очереди без удаления
func (q *Queue) Peek() (int, error) {
	if q.size == 0 {
		return 0, fmt.Errorf("очередь пуста")
	}
	return q.data[q.head], nil
}

// Size возвращает количество элементов
func (q *Queue) Size() int {
	return q.size
}

// Clear очищает очередь
func (q *Queue) Clear() {
	q.head = 0
	q.tail = 0
	q.size = 0
}

// Print выводит очередь
func (q *Queue) Print() string {
	if q.size == 0 {
		return "[]"
	}
	result := "["
	idx := q.head
	for i := 0; i < q.size; i++ {
		if i > 0 {
			result += ", "
		}
		result += fmt.Sprintf("%d", q.data[idx])
		idx = (idx + 1) % q.capacity
	}
	result += "]"
	return result
}

// resize увеличивает ёмкость очереди
func (q *Queue) resize(newCapacity int) {
	newData := make([]int, newCapacity)
	idx := q.head
	for i := 0; i < q.size; i++ {
		newData[i] = q.data[idx]
		idx = (idx + 1) % q.capacity
	}
	q.data = newData
	q.capacity = newCapacity
	q.head = 0
	q.tail = q.size
}

// SaveToBinary сохраняет очередь в бинарном формате
func (q *Queue) SaveToBinary(w io.Writer) error {
	if err := binary.Write(w, binary.LittleEndian, int32(q.size)); err != nil {
		return err
	}
	idx := q.head
	for i := 0; i < q.size; i++ {
		if err := binary.Write(w, binary.LittleEndian, int32(q.data[idx])); err != nil {
			return err
		}
		idx = (idx + 1) % q.capacity
	}
	return nil
}

// LoadFromBinary загружает очередь из бинарного формата
func (q *Queue) LoadFromBinary(r io.Reader) error {
	q.Clear()

	var size int32
	if err := binary.Read(r, binary.LittleEndian, &size); err != nil {
		return err
	}

	for i := 0; i < int(size); i++ {
		var value int32
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		q.Enqueue(int(value))
	}
	return nil
}

// ToJSON сериализует очередь в JSON
func (q *Queue) ToJSON() (string, error) {
	data := make([]int, q.size)
	idx := q.head
	for i := 0; i < q.size; i++ {
		data[i] = q.data[idx]
		idx = (idx + 1) % q.capacity
	}
	bytes, err := json.Marshal(data)
	if err != nil {
		return "", err
	}
	return string(bytes), nil
}

// FromJSON десериализует очередь из JSON
func (q *Queue) FromJSON(jsonStr string) error {
	q.Clear()
	var data []int
	if err := json.Unmarshal([]byte(jsonStr), &data); err != nil {
		return err
	}
	for _, val := range data {
		q.Enqueue(val)
	}
	return nil
}

package containers

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"io"
)

// Array - динамический массив
type Array struct {
	data     []int
	capacity int
	length   int
}

// NewArray создаёт новый массив
func NewArray(initialCapacity int) *Array {
	if initialCapacity <= 0 {
		initialCapacity = 10
	}
	return &Array{
		data:     make([]int, initialCapacity),
		capacity: initialCapacity,
		length:   0,
	}
}

// Push добавляет элемент в конец
func (a *Array) Push(value int) {
	if a.length >= a.capacity {
		a.resize(a.capacity * 2)
	}
	a.data[a.length] = value
	a.length++
}

// Get возвращает элемент по индексу
func (a *Array) Get(index int) (int, error) {
	if index < 0 || index >= a.length {
		return 0, fmt.Errorf("индекс массива выходит за границы")
	}
	return a.data[index], nil
}

// Insert вставляет элемент по индексу
func (a *Array) Insert(index int, value int) error {
	if index < 0 || index > a.length {
		return fmt.Errorf("индекс массива выходит за границы")
	}
	if a.length >= a.capacity {
		a.resize(a.capacity * 2)
	}
	// Сдвигаем элементы вправо
	for i := a.length; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = value
	a.length++
	return nil
}

// Remove удаляет элемент по индексу
func (a *Array) Remove(index int) error {
	if index < 0 || index >= a.length {
		return fmt.Errorf("индекс массива выходит за границы")
	}
	// Сдвигаем элементы влево
	for i := index; i < a.length-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.length--
	return nil
}

// Replace заменяет элемент по индексу
func (a *Array) Replace(index int, value int) error {
	if index < 0 || index >= a.length {
		return fmt.Errorf("индекс массива выходит за границы")
	}
	a.data[index] = value
	return nil
}

// Size возвращает количество элементов
func (a *Array) Size() int {
	return a.length
}

// Clear очищает массив
func (a *Array) Clear() {
	a.length = 0
}

// Print выводит массив
func (a *Array) Print() string {
	if a.length == 0 {
		return "[]"
	}
	result := "["
	for i := 0; i < a.length; i++ {
		if i > 0 {
			result += ", "
		}
		result += fmt.Sprintf("%d", a.data[i])
	}
	result += "]"
	return result
}

// resize увеличивает ёмкость массива
func (a *Array) resize(newCapacity int) {
	newData := make([]int, newCapacity)
	copy(newData, a.data[:a.length])
	a.data = newData
	a.capacity = newCapacity
}

// SaveToBinary сохраняет массив в бинарном формате
func (a *Array) SaveToBinary(w io.Writer) error {
	// Записываем длину
	if err := binary.Write(w, binary.LittleEndian, int32(a.length)); err != nil {
		return err
	}
	// Записываем элементы
	for i := 0; i < a.length; i++ {
		if err := binary.Write(w, binary.LittleEndian, int32(a.data[i])); err != nil {
			return err
		}
	}
	return nil
}

// LoadFromBinary загружает массив из бинарного формата
func (a *Array) LoadFromBinary(r io.Reader) error {
	a.Clear()

	// Читаем длину
	var length int32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return err
	}

	// Читаем элементы
	for i := 0; i < int(length); i++ {
		var value int32
		if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
			return err
		}
		a.Push(int(value))
	}
	return nil
}

// ToJSON сериализует массив в JSON
func (a *Array) ToJSON() (string, error) {
	data := a.data[:a.length]
	bytes, err := json.Marshal(data)
	if err != nil {
		return "", err
	}
	return string(bytes), nil
}

// FromJSON десериализует массив из JSON
func (a *Array) FromJSON(jsonStr string) error {
	a.Clear()
	var data []int
	if err := json.Unmarshal([]byte(jsonStr), &data); err != nil {
		return err
	}
	for _, val := range data {
		a.Push(val)
	}
	return nil
}

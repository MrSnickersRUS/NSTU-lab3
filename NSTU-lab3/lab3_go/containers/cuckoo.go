package containers

import (
	"fmt"
	"hash/fnv"
)

// CuckooHashMap - хеш-таблица с кукушкой хешированием
type CuckooHashMap struct {
	table1   []cuckooEntry
	table2   []cuckooEntry
	capacity int
	count    int
}

type cuckooEntry struct {
	key      string
	value    string
	occupied bool
}

const (
	cuckooDefaultCapacity = 101
	maxCuckooIterations   = 500
)

// NewCuckooHashMap создаёт новую хеш-таблицу с кукушкой
func NewCuckooHashMap(initialCapacity int) *CuckooHashMap {
	if initialCapacity <= 0 {
		initialCapacity = cuckooDefaultCapacity
	}
	return &CuckooHashMap{
		table1:   make([]cuckooEntry, initialCapacity),
		table2:   make([]cuckooEntry, initialCapacity),
		capacity: initialCapacity,
		count:    0,
	}
}

// hash1 первая хеш-функция
func (ch *CuckooHashMap) hash1(key string) int {
	hasher := fnv.New32a()
	hasher.Write([]byte(key))
	return int(hasher.Sum32()) % ch.capacity
}

// hash2 вторая хеш-функция
func (ch *CuckooHashMap) hash2(key string) int {
	hasher := fnv.New32a()
	hasher.Write([]byte(key))
	return int(hasher.Sum32()/uint32(ch.capacity)) % ch.capacity
}

// insertInternal вспомогательная функция для вставки с ограничением итераций
func (ch *CuckooHashMap) insertInternal(key string, value string, iterations int) error {
	if iterations >= maxCuckooIterations {
		return fmt.Errorf("слишком много итераций, требуется рехеширование")
	}

	idx1 := ch.hash1(key)

	// Пробуем table1
	if !ch.table1[idx1].occupied {
		ch.table1[idx1] = cuckooEntry{key: key, value: value, occupied: true}
		return nil
	}

	// Если ключ уже есть в table1, обновляем
	if ch.table1[idx1].key == key {
		ch.table1[idx1].value = value
		return nil
	}

	// Вытесняем из table1
	oldKey := ch.table1[idx1].key
	oldValue := ch.table1[idx1].value
	ch.table1[idx1] = cuckooEntry{key: key, value: value, occupied: true}

	idx2 := ch.hash2(oldKey)

	// Пробуем table2
	if !ch.table2[idx2].occupied {
		ch.table2[idx2] = cuckooEntry{key: oldKey, value: oldValue, occupied: true}
		return nil
	}

	// Если ключ уже есть в table2, обновляем
	if ch.table2[idx2].key == oldKey {
		ch.table2[idx2].value = oldValue
		return nil
	}

	// Вытесняем из table2
	nextKey := ch.table2[idx2].key
	nextValue := ch.table2[idx2].value
	ch.table2[idx2] = cuckooEntry{key: oldKey, value: oldValue, occupied: true}

	// Рекурсивно вставляем вытесненный элемент
	return ch.insertInternal(nextKey, nextValue, iterations+1)
}

// Put добавляет или обновляет пару ключ-значение
func (ch *CuckooHashMap) Put(key string, value string) error {
	if ch.count >= ch.capacity*2 {
		return fmt.Errorf("хеш-таблица заполнена")
	}

	// Проверяем, есть ли уже такой ключ
	if ch.Contains(key) {
		idx1 := ch.hash1(key)
		idx2 := ch.hash2(key)

		if ch.table1[idx1].occupied && ch.table1[idx1].key == key {
			ch.table1[idx1].value = value
			return nil
		}
		if ch.table2[idx2].occupied && ch.table2[idx2].key == key {
			ch.table2[idx2].value = value
			return nil
		}
	}

	err := ch.insertInternal(key, value, 0)
	if err == nil {
		ch.count++
	}
	return err
}

// Get возвращает значение по ключу
func (ch *CuckooHashMap) Get(key string) (string, error) {
	idx1 := ch.hash1(key)
	idx2 := ch.hash2(key)

	if ch.table1[idx1].occupied && ch.table1[idx1].key == key {
		return ch.table1[idx1].value, nil
	}
	if ch.table2[idx2].occupied && ch.table2[idx2].key == key {
		return ch.table2[idx2].value, nil
	}
	return "", fmt.Errorf("ключ не найден")
}

// Contains проверяет наличие ключа
func (ch *CuckooHashMap) Contains(key string) bool {
	idx1 := ch.hash1(key)
	idx2 := ch.hash2(key)

	return (ch.table1[idx1].occupied && ch.table1[idx1].key == key) ||
		(ch.table2[idx2].occupied && ch.table2[idx2].key == key)
}

// Remove удаляет пару по ключу
func (ch *CuckooHashMap) Remove(key string) bool {
	idx1 := ch.hash1(key)
	idx2 := ch.hash2(key)

	if ch.table1[idx1].occupied && ch.table1[idx1].key == key {
		ch.table1[idx1].occupied = false
		ch.count--
		return true
	}
	if ch.table2[idx2].occupied && ch.table2[idx2].key == key {
		ch.table2[idx2].occupied = false
		ch.count--
		return true
	}
	return false
}

// Clear очищает таблицу
func (ch *CuckooHashMap) Clear() {
	for i := range ch.table1 {
		ch.table1[i].occupied = false
	}
	for i := range ch.table2 {
		ch.table2[i].occupied = false
	}
	ch.count = 0
}

// Size возвращает количество элементов
func (ch *CuckooHashMap) Size() int {
	return ch.count
}

// Print выводит хеш-таблицу
func (ch *CuckooHashMap) Print() string {
	result := "CuckooHashMap {\n  Table 1:\n"
	for i, e := range ch.table1 {
		if e.occupied {
			result += fmt.Sprintf("    [%d] %s => %s\n", i, e.key, e.value)
		}
	}
	result += "  Table 2:\n"
	for i, e := range ch.table2 {
		if e.occupied {
			result += fmt.Sprintf("    [%d] %s => %s\n", i, e.key, e.value)
		}
	}
	result += fmt.Sprintf("} (size: %d)", ch.count)
	return result
}

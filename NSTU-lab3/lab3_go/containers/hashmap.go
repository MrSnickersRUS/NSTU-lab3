package containers

import (
	"encoding/binary"
	"encoding/json"
	"fmt"
	"hash/fnv"
	"io"
)

// HashMap - хеш-таблица с двойным хешированием
type HashMap struct {
	table    []entry
	capacity int
	count    int
}

type entry struct {
	key      string
	value    string
	occupied bool
}

const defaultCapacity = 101

// NewHashMap создаёт новую хеш-таблицу
func NewHashMap(initialCapacity int) *HashMap {
	if initialCapacity <= 0 {
		initialCapacity = defaultCapacity
	}
	return &HashMap{
		table:    make([]entry, initialCapacity),
		capacity: initialCapacity,
		count:    0,
	}
}

// hash1 первая хеш-функция
func (h *HashMap) hash1(key string) int {
	hasher := fnv.New32a()
	hasher.Write([]byte(key))
	return int(hasher.Sum32()) % h.capacity
}

// hash2 вторая хеш-функция (для двойного хеширования)
func (h *HashMap) hash2(key string) int {
	hasher := fnv.New32a()
	hasher.Write([]byte(key))
	h2 := int(hasher.Sum32()) % (h.capacity - 1)
	if h2 == 0 {
		return 1
	}
	return h2
}

// findIndex ищет индекс для ключа
func (h *HashMap) findIndex(key string) (int, bool) {
	h1 := h.hash1(key)
	h2 := h.hash2(key)
	index := h1

	for i := 0; i < h.capacity; i++ {
		if !h.table[index].occupied {
			return index, false
		}
		if h.table[index].key == key {
			return index, true
		}
		index = (h1 + (i+1)*h2) % h.capacity
	}
	return -1, false
}

// Put добавляет или обновляет пару ключ-значение
func (h *HashMap) Put(key string, value string) error {
	if h.count >= h.capacity {
		return fmt.Errorf("хеш-таблица заполнена")
	}

	index, found := h.findIndex(key)
	if index == -1 {
		return fmt.Errorf("хеш-таблица заполнена")
	}

	if found {
		h.table[index].value = value
	} else {
		h.table[index] = entry{key: key, value: value, occupied: true}
		h.count++
	}
	return nil
}

// Get возвращает значение по ключу
func (h *HashMap) Get(key string) (string, error) {
	index, found := h.findIndex(key)
	if !found {
		return "", fmt.Errorf("ключ не найден")
	}
	return h.table[index].value, nil
}

// Contains проверяет наличие ключа
func (h *HashMap) Contains(key string) bool {
	_, found := h.findIndex(key)
	return found
}

// Remove удаляет пару по ключу
func (h *HashMap) Remove(key string) bool {
	index, found := h.findIndex(key)
	if !found {
		return false
	}
	h.table[index].occupied = false
	h.count--
	return true
}

// Clear очищает таблицу
func (h *HashMap) Clear() {
	for i := range h.table {
		h.table[i].occupied = false
	}
	h.count = 0
}

// Size возвращает количество элементов
func (h *HashMap) Size() int {
	return h.count
}

// Print выводит хеш-таблицу
func (h *HashMap) Print() string {
	result := "HashMap {\n"
	for i, e := range h.table {
		if e.occupied {
			result += fmt.Sprintf("  [%d] %s => %s\n", i, e.key, e.value)
		}
	}
	result += fmt.Sprintf("} (size: %d)", h.count)
	return result
}

// SaveToBinary сохраняет хеш-таблицу в бинарном формате
func (h *HashMap) SaveToBinary(w io.Writer) error {
	if err := binary.Write(w, binary.LittleEndian, int32(h.count)); err != nil {
		return err
	}

	for _, e := range h.table {
		if e.occupied {
			// Записываем длину ключа и сам ключ
			if err := binary.Write(w, binary.LittleEndian, int32(len(e.key))); err != nil {
				return err
			}
			if _, err := w.Write([]byte(e.key)); err != nil {
				return err
			}

			// Записываем длину значения и само значение
			if err := binary.Write(w, binary.LittleEndian, int32(len(e.value))); err != nil {
				return err
			}
			if _, err := w.Write([]byte(e.value)); err != nil {
				return err
			}
		}
	}
	return nil
}

// LoadFromBinary загружает хеш-таблицу из бинарного формата
func (h *HashMap) LoadFromBinary(r io.Reader) error {
	h.Clear()

	var count int32
	if err := binary.Read(r, binary.LittleEndian, &count); err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		// Читаем ключ
		var keyLen int32
		if err := binary.Read(r, binary.LittleEndian, &keyLen); err != nil {
			return err
		}
		keyBytes := make([]byte, keyLen)
		if _, err := io.ReadFull(r, keyBytes); err != nil {
			return err
		}

		// Читаем значение
		var valueLen int32
		if err := binary.Read(r, binary.LittleEndian, &valueLen); err != nil {
			return err
		}
		valueBytes := make([]byte, valueLen)
		if _, err := io.ReadFull(r, valueBytes); err != nil {
			return err
		}

		if err := h.Put(string(keyBytes), string(valueBytes)); err != nil {
			return err
		}
	}
	return nil
}

// ToJSON сериализует HashMap в JSON
func (h *HashMap) ToJSON() (string, error) {
	data := make(map[string]string)
	for i := 0; i < h.capacity; i++ {
		if h.table[i].occupied {
			data[h.table[i].key] = h.table[i].value
		}
	}
	bytes, err := json.Marshal(data)
	if err != nil {
		return "", err
	}
	return string(bytes), nil
}

// FromJSON десериализует HashMap из JSON
func (h *HashMap) FromJSON(jsonStr string) error {
	h.Clear()
	var data map[string]string
	if err := json.Unmarshal([]byte(jsonStr), &data); err != nil {
		return err
	}
	for key, value := range data {
		if err := h.Put(key, value); err != nil {
			return err
		}
	}
	return nil
}

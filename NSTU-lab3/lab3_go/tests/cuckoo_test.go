package containers_test

import (
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestCuckoo_PutGet(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	err := ch.Put("key1", "value1")
	require.NoError(t, err)

	val, err := ch.Get("key1")
	assert.NoError(t, err)
	assert.Equal(t, "value1", val)
}

func TestCuckoo_Update(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	ch.Put("key", "value1")
	ch.Put("key", "value2")

	val, err := ch.Get("key")
	assert.NoError(t, err)
	assert.Equal(t, "value2", val)
	assert.Equal(t, 1, ch.Size())
}

func TestCuckoo_Remove(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	ch.Put("key", "value")

	removed := ch.Remove("key")
	assert.True(t, removed)
	assert.Equal(t, 0, ch.Size())

	_, err := ch.Get("key")
	assert.Error(t, err)
}

func TestCuckoo_Contains(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	ch.Put("test", "data")

	assert.True(t, ch.Contains("test"))
	assert.False(t, ch.Contains("missing"))
}

func TestCuckoo_GetNonExistent(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	_, err := ch.Get("nonexistent")
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "не найден")
}

func TestCuckoo_RemoveNonExistent(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	removed := ch.Remove("nonexistent")
	assert.False(t, removed)
}

func TestCuckoo_MultipleInserts(t *testing.T) {
	ch := containers.NewCuckooHashMap(10)
	for i := 0; i < 15; i++ {
		key := string(rune('a' + i))
		err := ch.Put(key, key+"_val")
		if err == nil {
			assert.True(t, ch.Contains(key))
		}
	}
}

func TestCuckoo_Clear(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	ch.Put("key1", "val1")
	ch.Put("key2", "val2")

	ch.Clear()
	assert.Equal(t, 0, ch.Size())
	assert.False(t, ch.Contains("key1"))
}

func TestCuckoo_Print(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	result := ch.Print()
	assert.Contains(t, result, "CuckooHashMap")
	assert.Contains(t, result, "size: 0")

	ch.Put("key", "value")
	result = ch.Print()
	assert.Contains(t, result, "key")
	assert.Contains(t, result, "value")
}

func TestCuckoo_FullTable(t *testing.T) {
	ch := containers.NewCuckooHashMap(5)

	// Пытаемся заполнить до предела
	for i := 0; i < 10; i++ {
		key := string(rune('a' + i))
		ch.Put(key, "val")
	}

	// Проверяем, что таблица заполнена
	err := ch.Put("overflow", "value")
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "заполнена")
}

func TestCuckoo_CuckooingBehavior(t *testing.T) {
	ch := containers.NewCuckooHashMap(10)

	// Вставляем элементы, которые могут вызвать вытеснение
	keys := []string{"a", "b", "c", "d", "e", "f", "g", "h"}
	for _, key := range keys {
		err := ch.Put(key, key+"_val")
		if err != nil {
			// Если возникла ошибка из-за циклов, это нормально
			t.Logf("Cuckoo cycle detected for key %s", key)
		}
	}

	// Проверяем, что вставленные элементы можно найти
	for _, key := range keys {
		if ch.Contains(key) {
			val, err := ch.Get(key)
			assert.NoError(t, err)
			assert.Equal(t, key+"_val", val)
		}
	}
}

func TestCuckoo_EmptyOperations(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	assert.Equal(t, 0, ch.Size())
	assert.False(t, ch.Contains("test"))
	assert.False(t, ch.Remove("test"))
}

func TestCuckoo_UpdateInTable1(t *testing.T) {
	ch := containers.NewCuckooHashMap(101)
	ch.Put("key", "value1")

	// Обновляем значение
	ch.Put("key", "value2")

	val, _ := ch.Get("key")
	assert.Equal(t, "value2", val)
}

func TestCuckoo_RehashingScenario(t *testing.T) {
	ch := containers.NewCuckooHashMap(5)

	// Вставляем элементы до тех пор, пока не получим ошибку
	count := 0
	for i := 0; i < 100; i++ {
		key := string(rune('a'+(i%26))) + string(rune('0'+(i/26)))
		err := ch.Put(key, "value")
		if err != nil {
			if count < 5 {
				t.Logf("Error at insertion %d: %v", i, err)
			}
			break
		}
		count++
	}

	t.Logf("Successfully inserted %d elements before failure", count)
	assert.Greater(t, count, 0, "Should insert at least some elements")
}

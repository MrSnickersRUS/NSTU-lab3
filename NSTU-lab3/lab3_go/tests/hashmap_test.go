package containers_test

import (
	"bytes"
	"fmt"
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestHashMap_PutGet(t *testing.T) {
	hm := containers.NewHashMap(101)
	err := hm.Put("ключ1", "значение1")
	require.NoError(t, err)

	val, err := hm.Get("ключ1")
	assert.NoError(t, err)
	assert.Equal(t, "значение1", val)
}

func TestHashMap_Update(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("key", "value1")
	hm.Put("key", "value2")

	val, err := hm.Get("key")
	assert.NoError(t, err)
	assert.Equal(t, "value2", val)
	assert.Equal(t, 1, hm.Size())
}

func TestHashMap_Remove(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("key", "value")

	removed := hm.Remove("key")
	assert.True(t, removed)
	assert.Equal(t, 0, hm.Size())

	_, err := hm.Get("key")
	assert.Error(t, err)
}

func TestHashMap_Contains(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("test", "data")

	assert.True(t, hm.Contains("test"))
	assert.False(t, hm.Contains("missing"))
}

func TestHashMap_GetNonExistent(t *testing.T) {
	hm := containers.NewHashMap(101)
	_, err := hm.Get("nonexistent")
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "не найден")
}

func TestHashMap_RemoveNonExistent(t *testing.T) {
	hm := containers.NewHashMap(101)
	removed := hm.Remove("nonexistent")
	assert.False(t, removed)
}

func TestHashMap_MultipleCollisions(t *testing.T) {
	hm := containers.NewHashMap(5) // маленькая таблица для коллизий
	for i := 0; i < 4; i++ {
		key := fmt.Sprintf("key%d", i)
		err := hm.Put(key, fmt.Sprintf("value%d", i))
		assert.NoError(t, err)
	}
	assert.Equal(t, 4, hm.Size())
}

func TestHashMap_UpdateExisting(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("key", "value1")
	hm.Put("key", "value2")
	hm.Put("key", "value3")

	val, _ := hm.Get("key")
	assert.Equal(t, "value3", val)
	assert.Equal(t, 1, hm.Size())
}

func TestHashMap_ClearAndReuse(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("a", "1")
	hm.Put("b", "2")
	hm.Clear()

	err := hm.Put("c", "3")
	assert.NoError(t, err)
	assert.Equal(t, 1, hm.Size())
}

func TestHashMap_Print(t *testing.T) {
	hm := containers.NewHashMap(101)
	result := hm.Print()
	assert.Contains(t, result, "HashMap")
	assert.Contains(t, result, "size: 0")

	hm.Put("key", "value")
	result = hm.Print()
	assert.Contains(t, result, "key")
	assert.Contains(t, result, "value")
}

func TestHashMap_FullTable(t *testing.T) {
	hm := containers.NewHashMap(3)
	hm.Put("a", "1")
	hm.Put("b", "2")
	hm.Put("c", "3")

	err := hm.Put("d", "4")
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "заполнена")
}

func TestHashMap_Clear(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("key1", "val1")
	hm.Put("key2", "val2")

	hm.Clear()
	assert.Equal(t, 0, hm.Size())
	assert.False(t, hm.Contains("key1"))
}

func TestHashMap_BinarySerialization(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("привет", "мир")
	hm.Put("hello", "world")

	var buf bytes.Buffer
	err := hm.SaveToBinary(&buf)
	require.NoError(t, err)

	hm2 := containers.NewHashMap(101)
	err = hm2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, hm.Size(), hm2.Size())
	val, _ := hm2.Get("привет")
	assert.Equal(t, "мир", val)
}

func TestHashMap_EmptySerialization(t *testing.T) {
	hm := containers.NewHashMap(101)

	var buf bytes.Buffer
	err := hm.SaveToBinary(&buf)
	require.NoError(t, err)

	hm2 := containers.NewHashMap(101)
	err = hm2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 0, hm2.Size())
}

func TestHashMap_SerializationLargeData(t *testing.T) {
	hm := containers.NewHashMap(101)
	for i := 0; i < 50; i++ {
		hm.Put(fmt.Sprintf("key%d", i), fmt.Sprintf("value%d", i))
	}

	var buf bytes.Buffer
	err := hm.SaveToBinary(&buf)
	require.NoError(t, err)

	hm2 := containers.NewHashMap(101)
	err = hm2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 50, hm2.Size())
	for i := 0; i < 50; i++ {
		val, err := hm2.Get(fmt.Sprintf("key%d", i))
		assert.NoError(t, err)
		assert.Equal(t, fmt.Sprintf("value%d", i), val)
	}
}

func TestHashMap_JSONSerialization(t *testing.T) {
	hm := containers.NewHashMap(101)
	hm.Put("name", "Alice")
	hm.Put("age", "30")
	hm.Put("city", "Moscow")

	jsonStr, err := hm.ToJSON()
	require.NoError(t, err)
	assert.Contains(t, jsonStr, "Alice")
	assert.Contains(t, jsonStr, "30")
	assert.Contains(t, jsonStr, "Moscow")

	hm2 := containers.NewHashMap(101)
	err = hm2.FromJSON(jsonStr)
	require.NoError(t, err)

	assert.Equal(t, 3, hm2.Size())
	val, _ := hm2.Get("name")
	assert.Equal(t, "Alice", val)
	val, _ = hm2.Get("age")
	assert.Equal(t, "30", val)
	val, _ = hm2.Get("city")
	assert.Equal(t, "Moscow", val)
}

func TestHashMap_JSONEmpty(t *testing.T) {
	hm := containers.NewHashMap(101)
	jsonStr, err := hm.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "{}", jsonStr)
}

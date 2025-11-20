package containers_test

import (
	"bytes"
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDoubleList_AddHead(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(10)
	list.AddHead(20)
	list.AddHead(30)

	assert.Equal(t, 3, list.Size())
	assert.True(t, list.Contains(10))
	assert.True(t, list.Contains(20))
	assert.True(t, list.Contains(30))
}

func TestDoubleList_AddTail(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddTail(10)
	list.AddTail(20)
	list.AddTail(30)

	assert.Equal(t, 3, list.Size())
	assert.True(t, list.Contains(10))
	assert.True(t, list.Contains(20))
	assert.True(t, list.Contains(30))
}

func TestDoubleList_Remove(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(10)
	list.AddHead(20)
	list.AddHead(30)

	result := list.Remove(20)
	assert.True(t, result)
	assert.Equal(t, 2, list.Size())
	assert.False(t, list.Contains(20))
}

func TestDoubleList_RemoveNonExistent(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(10)

	result := list.Remove(999)
	assert.False(t, result)
	assert.Equal(t, 1, list.Size())
}

func TestDoubleList_RemoveHead(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(10)
	list.AddHead(20)
	list.AddHead(30)

	list.Remove(30) // удаляем голову
	assert.Equal(t, 2, list.Size())
	assert.False(t, list.Contains(30))
}

func TestDoubleList_RemoveTail(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddTail(10)
	list.AddTail(20)
	list.AddTail(30)

	list.Remove(30) // удаляем хвост
	assert.Equal(t, 2, list.Size())
	assert.False(t, list.Contains(30))
}

func TestDoubleList_Contains(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(42)

	assert.True(t, list.Contains(42))
	assert.False(t, list.Contains(999))
}

func TestDoubleList_Clear(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(1)
	list.AddHead(2)
	list.AddHead(3)

	list.Clear()
	assert.Equal(t, 0, list.Size())
	assert.False(t, list.Contains(1))
}

func TestDoubleList_Print(t *testing.T) {
	list := containers.NewDoubleList()
	result := list.Print()
	assert.Equal(t, "[]", result)

	list.AddHead(10)
	list.AddHead(20)
	result = list.Print()
	assert.Contains(t, result, "20")
	assert.Contains(t, result, "10")
}

func TestDoubleList_EmptyOperations(t *testing.T) {
	list := containers.NewDoubleList()
	assert.Equal(t, 0, list.Size())
	assert.False(t, list.Contains(1))
	assert.False(t, list.Remove(1))
}

func TestDoubleList_MixedOperations(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(1)
	list.AddTail(2)
	list.AddHead(0)
	list.AddTail(3)

	assert.Equal(t, 4, list.Size())

	list.Remove(1)
	list.Remove(2)

	assert.Equal(t, 2, list.Size())
	assert.True(t, list.Contains(0))
	assert.True(t, list.Contains(3))
}

func TestDoubleList_RemoveMiddle(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddTail(10)
	list.AddTail(20)
	list.AddTail(30)
	list.AddTail(40)
	list.AddTail(50)

	// Удаляем средний элемент
	list.Remove(30)
	assert.Equal(t, 4, list.Size())
	assert.False(t, list.Contains(30))

	// Остальные элементы на месте
	assert.True(t, list.Contains(10))
	assert.True(t, list.Contains(20))
	assert.True(t, list.Contains(40))
	assert.True(t, list.Contains(50))
}

func TestDoubleList_SingleElement(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddHead(42)

	assert.Equal(t, 1, list.Size())

	list.Remove(42)
	assert.Equal(t, 0, list.Size())
}

func TestDoubleList_BinarySerialization(t *testing.T) {
	list := containers.NewDoubleList()
	list.AddTail(100)
	list.AddTail(200)
	list.AddTail(300)

	var buf bytes.Buffer
	err := list.SaveToBinary(&buf)
	require.NoError(t, err)

	list2 := containers.NewDoubleList()
	err = list2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, list.Size(), list2.Size())
	assert.True(t, list2.Contains(100))
	assert.True(t, list2.Contains(200))
	assert.True(t, list2.Contains(300))
}

func TestDoubleList_EmptySerialization(t *testing.T) {
	list := containers.NewDoubleList()

	var buf bytes.Buffer
	err := list.SaveToBinary(&buf)
	require.NoError(t, err)

	list2 := containers.NewDoubleList()
	err = list2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 0, list2.Size())
}

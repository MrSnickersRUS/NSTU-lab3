package containers_test

import (
	"bytes"
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestSingleList_AddHead(t *testing.T) {
	list := containers.NewSingleList()
	list.AddHead(10)
	list.AddHead(20)
	list.AddHead(30)

	assert.Equal(t, 3, list.Size())
	assert.True(t, list.Contains(10))
	assert.True(t, list.Contains(20))
	assert.True(t, list.Contains(30))
}

func TestSingleList_AddTail(t *testing.T) {
	list := containers.NewSingleList()
	list.AddTail(10)
	list.AddTail(20)
	list.AddTail(30)

	assert.Equal(t, 3, list.Size())
	assert.True(t, list.Contains(10))
	assert.True(t, list.Contains(20))
	assert.True(t, list.Contains(30))
}

func TestSingleList_Remove(t *testing.T) {
	list := containers.NewSingleList()
	list.AddHead(10)
	list.AddHead(20)
	list.AddHead(30)

	result := list.Remove(20)
	assert.True(t, result)
	assert.Equal(t, 2, list.Size())
	assert.False(t, list.Contains(20))
}

func TestSingleList_RemoveNonExistent(t *testing.T) {
	list := containers.NewSingleList()
	list.AddHead(10)

	result := list.Remove(999)
	assert.False(t, result)
	assert.Equal(t, 1, list.Size())
}

func TestSingleList_RemoveHead(t *testing.T) {
	list := containers.NewSingleList()
	list.AddHead(10)
	list.AddHead(20)
	list.AddHead(30)

	list.Remove(30) // удаляем голову
	assert.Equal(t, 2, list.Size())
	assert.False(t, list.Contains(30))
}

func TestSingleList_Contains(t *testing.T) {
	list := containers.NewSingleList()
	list.AddHead(42)

	assert.True(t, list.Contains(42))
	assert.False(t, list.Contains(999))
}

func TestSingleList_Clear(t *testing.T) {
	list := containers.NewSingleList()
	list.AddHead(1)
	list.AddHead(2)
	list.AddHead(3)

	list.Clear()
	assert.Equal(t, 0, list.Size())
	assert.False(t, list.Contains(1))
}

func TestSingleList_Print(t *testing.T) {
	list := containers.NewSingleList()
	result := list.Print()
	assert.Equal(t, "[]", result)

	list.AddHead(10)
	list.AddHead(20)
	result = list.Print()
	assert.Contains(t, result, "20")
	assert.Contains(t, result, "10")
}

func TestSingleList_EmptyOperations(t *testing.T) {
	list := containers.NewSingleList()
	assert.Equal(t, 0, list.Size())
	assert.False(t, list.Contains(1))
	assert.False(t, list.Remove(1))
}

func TestSingleList_MixedOperations(t *testing.T) {
	list := containers.NewSingleList()
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

func TestSingleList_BinarySerialization(t *testing.T) {
	list := containers.NewSingleList()
	list.AddTail(10)
	list.AddTail(20)
	list.AddTail(30)

	var buf bytes.Buffer
	err := list.SaveToBinary(&buf)
	require.NoError(t, err)

	list2 := containers.NewSingleList()
	err = list2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, list.Size(), list2.Size())
	assert.True(t, list2.Contains(10))
	assert.True(t, list2.Contains(20))
	assert.True(t, list2.Contains(30))
}

func TestSingleList_EmptySerialization(t *testing.T) {
	list := containers.NewSingleList()

	var buf bytes.Buffer
	err := list.SaveToBinary(&buf)
	require.NoError(t, err)

	list2 := containers.NewSingleList()
	err = list2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 0, list2.Size())
}

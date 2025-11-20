package containers_test

import (
	"bytes"
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestArray_Push(t *testing.T) {
	arr := containers.NewArray(2)
	arr.Push(10)
	arr.Push(20)
	arr.Push(30) // должен расшириться

	assert.Equal(t, 3, arr.Size())
	val, err := arr.Get(0)
	assert.NoError(t, err)
	assert.Equal(t, 10, val)
}

func TestArray_Insert(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(10)
	arr.Push(30)
	err := arr.Insert(1, 20)

	require.NoError(t, err)
	assert.Equal(t, 3, arr.Size())
	val, _ := arr.Get(1)
	assert.Equal(t, 20, val)
}

func TestArray_Remove(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(10)
	arr.Push(20)
	arr.Push(30)
	err := arr.Remove(1)

	require.NoError(t, err)
	assert.Equal(t, 2, arr.Size())
	val, _ := arr.Get(1)
	assert.Equal(t, 30, val)
}

func TestArray_Replace(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(10)
	arr.Push(20)
	arr.Push(30)

	err := arr.Replace(1, 99)
	assert.NoError(t, err)

	val, _ := arr.Get(1)
	assert.Equal(t, 99, val)
}

func TestArray_ReplaceOutOfBounds(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(10)

	err := arr.Replace(5, 99)
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "индекс")
}

func TestArray_GetOutOfBounds(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(1)

	_, err := arr.Get(-1)
	assert.Error(t, err)

	_, err = arr.Get(100)
	assert.Error(t, err)
}

func TestArray_InsertOutOfBounds(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(1)

	err := arr.Insert(-1, 99)
	assert.Error(t, err)

	err = arr.Insert(100, 99)
	assert.Error(t, err)
}

func TestArray_RemoveOutOfBounds(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(1)

	err := arr.Remove(-1)
	assert.Error(t, err)

	err = arr.Remove(100)
	assert.Error(t, err)
}

func TestArray_InsertAtEnd(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(1)
	arr.Push(2)

	err := arr.Insert(2, 99) // insert at end
	assert.NoError(t, err)
	assert.Equal(t, 3, arr.Size())
	val, _ := arr.Get(2)
	assert.Equal(t, 99, val)
}

func TestArray_MultipleResize(t *testing.T) {
	arr := containers.NewArray(1)
	for i := 0; i < 100; i++ {
		arr.Push(i)
	}
	assert.Equal(t, 100, arr.Size())
}

func TestArray_Print(t *testing.T) {
	arr := containers.NewArray(10)
	assert.Equal(t, "[]", arr.Print())

	arr.Push(1)
	arr.Push(2)
	assert.Equal(t, "[1, 2]", arr.Print())
}

func TestArray_BinarySerialization(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(42)
	arr.Push(100)
	arr.Push(200)

	var buf bytes.Buffer
	err := arr.SaveToBinary(&buf)
	require.NoError(t, err)

	arr2 := containers.NewArray(10)
	err = arr2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, arr.Size(), arr2.Size())
	assert.Equal(t, arr.Print(), arr2.Print())
}

func TestArray_EmptySerialization(t *testing.T) {
	arr := containers.NewArray(10)

	var buf bytes.Buffer
	err := arr.SaveToBinary(&buf)
	require.NoError(t, err)

	arr2 := containers.NewArray(10)
	err = arr2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 0, arr2.Size())
}

func TestArray_SerializationWithResize(t *testing.T) {
	arr := containers.NewArray(2)
	for i := 0; i < 10; i++ {
		arr.Push(i)
	}

	var buf bytes.Buffer
	err := arr.SaveToBinary(&buf)
	require.NoError(t, err)

	arr2 := containers.NewArray(1)
	err = arr2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 10, arr2.Size())
	for i := 0; i < 10; i++ {
		val, _ := arr2.Get(i)
		assert.Equal(t, i, val)
	}
}

func TestArray_JSONSerialization(t *testing.T) {
	arr := containers.NewArray(10)
	arr.Push(1)
	arr.Push(2)
	arr.Push(3)

	jsonStr, err := arr.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "[1,2,3]", jsonStr)

	arr2 := containers.NewArray(10)
	err = arr2.FromJSON(jsonStr)
	require.NoError(t, err)

	assert.Equal(t, 3, arr2.Size())
	val, _ := arr2.Get(0)
	assert.Equal(t, 1, val)
	val, _ = arr2.Get(1)
	assert.Equal(t, 2, val)
	val, _ = arr2.Get(2)
	assert.Equal(t, 3, val)
}

func TestArray_JSONEmpty(t *testing.T) {
	arr := containers.NewArray(10)
	jsonStr, err := arr.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "[]", jsonStr)

	arr2 := containers.NewArray(10)
	err = arr2.FromJSON(jsonStr)
	require.NoError(t, err)
	assert.Equal(t, 0, arr2.Size())
}

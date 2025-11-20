package containers_test

import (
	"bytes"
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestStack_PushPop(t *testing.T) {
	stack := containers.NewStack(10)
	stack.Push(10)
	stack.Push(20)
	stack.Push(30)

	val, err := stack.Pop()
	assert.NoError(t, err)
	assert.Equal(t, 30, val)

	val, err = stack.Pop()
	assert.NoError(t, err)
	assert.Equal(t, 20, val)

	assert.Equal(t, 1, stack.Size())
}

func TestStack_Peek(t *testing.T) {
	stack := containers.NewStack(10)
	stack.Push(42)

	val, err := stack.Peek()
	assert.NoError(t, err)
	assert.Equal(t, 42, val)
	assert.Equal(t, 1, stack.Size()) // размер не изменился
}

func TestStack_PopEmpty(t *testing.T) {
	stack := containers.NewStack(10)
	_, err := stack.Pop()
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "стек пуст")
}

func TestStack_PeekEmpty(t *testing.T) {
	stack := containers.NewStack(10)
	_, err := stack.Peek()
	assert.Error(t, err)
}

func TestStack_Resize(t *testing.T) {
	stack := containers.NewStack(2)
	stack.Push(1)
	stack.Push(2)
	stack.Push(3) // должен вызвать resize

	assert.Equal(t, 3, stack.Size())
	val, _ := stack.Pop()
	assert.Equal(t, 3, val)
}

func TestStack_MultipleResize(t *testing.T) {
	stack := containers.NewStack(1)
	for i := 0; i < 50; i++ {
		stack.Push(i)
	}
	assert.Equal(t, 50, stack.Size())
}

func TestStack_ClearAndReuse(t *testing.T) {
	stack := containers.NewStack(10)
	stack.Push(1)
	stack.Push(2)
	stack.Clear()
	assert.Equal(t, 0, stack.Size())

	stack.Push(3)
	val, _ := stack.Pop()
	assert.Equal(t, 3, val)
}

func TestStack_Print(t *testing.T) {
	stack := containers.NewStack(10)
	assert.Equal(t, "[]", stack.Print())

	stack.Push(10)
	stack.Push(20)
	assert.Equal(t, "[10, 20]", stack.Print())
}

func TestStack_BinarySerialization(t *testing.T) {
	stack := containers.NewStack(10)
	stack.Push(1)
	stack.Push(2)
	stack.Push(3)

	var buf bytes.Buffer
	err := stack.SaveToBinary(&buf)
	require.NoError(t, err)

	stack2 := containers.NewStack(10)
	err = stack2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, stack.Size(), stack2.Size())
	assert.Equal(t, stack.Print(), stack2.Print())
}

func TestStack_EmptySerialization(t *testing.T) {
	stack := containers.NewStack(10)

	var buf bytes.Buffer
	err := stack.SaveToBinary(&buf)
	require.NoError(t, err)

	stack2 := containers.NewStack(10)
	err = stack2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 0, stack2.Size())
}

func TestStack_JSONSerialization(t *testing.T) {
	stack := containers.NewStack(10)
	stack.Push(1)
	stack.Push(2)
	stack.Push(3)

	jsonStr, err := stack.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "[1,2,3]", jsonStr)

	stack2 := containers.NewStack(10)
	err = stack2.FromJSON(jsonStr)
	require.NoError(t, err)

	assert.Equal(t, 3, stack2.Size())
	val, _ := stack2.Pop()
	assert.Equal(t, 3, val)
}

func TestStack_JSONEmpty(t *testing.T) {
	stack := containers.NewStack(10)
	jsonStr, err := stack.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "[]", jsonStr)
}

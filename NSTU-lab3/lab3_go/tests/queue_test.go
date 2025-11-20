package containers_test

import (
	"bytes"
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestQueue_EnqueueDequeue(t *testing.T) {
	queue := containers.NewQueue(10)
	queue.Enqueue(10)
	queue.Enqueue(20)
	queue.Enqueue(30)

	val, err := queue.Dequeue()
	assert.NoError(t, err)
	assert.Equal(t, 10, val)

	val, err = queue.Dequeue()
	assert.NoError(t, err)
	assert.Equal(t, 20, val)

	assert.Equal(t, 1, queue.Size())
}

func TestQueue_CircularBehavior(t *testing.T) {
	queue := containers.NewQueue(3)
	queue.Enqueue(1)
	queue.Enqueue(2)
	queue.Enqueue(3)

	queue.Dequeue()
	queue.Dequeue()

	queue.Enqueue(4)
	queue.Enqueue(5)

	assert.Equal(t, 3, queue.Size())
	val, _ := queue.Dequeue()
	assert.Equal(t, 3, val)
}

func TestQueue_Peek(t *testing.T) {
	queue := containers.NewQueue(10)
	queue.Enqueue(42)
	queue.Enqueue(100)

	val, err := queue.Peek()
	assert.NoError(t, err)
	assert.Equal(t, 42, val)
	assert.Equal(t, 2, queue.Size()) // размер не изменился
}

func TestQueue_PeekEmpty(t *testing.T) {
	queue := containers.NewQueue(10)
	_, err := queue.Peek()
	assert.Error(t, err)
	assert.Contains(t, err.Error(), "пуста")
}

func TestQueue_DequeueEmpty(t *testing.T) {
	queue := containers.NewQueue(10)
	_, err := queue.Dequeue()
	assert.Error(t, err)
}

func TestQueue_Resize(t *testing.T) {
	queue := containers.NewQueue(2)
	queue.Enqueue(1)
	queue.Enqueue(2)
	queue.Enqueue(3) // должен вызвать resize

	assert.Equal(t, 3, queue.Size())
	val, _ := queue.Dequeue()
	assert.Equal(t, 1, val)
}

func TestQueue_MultipleResize(t *testing.T) {
	queue := containers.NewQueue(2)
	for i := 0; i < 50; i++ {
		queue.Enqueue(i)
	}
	assert.Equal(t, 50, queue.Size())
}

func TestQueue_CircularWrapAround(t *testing.T) {
	queue := containers.NewQueue(5)
	for i := 0; i < 3; i++ {
		queue.Enqueue(i)
	}
	queue.Dequeue()
	queue.Dequeue()

	for i := 10; i < 15; i++ {
		queue.Enqueue(i)
	}

	val, _ := queue.Dequeue()
	assert.Equal(t, 2, val)

	val, _ = queue.Dequeue()
	assert.Equal(t, 10, val)
}

func TestQueue_Print(t *testing.T) {
	queue := containers.NewQueue(10)
	assert.Equal(t, "[]", queue.Print())

	queue.Enqueue(5)
	queue.Enqueue(10)
	assert.Equal(t, "[5, 10]", queue.Print())
}

func TestQueue_BinarySerialization(t *testing.T) {
	queue := containers.NewQueue(10)
	queue.Enqueue(100)
	queue.Enqueue(200)
	queue.Enqueue(300)

	var buf bytes.Buffer
	err := queue.SaveToBinary(&buf)
	require.NoError(t, err)

	queue2 := containers.NewQueue(10)
	err = queue2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, queue.Size(), queue2.Size())
	assert.Equal(t, queue.Print(), queue2.Print())
}

func TestQueue_EmptySerialization(t *testing.T) {
	queue := containers.NewQueue(10)

	var buf bytes.Buffer
	err := queue.SaveToBinary(&buf)
	require.NoError(t, err)

	queue2 := containers.NewQueue(10)
	err = queue2.LoadFromBinary(&buf)
	require.NoError(t, err)

	assert.Equal(t, 0, queue2.Size())
}

func TestQueue_JSONSerialization(t *testing.T) {
	queue := containers.NewQueue(10)
	queue.Enqueue(1)
	queue.Enqueue(2)
	queue.Enqueue(3)

	jsonStr, err := queue.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "[1,2,3]", jsonStr)

	queue2 := containers.NewQueue(10)
	err = queue2.FromJSON(jsonStr)
	require.NoError(t, err)

	assert.Equal(t, 3, queue2.Size())
	val, _ := queue2.Dequeue()
	assert.Equal(t, 1, val)
}

func TestQueue_JSONEmpty(t *testing.T) {
	queue := containers.NewQueue(10)
	jsonStr, err := queue.ToJSON()
	require.NoError(t, err)
	assert.Equal(t, "[]", jsonStr)
}

package containers_test

import (
	"testing"

	"lab3_go/containers"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestSet_AddContains(t *testing.T) {
	set := containers.NewSet(101)
	err := set.Add("элемент1")
	require.NoError(t, err)

	assert.True(t, set.Contains("элемент1"))
	assert.False(t, set.Contains("элемент2"))
}

func TestSet_Remove(t *testing.T) {
	set := containers.NewSet(101)
	set.Add("test")

	removed := set.Remove("test")
	assert.True(t, removed)
	assert.Equal(t, 0, set.Size())
}

func TestSet_AddDuplicate(t *testing.T) {
	set := containers.NewSet(101)
	set.Add("item")
	set.Add("item")
	set.Add("item")

	assert.Equal(t, 1, set.Size())
}

func TestSet_RemoveNonExistent(t *testing.T) {
	set := containers.NewSet(101)
	removed := set.Remove("nonexistent")
	assert.False(t, removed)
}

func TestSet_Clear(t *testing.T) {
	set := containers.NewSet(101)
	set.Add("item1")
	set.Add("item2")

	set.Clear()
	assert.Equal(t, 0, set.Size())
	assert.False(t, set.Contains("item1"))
}

func TestSet_Print(t *testing.T) {
	set := containers.NewSet(101)
	result := set.Print()
	assert.Contains(t, result, "HashMap")
}

package containers

// Set - множество на основе HashMap
type Set struct {
	hashMap *HashMap
}

// NewSet создаёт новое множество
func NewSet(initialCapacity int) *Set {
	return &Set{
		hashMap: NewHashMap(initialCapacity),
	}
}

// Add добавляет элемент в множество
func (s *Set) Add(value string) error {
	return s.hashMap.Put(value, "")
}

// Contains проверяет наличие элемента
func (s *Set) Contains(value string) bool {
	return s.hashMap.Contains(value)
}

// Remove удаляет элемент из множества
func (s *Set) Remove(value string) bool {
	return s.hashMap.Remove(value)
}

// Clear очищает множество
func (s *Set) Clear() {
	s.hashMap.Clear()
}

// Size возвращает количество элементов
func (s *Set) Size() int {
	return s.hashMap.Size()
}

// Print выводит множество
func (s *Set) Print() string {
	return s.hashMap.Print()
}

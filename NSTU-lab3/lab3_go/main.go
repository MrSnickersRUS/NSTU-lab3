package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"lab3_go/containers"
	"os"
	"strconv"
	"strings"
)

// ContainerManager управляет всеми контейнерами
type ContainerManager struct {
	arrays   map[string]*containers.Array
	stacks   map[string]*containers.Stack
	queues   map[string]*containers.Queue
	hashmaps map[string]*containers.HashMap
	sets     map[string]*containers.Set
	trees    map[string]*containers.AVLTree
}

func NewContainerManager() *ContainerManager {
	return &ContainerManager{
		arrays:   make(map[string]*containers.Array),
		stacks:   make(map[string]*containers.Stack),
		queues:   make(map[string]*containers.Queue),
		hashmaps: make(map[string]*containers.HashMap),
		sets:     make(map[string]*containers.Set),
		trees:    make(map[string]*containers.AVLTree),
	}
}

// Структура для сохранения всего состояния
type AllData struct {
	Arrays   map[string][]int             `json:"arrays"`
	Stacks   map[string][]int             `json:"stacks"`
	Queues   map[string][]int             `json:"queues"`
	HashMaps map[string]map[string]string `json:"hashmaps"`
	Trees    map[string][]string          `json:"trees"`
}

// saveAllToJSON сохраняет все контейнеры в один JSON файл
func (cm *ContainerManager) saveAllToJSON(filename string) error {
	data := AllData{
		Arrays:   make(map[string][]int),
		Stacks:   make(map[string][]int),
		Queues:   make(map[string][]int),
		HashMaps: make(map[string]map[string]string),
		Trees:    make(map[string][]string),
	}

	// Сериализуем все массивы
	for name, arr := range cm.arrays {
		jsonStr, err := arr.ToJSON()
		if err != nil {
			return err
		}
		var values []int
		if err := json.Unmarshal([]byte(jsonStr), &values); err != nil {
			return err
		}
		data.Arrays[name] = values
	}

	// Сериализуем все стеки
	for name, stack := range cm.stacks {
		jsonStr, err := stack.ToJSON()
		if err != nil {
			return err
		}
		var values []int
		if err := json.Unmarshal([]byte(jsonStr), &values); err != nil {
			return err
		}
		data.Stacks[name] = values
	}

	// Сериализуем все очереди
	for name, queue := range cm.queues {
		jsonStr, err := queue.ToJSON()
		if err != nil {
			return err
		}
		var values []int
		if err := json.Unmarshal([]byte(jsonStr), &values); err != nil {
			return err
		}
		data.Queues[name] = values
	}

	// Сериализуем все хеш-таблицы
	for name, hm := range cm.hashmaps {
		jsonStr, err := hm.ToJSON()
		if err != nil {
			return err
		}
		var values map[string]string
		if err := json.Unmarshal([]byte(jsonStr), &values); err != nil {
			return err
		}
		data.HashMaps[name] = values
	}

	// Сериализуем все деревья
	for name, tree := range cm.trees {
		jsonStr, err := tree.ToJSON()
		if err != nil {
			return err
		}
		var values []string
		if err := json.Unmarshal([]byte(jsonStr), &values); err != nil {
			return err
		}
		data.Trees[name] = values
	}

	// Записываем в файл с форматированием
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	encoder := json.NewEncoder(file)
	encoder.SetIndent("", "  ")
	return encoder.Encode(data)
}

// loadAllFromJSON загружает все контейнеры из JSON файла
func (cm *ContainerManager) loadAllFromJSON(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var data AllData
	decoder := json.NewDecoder(file)
	if err := decoder.Decode(&data); err != nil {
		return err
	}

	// Загружаем массивы
	for name, values := range data.Arrays {
		arr := containers.NewArray(10)
		jsonBytes, _ := json.Marshal(values)
		if err := arr.FromJSON(string(jsonBytes)); err != nil {
			return err
		}
		cm.arrays[name] = arr
	}

	// Загружаем стеки
	for name, values := range data.Stacks {
		stack := containers.NewStack(10)
		jsonBytes, _ := json.Marshal(values)
		if err := stack.FromJSON(string(jsonBytes)); err != nil {
			return err
		}
		cm.stacks[name] = stack
	}

	// Загружаем очереди
	for name, values := range data.Queues {
		queue := containers.NewQueue(10)
		jsonBytes, _ := json.Marshal(values)
		if err := queue.FromJSON(string(jsonBytes)); err != nil {
			return err
		}
		cm.queues[name] = queue
	}

	// Загружаем хеш-таблицы
	for name, values := range data.HashMaps {
		hm := containers.NewHashMap(101)
		jsonBytes, _ := json.Marshal(values)
		if err := hm.FromJSON(string(jsonBytes)); err != nil {
			return err
		}
		cm.hashmaps[name] = hm
	}

	// Загружаем деревья
	for name, values := range data.Trees {
		tree := containers.NewAVLTree()
		jsonBytes, _ := json.Marshal(values)
		if err := tree.FromJSON(string(jsonBytes)); err != nil {
			return err
		}
		cm.trees[name] = tree
	}

	return nil
}

func (cm *ContainerManager) processCommand(cmd string) {
	parts := strings.Fields(cmd)
	if len(parts) == 0 {
		return
	}

	command := strings.ToUpper(parts[0])

	switch command {
	case "HELP":
		cm.printHelp()
	case "EXIT":
		fmt.Println("До свидания!")
		os.Exit(0)

	// Array команды
	case "MPUSH":
		if len(parts) < 3 {
			fmt.Println("❌ Ошибка: MPUSH требует <имя> <значение>")
			return
		}
		name := parts[1]
		val, err := strconv.Atoi(parts[2])
		if err != nil {
			fmt.Println("❌ Ошибка: неверное значение")
			return
		}
		if cm.arrays[name] == nil {
			cm.arrays[name] = containers.NewArray(10)
		}
		cm.arrays[name].Push(val)
		fmt.Printf("✓ Добавлено: %d\n", val)

	case "MGET":
		if len(parts) < 3 {
			fmt.Println("❌ Ошибка: MGET требует <имя> <индекс>")
			return
		}
		name := parts[1]
		idx, _ := strconv.Atoi(parts[2])
		if cm.arrays[name] == nil {
			fmt.Println("❌ Ошибка: массив не найден")
			return
		}
		val, err := cm.arrays[name].Get(idx)
		if err != nil {
			fmt.Println("❌ Ошибка:", err)
			return
		}
		fmt.Println(val)

	case "MPRINT":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: MPRINT требует <имя>")
			return
		}
		if cm.arrays[parts[1]] != nil {
			fmt.Println(cm.arrays[parts[1]].Print())
		}

	// Stack команды
	case "SPUSH":
		if len(parts) < 3 {
			fmt.Println("❌ Ошибка: SPUSH требует <имя> <значение>")
			return
		}
		name := parts[1]
		val, _ := strconv.Atoi(parts[2])
		if cm.stacks[name] == nil {
			cm.stacks[name] = containers.NewStack(10)
		}
		cm.stacks[name].Push(val)
		fmt.Printf("✓ Добавлено: %d\n", val)

	case "SPOP":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: SPOP требует <имя>")
			return
		}
		if cm.stacks[parts[1]] == nil {
			fmt.Println("❌ Ошибка: стек не найден")
			return
		}
		val, err := cm.stacks[parts[1]].Pop()
		if err != nil {
			fmt.Println("❌ Ошибка:", err)
			return
		}
		fmt.Println(val)

	case "SPRINT":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: SPRINT требует <имя>")
			return
		}
		if cm.stacks[parts[1]] != nil {
			fmt.Println(cm.stacks[parts[1]].Print())
		}

	// Queue команды
	case "QPUSH":
		if len(parts) < 3 {
			fmt.Println("❌ Ошибка: QPUSH требует <имя> <значение>")
			return
		}
		name := parts[1]
		val, _ := strconv.Atoi(parts[2])
		if cm.queues[name] == nil {
			cm.queues[name] = containers.NewQueue(10)
		}
		cm.queues[name].Enqueue(val)
		fmt.Printf("✓ Добавлено: %d\n", val)

	case "QPOP":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: QPOP требует <имя>")
			return
		}
		if cm.queues[parts[1]] == nil {
			fmt.Println("❌ Ошибка: очередь не найдена")
			return
		}
		val, err := cm.queues[parts[1]].Dequeue()
		if err != nil {
			fmt.Println("❌ Ошибка:", err)
			return
		}
		fmt.Println(val)

	case "QPRINT":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: QPRINT требует <имя>")
			return
		}
		if cm.queues[parts[1]] != nil {
			fmt.Println(cm.queues[parts[1]].Print())
		}

	// HashMap команды
	case "HPUT":
		if len(parts) < 4 {
			fmt.Println("❌ Ошибка: HPUT требует <имя> <ключ> <значение>")
			return
		}
		name := parts[1]
		key := parts[2]
		value := parts[3]
		if cm.hashmaps[name] == nil {
			cm.hashmaps[name] = containers.NewHashMap(101)
		}
		err := cm.hashmaps[name].Put(key, value)
		if err != nil {
			fmt.Println("❌ Ошибка:", err)
			return
		}
		fmt.Printf("✓ Добавлено: %s => %s\n", key, value)

	case "HGET":
		if len(parts) < 3 {
			fmt.Println("❌ Ошибка: HGET требует <имя> <ключ>")
			return
		}
		if cm.hashmaps[parts[1]] == nil {
			fmt.Println("❌ Ошибка: хеш-таблица не найдена")
			return
		}
		val, err := cm.hashmaps[parts[1]].Get(parts[2])
		if err != nil {
			fmt.Println("❌ Ошибка:", err)
			return
		}
		fmt.Println(val)

	case "HPRINT":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: HPRINT требует <имя>")
			return
		}
		if cm.hashmaps[parts[1]] != nil {
			fmt.Println(cm.hashmaps[parts[1]].Print())
		}

	// JSON команды для сохранения
	case "SAVE_JSON":
		if len(parts) < 3 {
			fmt.Println("❌ Ошибка: SAVE_JSON требует <тип> <имя>")
			fmt.Println("   Типы: ARRAY, STACK, QUEUE, HASHMAP, TREE")
			return
		}
		containerType := strings.ToUpper(parts[1])
		name := parts[2]
		var jsonStr string
		var err error

		switch containerType {
		case "ARRAY":
			if cm.arrays[name] == nil {
				fmt.Println("❌ Ошибка: массив не найден")
				return
			}
			jsonStr, err = cm.arrays[name].ToJSON()
		case "STACK":
			if cm.stacks[name] == nil {
				fmt.Println("❌ Ошибка: стек не найден")
				return
			}
			jsonStr, err = cm.stacks[name].ToJSON()
		case "QUEUE":
			if cm.queues[name] == nil {
				fmt.Println("❌ Ошибка: очередь не найдена")
				return
			}
			jsonStr, err = cm.queues[name].ToJSON()
		case "HASHMAP":
			if cm.hashmaps[name] == nil {
				fmt.Println("❌ Ошибка: хеш-таблица не найдена")
				return
			}
			jsonStr, err = cm.hashmaps[name].ToJSON()
		case "TREE":
			if cm.trees[name] == nil {
				fmt.Println("❌ Ошибка: дерево не найдено")
				return
			}
			jsonStr, err = cm.trees[name].ToJSON()
		default:
			fmt.Println("❌ Ошибка: неизвестный тип контейнера")
			return
		}

		if err != nil {
			fmt.Println("❌ Ошибка сериализации:", err)
			return
		}
		fmt.Println(jsonStr)

	// Сохранение всего состояния в один JSON файл
	case "SAVE_ALL":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: SAVE_ALL требует <файл>")
			return
		}
		filename := parts[1]
		err := cm.saveAllToJSON(filename)
		if err != nil {
			fmt.Println("❌ Ошибка сохранения:", err)
			return
		}
		fmt.Printf("✓ Все данные сохранены в: %s\n", filename)

	// Загрузка всего состояния из JSON файла
	case "LOAD_ALL":
		if len(parts) < 2 {
			fmt.Println("❌ Ошибка: LOAD_ALL требует <файл>")
			return
		}
		filename := parts[1]
		err := cm.loadAllFromJSON(filename)
		if err != nil {
			fmt.Println("❌ Ошибка загрузки:", err)
			return
		}
		fmt.Printf("✓ Все данные загружены из: %s\n", filename)

	// JSON команды для сохранения в файл
	case "SAVE_JSON_FILE":
		if len(parts) < 4 {
			fmt.Println("❌ Ошибка: SAVE_JSON_FILE требует <тип> <имя> <файл>")
			fmt.Println("   Типы: ARRAY, STACK, QUEUE, HASHMAP, TREE")
			return
		}
		containerType := strings.ToUpper(parts[1])
		name := parts[2]
		filename := parts[3]
		var jsonStr string
		var err error

		switch containerType {
		case "ARRAY":
			if cm.arrays[name] == nil {
				fmt.Println("❌ Ошибка: массив не найден")
				return
			}
			jsonStr, err = cm.arrays[name].ToJSON()
		case "STACK":
			if cm.stacks[name] == nil {
				fmt.Println("❌ Ошибка: стек не найден")
				return
			}
			jsonStr, err = cm.stacks[name].ToJSON()
		case "QUEUE":
			if cm.queues[name] == nil {
				fmt.Println("❌ Ошибка: очередь не найдена")
				return
			}
			jsonStr, err = cm.queues[name].ToJSON()
		case "HASHMAP":
			if cm.hashmaps[name] == nil {
				fmt.Println("❌ Ошибка: хеш-таблица не найдена")
				return
			}
			jsonStr, err = cm.hashmaps[name].ToJSON()
		case "TREE":
			if cm.trees[name] == nil {
				fmt.Println("❌ Ошибка: дерево не найдено")
				return
			}
			jsonStr, err = cm.trees[name].ToJSON()
		default:
			fmt.Println("❌ Ошибка: неизвестный тип контейнера")
			return
		}

		if err != nil {
			fmt.Println("❌ Ошибка сериализации:", err)
			return
		}

		err = os.WriteFile(filename, []byte(jsonStr), 0644)
		if err != nil {
			fmt.Println("❌ Ошибка записи в файл:", err)
			return
		}
		fmt.Printf("✓ JSON сохранён в файл: %s\n", filename)

	// JSON команды для загрузки из файла
	case "LOAD_JSON_FILE":
		if len(parts) < 4 {
			fmt.Println("❌ Ошибка: LOAD_JSON_FILE требует <тип> <имя> <файл>")
			fmt.Println("   Типы: ARRAY, STACK, QUEUE, HASHMAP, TREE")
			return
		}
		containerType := strings.ToUpper(parts[1])
		name := parts[2]
		filename := parts[3]

		jsonBytes, err := os.ReadFile(filename)
		if err != nil {
			fmt.Println("❌ Ошибка чтения файла:", err)
			return
		}
		jsonStr := string(jsonBytes)

		switch containerType {
		case "ARRAY":
			if cm.arrays[name] == nil {
				cm.arrays[name] = containers.NewArray(10)
			}
			err = cm.arrays[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Массив загружен из файла: %s\n", filename)
			}
		case "STACK":
			if cm.stacks[name] == nil {
				cm.stacks[name] = containers.NewStack(10)
			}
			err = cm.stacks[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Стек загружен из файла: %s\n", filename)
			}
		case "QUEUE":
			if cm.queues[name] == nil {
				cm.queues[name] = containers.NewQueue(10)
			}
			err = cm.queues[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Очередь загружена из файла: %s\n", filename)
			}
		case "HASHMAP":
			if cm.hashmaps[name] == nil {
				cm.hashmaps[name] = containers.NewHashMap(101)
			}
			err = cm.hashmaps[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Хеш-таблица загружена из файла: %s\n", filename)
			}
		case "TREE":
			if cm.trees[name] == nil {
				cm.trees[name] = containers.NewAVLTree()
			}
			err = cm.trees[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Дерево загружено из файла: %s\n", filename)
			}
		default:
			fmt.Println("❌ Ошибка: неизвестный тип контейнера")
			return
		}

		if err != nil {
			fmt.Println("❌ Ошибка десериализации:", err)
			return
		}

	// JSON команды для загрузки
	case "LOAD_JSON":
		if len(parts) < 4 {
			fmt.Println("❌ Ошибка: LOAD_JSON требует <тип> <имя> <json>")
			fmt.Println("   Типы: ARRAY, STACK, QUEUE, HASHMAP, TREE")
			return
		}
		containerType := strings.ToUpper(parts[1])
		name := parts[2]
		jsonStr := strings.Join(parts[3:], " ")

		var err error
		switch containerType {
		case "ARRAY":
			if cm.arrays[name] == nil {
				cm.arrays[name] = containers.NewArray(10)
			}
			err = cm.arrays[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Массив загружен из JSON\n")
			}
		case "STACK":
			if cm.stacks[name] == nil {
				cm.stacks[name] = containers.NewStack(10)
			}
			err = cm.stacks[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Стек загружен из JSON\n")
			}
		case "QUEUE":
			if cm.queues[name] == nil {
				cm.queues[name] = containers.NewQueue(10)
			}
			err = cm.queues[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Очередь загружена из JSON\n")
			}
		case "HASHMAP":
			if cm.hashmaps[name] == nil {
				cm.hashmaps[name] = containers.NewHashMap(101)
			}
			err = cm.hashmaps[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Хеш-таблица загружена из JSON\n")
			}
		case "TREE":
			if cm.trees[name] == nil {
				cm.trees[name] = containers.NewAVLTree()
			}
			err = cm.trees[name].FromJSON(jsonStr)
			if err == nil {
				fmt.Printf("✓ Дерево загружено из JSON\n")
			}
		default:
			fmt.Println("❌ Ошибка: неизвестный тип контейнера")
			return
		}

		if err != nil {
			fmt.Println("❌ Ошибка десериализации:", err)
			return
		}

	default:
		fmt.Println("❌ Неизвестная команда. Введите HELP для справки")
	}
}

func (cm *ContainerManager) printHelp() {
	fmt.Println(`
📖 ДОСТУПНЫЕ КОМАНДЫ:

МАССИВ (Array):
  MPUSH <имя> <значение>     - добавить элемент
  MGET <имя> <индекс>        - получить элемент
  MPRINT <имя>               - вывести массив

СТЕК (Stack):
  SPUSH <имя> <значение>     - добавить в стек
  SPOP <имя>                 - извлечь из стека
  SPRINT <имя>               - вывести стек

ОЧЕРЕДЬ (Queue):
  QPUSH <имя> <значение>     - добавить в очередь
  QPOP <имя>                 - извлечь из очереди
  QPRINT <имя>               - вывести очередь

ХЕШ-ТАБЛИЦА (HashMap):
  HPUT <имя> <ключ> <значение> - добавить пару
  HGET <имя> <ключ>            - получить значение
  HPRINT <имя>                 - вывести таблицу

JSON СЕРИАЛИЗАЦИЯ:
  SAVE_ALL <файл>                  - сохранить ВСЕ контейнеры в JSON
  LOAD_ALL <файл>                  - загрузить ВСЕ контейнеры из JSON
  SAVE_JSON <тип> <имя>            - вывести JSON в консоль
  LOAD_JSON <тип> <имя> <json>     - загрузить из JSON строки
  SAVE_JSON_FILE <тип> <имя> <файл> - сохранить JSON в файл
  LOAD_JSON_FILE <тип> <имя> <файл> - загрузить JSON из файла
    Типы: ARRAY, STACK, QUEUE, HASHMAP, TREE

ОБЩИЕ:
  HELP                       - эта справка
  EXIT                       - выход`)
}

func main() {
	fmt.Println()
	fmt.Println("Введите 'HELP' для справки или 'EXIT' для выхода")
	fmt.Println()

	manager := NewContainerManager()
	scanner := bufio.NewScanner(os.Stdin)

	for {
		fmt.Print("> ")
		if !scanner.Scan() {
			break
		}

		cmd := strings.TrimSpace(scanner.Text())
		if cmd != "" {
			manager.processCommand(cmd)
		}
	}
}

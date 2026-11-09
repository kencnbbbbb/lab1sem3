package main

import (
	"bufio"
	"fmt"
	"lab1variant3/str"
	"os"
	"sort"
	"strconv"
	"strings"
)

type object struct {
	kind   string
	array  str.Array
	single str.OneLinkedList
	double str.LinkedList
	stack  str.Stack
	queue  str.Queue
	tree   str.Tree
}

var objects = map[string]*object{}

func (o *object) read() string {
	switch o.kind {
	case "Array":
		return o.array.Read()
	case "OneLinkedList":
		return o.single.ReadHT()
	case "LinkedList":
		return o.double.ReadHT()
	case "Stack":
		return o.stack.Read()
	case "Queue":
		return o.queue.Read()
	case "Tree":
		return o.tree.Read()
	}
	return ""
}
func newObject(kind string) *object { return &object{kind: kind, array: str.NewArray(16)} }
func number(s string) (int, error) {
	n, err := strconv.ParseInt(s, 10, 32)
	if err != nil {
		return 0, fmt.Errorf("ожидается целое 32-битное число: %s", s)
	}
	return int(n), nil
}
func load(file string) error {
	f, err := os.Open(file)
	if os.IsNotExist(err) {
		return nil
	}
	if err != nil {
		return err
	}
	defer f.Close()
	scanner := bufio.NewScanner(f)
	scanner.Buffer(make([]byte, 4096), 16*1024*1024)
	for scanner.Scan() {
		fields := strings.Fields(scanner.Text())
		if len(fields) == 0 {
			continue
		}
		if len(fields) < 2 {
			return fmt.Errorf("неверная строка файла")
		}
		kind, name := fields[0], fields[1]
		if objects[name] != nil {
			return fmt.Errorf("повторное имя в файле: %s", name)
		}
		o := newObject(kind)
		switch kind {
		case "Array", "OneLinkedList", "LinkedList", "Stack", "Queue", "Tree":
		default:
			return fmt.Errorf("неизвестная структура в файле: %s", kind)
		}
		for _, value := range fields[2:] {
			switch kind {
			case "Array":
				o.array.PushBack(value)
			case "OneLinkedList":
				o.single.PushBack(value)
			case "LinkedList":
				o.double.PushBack(value)
			case "Stack":
				o.stack.PushBack(value)
			case "Queue":
				o.queue.PushBack(value)
			case "Tree":
				n, err := number(value)
				if err != nil {
					return err
				}
				o.tree.Insert(n)
			}
		}
		objects[name] = o
	}
	return scanner.Err()
}
func save(file string) error {
	var out strings.Builder
	names := make([]string, 0, len(objects))
	for name := range objects {
		names = append(names, name)
	}
	sort.Strings(names)
	for _, name := range names {
		o := objects[name]
		values := o.read()
		if o.kind == "Stack" {
			values = o.stack.List.ReadHT()
		} // bottom to top
		fmt.Fprintf(&out, "%s %s%s\n", o.kind, name, values)
	}
	temp := file + ".tmp"
	if err := os.WriteFile(temp, []byte(out.String()), 0600); err != nil {
		os.Remove(temp)
		return err
	}
	if err := os.Rename(temp, file); err != nil {
		os.Remove(temp)
		return err
	}
	return nil
}

type spec struct {
	min, max       int
	change, create bool
	kind           string
}

var specs = map[string]spec{
	"PRINT":          {0, 0, false, false, ""},
	"MCREATE":        {0, 0, true, true, "Array"},
	"FCREATE":        {0, 0, true, true, "OneLinkedList"},
	"LCREATE":        {0, 0, true, true, "LinkedList"},
	"SCREATE":        {0, 0, true, true, "Stack"},
	"QCREATE":        {0, 0, true, true, "Queue"},
	"TCREATE":        {0, 0, true, true, "Tree"},
	"MPUSH":          {1, -1, true, true, "Array"},
	"FPUSH":          {1, -1, true, true, "OneLinkedList"},
	"FPUSHHEAD":      {1, -1, true, true, "OneLinkedList"},
	"LPUSH":          {1, -1, true, true, "LinkedList"},
	"LPUSHHEAD":      {1, -1, true, true, "LinkedList"},
	"SPUSH":          {1, -1, true, true, "Stack"},
	"QPUSH":          {1, -1, true, true, "Queue"},
	"TINSERT":        {1, -1, true, true, "Tree"},
	"MPUSHIDX":       {2, 2, true, false, "Array"},
	"MSETIDX":        {2, 2, true, false, "Array"},
	"FPUSHBEFORE":    {2, 2, true, false, "OneLinkedList"},
	"FPUSHAFTER":     {2, 2, true, false, "OneLinkedList"},
	"LPUSHBEFORE":    {2, 2, true, false, "LinkedList"},
	"LPUSHAFTER":     {2, 2, true, false, "LinkedList"},
	"MDELIDX":        {1, 1, true, false, "Array"},
	"FPOPBEFORE":     {1, 1, true, false, "OneLinkedList"},
	"FPOPAFTER":      {1, 1, true, false, "OneLinkedList"},
	"FDELVAL":        {1, 1, true, false, "OneLinkedList"},
	"LPOPBEFORE":     {1, 1, true, false, "LinkedList"},
	"LPOPAFTER":      {1, 1, true, false, "LinkedList"},
	"LDELVAL":        {1, 1, true, false, "LinkedList"},
	"FPOP":           {0, 0, true, false, "OneLinkedList"},
	"FPOPHEAD":       {0, 0, true, false, "OneLinkedList"},
	"LPOP":           {0, 0, true, false, "LinkedList"},
	"LPOPHEAD":       {0, 0, true, false, "LinkedList"},
	"SPOP":           {0, 0, true, false, "Stack"},
	"QPOP":           {0, 0, true, false, "Queue"},
	"MGET":           {1, 1, false, false, "Array"},
	"FFINDVAL":       {1, 1, false, false, "OneLinkedList"},
	"LFINDVAL":       {1, 1, false, false, "LinkedList"},
	"TFIND":          {1, 1, false, false, "Tree"},
	"MLENGTH":        {0, 0, false, false, "Array"},
	"MREAD":          {0, 0, false, false, "Array"},
	"FREADHT":        {0, 0, false, false, "OneLinkedList"},
	"FREADTH":        {0, 0, false, false, "OneLinkedList"},
	"LREADHT":        {0, 0, false, false, "LinkedList"},
	"LREADTH":        {0, 0, false, false, "LinkedList"},
	"SREAD":          {0, 0, false, false, "Stack"},
	"QREAD":          {0, 0, false, false, "Queue"},
	"TREAD":          {0, 0, false, false, "Tree"},
	"TCHECKCOMPLETE": {0, 0, false, false, "Tree"},
}

func execute(query string) (bool, error) {
	fields := strings.Fields(query)
	if len(fields) < 2 {
		return false, fmt.Errorf("ожидается КОМАНДА имя [аргументы]")
	}
	cmd, name, args := fields[0], fields[1], fields[2:]
	aliases := map[string]string{"MDEL": "MDELIDX", "MSET": "MSETIDX", "FGET": "FREADHT", "LGET": "LREADHT", "TGET": "TFIND", "ISMEMBER": "TFIND", "TCHECK": "TCHECKCOMPLETE"}
	if alias, ok := aliases[cmd]; ok {
		cmd = alias
	}
	op, ok := specs[cmd]
	if !ok {
		return false, fmt.Errorf("неизвестная команда: %s", cmd)
	}
	if len(args) < op.min || (op.max >= 0 && len(args) > op.max) {
		return false, fmt.Errorf("неверное число аргументов: %s", cmd)
	}
	numbers := []int{}
	if cmd == "TINSERT" || cmd == "TFIND" {
		for _, arg := range args {
			n, err := number(arg)
			if err != nil {
				return false, err
			}
			numbers = append(numbers, n)
		}
	}
	idx := 0
	if cmd == "MPUSHIDX" || cmd == "MGET" || cmd == "MDELIDX" || cmd == "MSETIDX" {
		n, err := number(args[0])
		if err != nil || n < 0 {
			return false, fmt.Errorf("некорректный индекс")
		}
		idx = n
	}
	o := objects[name]
	if cmd == "PRINT" {
		if o == nil {
			return false, fmt.Errorf("структура не найдена: %s", name)
		}
		fmt.Printf("=== PRINT %s ===\n", name)
		if o.kind == "Tree" {
			o.tree.Print()
		} else {
			fmt.Println(o.read())
		}
		return false, nil
	}
	if strings.HasSuffix(cmd, "CREATE") && o != nil {
		return false, fmt.Errorf("структура уже существует")
	}
	if o == nil {
		if !op.create {
			return false, fmt.Errorf("структура не найдена: %s", name)
		}
		o = newObject(op.kind)
		objects[name] = o
	}
	if o.kind != op.kind {
		return false, fmt.Errorf("имя занято другой структурой")
	}
	if op.kind == "Array" && len(args) > 0 && cmd != "MPUSH" {
		if idx > o.array.Length() || (idx == o.array.Length() && cmd != "MPUSHIDX") {
			return false, fmt.Errorf("индекс за пределами массива")
		}
	}
	switch cmd {
	case "MPUSH":
		for _, v := range args {
			o.array.PushBack(v)
		}
	case "MPUSHIDX":
		o.array.Insert(idx, args[1])
	case "MGET":
		value, _ := o.array.Get(idx)
		fmt.Println(value)
	case "MDELIDX":
		o.array.Erase(idx)
	case "MSETIDX":
		o.array.Set(idx, args[1])
	case "MLENGTH":
		fmt.Println(o.array.Length())
	case "MREAD":
		fmt.Println(o.array.Read())
	case "SPUSH":
		for _, v := range args {
			o.stack.PushBack(v)
		}
	case "QPUSH":
		for _, v := range args {
			o.queue.PushBack(v)
		}
	case "SPOP":
		if o.stack.List.Tail == nil {
			return false, fmt.Errorf("пустая структура")
		}
		fmt.Println(o.stack.Pop())
	case "QPOP":
		if o.queue.List.Head == nil {
			return false, fmt.Errorf("пустая структура")
		}
		fmt.Println(o.queue.Pop())
	case "SREAD":
		fmt.Println(o.stack.Read())
	case "QREAD":
		fmt.Println(o.queue.Read())
	case "TINSERT":
		for _, v := range numbers {
			o.tree.Insert(v)
		}
	case "TFIND":
		if o.tree.Find(numbers[0]) {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
	case "TCHECKCOMPLETE":
		if o.tree.IsComplete() {
			fmt.Println("TRUE")
		} else {
			fmt.Println("FALSE")
		}
	case "TREAD":
		fmt.Println(o.tree.Read())
	case "FPUSH":
		for _, v := range args {
			o.single.PushBack(v)
		}
	case "FPUSHHEAD":
		for _, v := range args {
			o.single.PushFront(v)
		}
	case "FPUSHBEFORE":
		o.single.PushBefore(args[0], args[1])
	case "FPUSHAFTER":
		o.single.PushAfter(args[0], args[1])
	case "FPOP":
		o.single.PopBack()
	case "FPOPHEAD":
		o.single.PopFront()
	case "FPOPBEFORE":
		o.single.PopBefore(args[0])
	case "FPOPAFTER":
		o.single.PopAfter(args[0])
	case "FDELVAL":
		o.single.DelByValue(args[0])
	case "FFINDVAL":
		fmt.Println(o.single.FindByValue(args[0]))
	case "FREADHT":
		fmt.Println(o.single.ReadHT())
	case "FREADTH":
		fmt.Println(o.single.ReadTH())
	case "LPUSH":
		for _, v := range args {
			o.double.PushBack(v)
		}
	case "LPUSHHEAD":
		for _, v := range args {
			o.double.PushFront(v)
		}
	case "LPUSHBEFORE":
		o.double.PushBefore(args[0], args[1])
	case "LPUSHAFTER":
		o.double.PushAfter(args[0], args[1])
	case "LPOP":
		o.double.PopBack()
	case "LPOPHEAD":
		o.double.PopFront()
	case "LPOPBEFORE":
		o.double.PopBefore(args[0])
	case "LPOPAFTER":
		o.double.PopAfter(args[0])
	case "LDELVAL":
		o.double.DelByValue(args[0])
	case "LFINDVAL":
		fmt.Println(o.double.Find(args[0]))
	case "LREADHT":
		fmt.Println(o.double.ReadHT())
	case "LREADTH":
		fmt.Println(o.double.ReadTH())
	}
	return op.change, nil
}
func run() error {
	if len(os.Args) != 5 {
		return fmt.Errorf("использование: dbms-go --file file.data --query 'КОМАНДА имя [аргументы]'")
	}
	file, query := "", ""
	for i := 1; i < len(os.Args); i += 2 {
		switch {
		case os.Args[i] == "--file" && file == "":
			file = os.Args[i+1]
		case os.Args[i] == "--query" && query == "":
			query = os.Args[i+1]
		default:
			return fmt.Errorf("неизвестный или повторный параметр")
		}
	}
	if file == "" || query == "" {
		return fmt.Errorf("нужны --file и --query")
	}
	if err := load(file); err != nil {
		return err
	}
	changed, err := execute(query)
	if err != nil {
		return err
	}
	if changed {
		return save(file)
	}
	return nil
}
func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "Ошибка:", err)
		os.Exit(1)
	}
}

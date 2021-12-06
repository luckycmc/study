package main
 
import (
    "math/rand"
    "time"
    "fmt"
)
 
const (
    P = 0.6
    MaxLevel = 8
)
 //随机层高
func randomLevel() int {
    i := 1
    rand.Seed(time.Now().UnixNano())
    for i < MaxLevel {
        p := rand.Float64()
        if (p < P) {
            i++
        } else {
            break
        }
    }
    return i
}
 //对应的数据节点
type node struct {
    level int
    nexts []*node   //切片指针
    v     int
}
 
type skipList struct {
    head *node    
}
 
func NewSkipList() *skipList {
    s := new(skipList)
    s.head = new(node)
    s.head.level = MaxLevel
    s.head.nexts = make([]*node, MaxLevel)
    return s
}
 //数据添加到节点
func (s *skipList) insert(v int) {
    l := randomLevel()
    add := new(node)
    add.level = l
    add.nexts = make([]*node, l)
    add.v = v
    i := l
 
    p := s.head
    for i > 0 {
        n := p.nexts[i - 1]
        for n != nil && n.v < v {
            p = n
            n = n.nexts[i - 1]
        }
        p.nexts[i - 1] = add
        add.nexts[i - 1] = n
        i --
    }
}
 
func (s *skipList) search(v int) *node {
    i := s.head.level
    p := s.head
    for i > 0 {
        n := p.nexts[i - 1]
        for n != nil && n.v < v {
            p = n
            n = n.nexts[i - 1]
        }
		fmt.Println("------------------------",i,n.v)
        if n != nil && n.v == v {
            return n
        }
        i --
		
    }
    return nil
}
 
func (s *skipList) delete(d *node) {
    i := d.level
    p := s.head
    for i > 0 {
        n := p.nexts[i - 1]
        for n != nil && n.v < d.v {
            p = n
            n = n.nexts[i - 1]
        }
        p.nexts[i - 1] = d.nexts[i - 1]
        i --
    }
}
 
func (s *skipList) print() {
    p := s.head
    for (p.nexts[0] != nil) {
        fmt.Println(p.nexts[0].v)
        p = p.nexts[0]
    }
}
 
func main() {
    s := NewSkipList()
    s.insert(8)
    s.insert(5)
    s.insert(3)
    s.insert(7)
    s.insert(4)
    s.insert(9)
    s.insert(100)
    s.insert(101)
    s.insert(55)
    s.insert(20)
    s.insert(18)
    s.insert(11)
    s.insert(15)
    s.insert(33)
    s.print()
 
    for i := 0; i < 2; i++ {
        n := s.search(7)
        if n != nil {
            fmt.Println("found")
            s.delete(n)
        } else {
            fmt.Println("no found")
        }
    }
 
    s.print()
}
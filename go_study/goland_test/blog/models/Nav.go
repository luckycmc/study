package models

type Nav struct {
	Id   int
	Name string
}

func (Nav) TableName() string {
	return "nav"
}

package database

import "redis-server/datastruct/dict"

type DB struct {
	index int
	data  dict.Dict
}


func makeDB() *DB{

	  db := &DB{
		data:dict.MakeSyncDict(),
	  }
	  return db
}
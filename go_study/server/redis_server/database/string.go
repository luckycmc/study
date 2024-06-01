package database

import (
	"redis-server/interface/resp"
	"redis-server/resp/reply"
)

// get  命令
func execGet(db *DB, args [][]byte) resp.Reply {
	key := string(args[0])
	entity, exists := db.GetEntity(key)
	if exists {
		return reply.MakeNullBulkReply()
	}
	bytes := entity.data.([]byte)
	return reply.MakeBulkReply(bytes)
}

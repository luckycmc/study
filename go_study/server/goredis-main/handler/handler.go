package handler

import (
	"context"
	"io"
	"net"
	"sync"
	"sync/atomic"

	"goredis-main/log"
	"goredis-main/server"
)

type Handler struct {
	sync.Once
	mu     sync.RWMutex
	conns  map[net.Conn]struct{}
	closed atomic.Bool

	db        DB
	parser    Parser
	persister Persister
	logger    log.Logger
}

func NewHandler(db DB, persister Persister, parser Parser, logger log.Logger) (server.Handler, error) {
	h := Handler{
		conns:     make(map[net.Conn]struct{}),
		persister: persister,
		logger:    logger,
		db:        db,
		parser:    parser,
	}

	return &h, nil
}
// server 启动
func (h *Handler) Start() error {
	// 加载持久化文件，还原内容
	reloader, err := h.persister.Reloader()
	if err != nil {
		return err
	}
	defer reloader.Close()
	//执行 handler 回复数据 
	h.handle(SetLoadingPattern(context.Background()), newFakeReaderWriter(reloader))
	return nil
}
// 实现handler 层面的处理器:
func (h *Handler) Handle(ctx context.Context, conn net.Conn) {
	h.mu.Lock()
	// 判断 db 是否已经关闭
	if h.closed.Load() {
		h.mu.Unlock()
		return
	}

	// 当前 conn 缓存起来
	h.conns[conn] = struct{}{}
	h.mu.Unlock()

	h.handle(ctx, conn)
}
//协议处理
func (h *Handler) handle(ctx context.Context, conn io.ReadWriter) {
	// 持续处理
	stream := h.parser.ParseStream(conn)
	for {
		select {
		case <-ctx.Done():
			h.logger.Warnf("[handler]handle ctx err: %s", ctx.Err().Error())
			return
        // 处理解析后的数据
		case droplet := <-stream:
			if err := h.handleDroplet(ctx, conn, droplet); err != nil {
				h.logger.Errorf("[handler]conn terminated, err: %s", droplet.Err.Error())
				return
			}
		}
	}
}
//处理用户返回的信息
func (h *Handler) handleDroplet(ctx context.Context, conn io.ReadWriter, droplet *Droplet) error {
	if droplet.Terminated() {
		return droplet.Err
	}

	if droplet.Err != nil {
		_, _ = conn.Write(droplet.Reply.ToBytes())
		h.logger.Errorf("[handler]conn request, err: %s", droplet.Err.Error())
		return nil
	}

	if droplet.Reply == nil {
		h.logger.Errorf("[handler]conn empty request")
		return nil
	}

	// 请求参数必须为 multiBulkReply 类型
	multiReply, ok := droplet.Reply.(MultiReply)
	if !ok {
		h.logger.Errorf("[handler]conn invalid request: %s", droplet.Reply.ToBytes())
		return nil
	}

	if reply := h.db.Do(ctx, multiReply.Args()); reply != nil {
		_, _ = conn.Write(reply.ToBytes())
		return nil
	}

	_, _ = conn.Write(UnknownErrReplyBytes)
	return nil
}

func (h *Handler) Close() {
	h.Once.Do(func() {
		h.logger.Warnf("[handler]handler closing...")
		h.closed.Store(true)
		h.mu.RLock()
		defer h.mu.RUnlock()
		for conn := range h.conns {
			if err := conn.Close(); err != nil {
				h.logger.Errorf("[handler]close conn err, local addr: %s, err: %s", conn.LocalAddr().String(), err.Error())
			}
		}
		h.conns = nil
		h.db.Close()
		h.persister.Close()
	})
}

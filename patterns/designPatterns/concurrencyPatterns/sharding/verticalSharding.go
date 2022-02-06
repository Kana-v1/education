package sharding

import (
	"crypto/sha1"
	"sync"
)

type Shard struct {
	sync.RWMutex

	m map[string]interface{}
}

type ShardedMap []*Shard

func NewShardedMap(nshards int) ShardedMap {
	shards := make([]*Shard, 0, nshards)

	for i := 0; i < nshards; i++ {
		shard := make(map[string]interface{})
		shards = append(shards, &Shard{m: shard})
	}

	return shards
}

func (m ShardedMap) getShardIndex(key string) int {
	checkSum := sha1.Sum([]byte(key)) // checkSum - []byte, so in slice there can be max 255 vals

	hash := int(checkSum[17]) // 17 - rand num

	return hash % len(m)
}

func (m ShardedMap) getShard(key string) *Shard {
	index := m.getShardIndex(key)

	return m[index]
}

func (m ShardedMap) Get(key string) interface{} {
	shard := m.getShard(key)
	shard.RLock()
	defer shard.RUnlock()

	return shard.m[key]
}

func (m ShardedMap) Set(key string, value interface{}) {
	shard := m.getShard(key)
	shard.Lock()

	shard.m[key] = value
	shard.Unlock()
}

func (m ShardedMap) Keys() []string {
	keys := make([]string, 0)

	wg := new(sync.WaitGroup)
	wg.Add(len(m))

	for _, shard := range m {
		go func(s *Shard) {
			s.RLock()

			for key := range s.m {
				keys = append(keys, key)
			}

			s.RUnlock()
			wg.Done()
		}(shard)
	}

	wg.Wait()

	return keys
}

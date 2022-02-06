package promise

import (
	"context"
	"sync"
	"time"
)

type Promise interface {
	Result() (string, error)
}

type InnerPromise struct {
	once *sync.Once
	wg   *sync.WaitGroup

	res   string
	err   error
	resCh <-chan string
	errCh <-chan error
}

func (promise *InnerPromise) Result() (string, error) {
	promise.once.Do(func() { // once.Do() это чтобы закешировать результат
		promise.wg.Add(1)
		defer promise.wg.Done()

		promise.res = <-promise.resCh
		promise.err = <-promise.errCh
	})

	promise.wg.Wait()

	return promise.res, promise.err
}

func SlowFunction(ctx context.Context) Promise {
	resCh := make(chan string)
	errCh := make(chan error)

	go func() {
		select {
		case <-time.After(2 * time.Second):
			resCh <- "I slept for 2 seonds"
			errCh <- nil
		case <-ctx.Done():
			resCh <- ""
			errCh <- ctx.Err()
		}
	}()

	return &InnerPromise{
		once: new(sync.Once),
		wg:   new(sync.WaitGroup),

		resCh: resCh,
		errCh: errCh,
	}
}

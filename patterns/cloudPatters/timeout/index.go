package timeout

import (
	"context"
	"time"
)

type timeout struct {
	timeout time.Duration
}

func NewTimeout(latency time.Duration) *timeout {
	return &timeout{
		timeout: latency,
	}
}

func (t timeout) slowFunction() (string, error) {
	return "", nil
}

func (t timeout) Timeout() (string, error) {
	ctx, _ := context.WithTimeout(context.Background(), t.timeout)

	chErr := make(chan error)
	chRes := make(chan string)

	go func(chErr chan<- error, chRes chan<- string) {
		res, err := t.slowFunction()

		chRes <- res
		chErr <- err
	}(chErr, chRes)

	select {
	case <-ctx.Done():
		return "", ctx.Err()
	case res := <-chRes:
		return res, <-chErr
	}
}

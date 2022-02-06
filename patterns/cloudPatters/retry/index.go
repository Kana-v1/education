package retry

import (
	"context"
	"log"
	"time"
)

type Retry struct {
	retryTimes          uint
	delayBetweenRetries time.Duration
}

func (r *Retry) effector(ctx context.Context) (string, error) {
	return "", nil
}

func (r *Retry) Retry(ctx context.Context) (res string, err error) {
	for attempt := 1; attempt <= int(r.retryTimes); attempt++ {
		res, err = r.effector(ctx)

		if err == nil {
			return
		}

		log.Printf("%v attempt of calling the effector failed", attempt)

		select {
		case <-ctx.Done():
			return
		default:
		}
	}

	return
}

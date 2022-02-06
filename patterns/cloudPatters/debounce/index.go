package debounce

import (
	"context"
	"time"
)

type response struct {
	result string
	err    error
}

type Debounce struct {
	cache map[int]response

	latency        time.Duration
	lastTimeCalled map[int]*time.Time
}

func (d *Debounce) circuit(ctx context.Context) (string, error) {
	return "response", nil
}

func (d *Debounce) DebounceFunctionFirst(ctx context.Context, caller int) (string, error) {
	if d.lastTimeCalled[caller] != nil {
		if d.lastTimeCalled[caller].Add(d.latency).Before(time.Now()) {
			callTime := time.Now()
			d.lastTimeCalled[caller] = &callTime

			res, err := d.circuit(ctx)

			d.cache[caller] = response{res, err}

			return res, err
		}
	}

	return d.cache[caller].result, d.cache[caller].err
}

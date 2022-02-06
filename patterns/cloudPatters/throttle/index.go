package throttle

import (
	"context"
	"fmt"
)

type Throttle struct {
	tokenBucket uint16
}

func (t *Throttle) effector(ctx context.Context) (string, error) {
	return "", nil
}

func (t *Throttle) Throttle(ctx context.Context) (string, error) {
	if t.tokenBucket == 0 {
		return "", fmt.Errorf("can not use this service right now")
	}

	t.tokenBucket--

	res, err := t.effector(ctx)

	t.tokenBucket++
	return res, err
}

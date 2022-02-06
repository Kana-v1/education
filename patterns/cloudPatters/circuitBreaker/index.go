package circuitBreaker

import (
	"context"
	"fmt"
	"time"
)

type CircuitBreaker struct {
	lastStateSuccessful bool
	lastAttempt         time.Time
	consecutiveFailures uint
	failureThreshold    uint
	shouldRetryAt       *time.Time
}

func (c *CircuitBreaker) circuit(ctx context.Context) (string, error) {
	fmt.Println(ctx)

	return "", nil
}

func (c *CircuitBreaker) Breaker(ctx context.Context) (string, error) {
	if c.shouldRetryAt != nil {
		if !time.Now().After(*c.shouldRetryAt) {
			return "", fmt.Errorf("service unreachable up to %v", c.shouldRetryAt)
		}

		c.shouldRetryAt = nil
		c.consecutiveFailures = 0
	}

	response, err := c.circuit(ctx)

	if err != nil {
		if c.consecutiveFailures >= c.failureThreshold {
			backoff := c.consecutiveFailures - c.failureThreshold

			nextAttemptTime := time.Now().Add(2 * time.Second * time.Duration(backoff))
			c.shouldRetryAt = &nextAttemptTime
		}

		c.consecutiveFailures++

		return response, err
	}

	return response, nil
}

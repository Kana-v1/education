package promise

import "time"

type Matrix struct{}

func Inverse(m Matrix) <-chan Matrix {
	out := make(chan Matrix)

	go func() {
		time.Sleep(5 * time.Second) // moke some work
		out <- Matrix{}
		close(out)
	}()

	return out
}

func InverseProduct(a, b Matrix) (firstPromise, secondPromise <-chan Matrix) {
	invA := make(chan Matrix)
	invB := make(chan Matrix)

	go func() {
		val := <-Inverse(a)
		invA <- val
	}()
	go func() {
		val := <-Inverse(b)
		invB <- val
	}()

	return invA, invB
}

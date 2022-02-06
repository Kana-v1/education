//обертка над объектом чтобы не лезть внутрь (тип если мы хотим чтобы метод возвращал не 1, а 2 и не хотим лезть внутрь)

package main

import "fmt"

type pizza interface {
	getPrice() int
}

type veggeMania struct { // component

}

func (p *veggeMania) getPrice() int {
	return 15
}

type tomatoToping struct { // decorator
	pizza pizza
}

func (c *tomatoToping) getPrice() int {
	pizzaPrice := c.pizza.getPrice()

	return pizzaPrice + 7
}

type cheeseToping struct { // decorator
	pizza pizza
}

func (c *cheeseToping) getPrice() int {
	return c.pizza.getPrice() + 10
}

func decoratorMain() {
	pizza := new(veggeMania)

	pizzaWithCheese := &cheeseToping{
		pizza: pizza,
	}

	pizzaWithCheeseAndTomato := &tomatoToping{
		pizza: pizzaWithCheese,
	}

	fmt.Printf("Price of veggemania with tomato and cheese topping is %d\n", pizzaWithCheeseAndTomato.getPrice())
}

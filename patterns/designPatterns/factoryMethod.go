package main

import "fmt"

type iGun interface {
	setName(string)
	setPower(int)
	getName() string
	getPower() int
}

type gun struct { // concrete product
	name  string
	power int
}

func (g *gun) setName(name string) {
	g.name = name
}

func (g *gun) setPower(power int) {
	g.power = power
}

func (g *gun) getName() string {
	return g.name
}

func (g *gun) getPower() int {
	return g.power
}

type ak47 struct { // concrete product
	gun
}

func newAk47() iGun {
	return &ak47{
		gun{
			name:  "ak57 gun",
			power: 4,
		},
	}
}

type musket struct { // concrete product
	gun
}

func newMusket() iGun {
	return &musket{
		gun{
			name:  "Musket gun",
			power: 1,
		},
	}
}

func getGun(gunType string) (iGun, error) { // factory
	if gunType == "ak47" {
		return newAk47(), nil
	}

	if gunType == "musket" {
		return newMusket(), nil
	}

	return nil, fmt.Errorf("wrong gun type")
}

func factoryMethodMain() {
	ak47, _ := getGun("ak47")
	musket, _ := getGun("musket")

	printDetails(ak47)
	printDetails(musket)
}

func printDetails(g iGun) {
	fmt.Printf("Gun: %s", g.getName())
	fmt.Println()
	fmt.Printf("Power: %d", g.getPower())
	fmt.Println()
}

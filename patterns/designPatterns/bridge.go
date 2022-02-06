package patters

import "fmt"

type printer interface { // realization
	printFile()
}

type epson struct{} // concrete realization

func (e *epson) printFile() {
	fmt.Println("epson printing file")
}

type hp struct{} // concrete realization
func (hp *hp) printFile() {
	fmt.Println("hp printing file")
}

type computer interface { // abstraction
	print()
	setPrinter(printer)
}

type mac struct { // extended abstraction
	printer printer
}

func (m *mac) print() {
	m.printer.printFile()
}

func (m *mac) setPrinter(p printer) {
	m.printer = p
}

type windows struct { // extended abstraction
	printer printer
}

func (w *windows) print() {
	w.printer.printFile()
}

func (w *windows) setPrinter(p printer) {
	w.printer = p
}

func bridgeMain() {

	hpPrinter := &hp{}
	epsonPrinter := &epson{}

	macComputer := &mac{}

	macComputer.setPrinter(hpPrinter)
	macComputer.print()
	fmt.Println()

	macComputer.setPrinter(epsonPrinter)
	macComputer.print()
	fmt.Println()

	winComputer := &windows{}

	winComputer.setPrinter(hpPrinter)
	winComputer.print()
	fmt.Println()

	winComputer.setPrinter(epsonPrinter)
	winComputer.print()
	fmt.Println()
}

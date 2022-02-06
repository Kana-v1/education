package main

type button struct { // sender
	command command
}

func (b *button) pressOn() {
	b.command.execute()
}

func (b *button) pressOff() {
	b.command.unexecute()
}

type command interface { // abstract command
	execute()
	unexecute()
}

type Command struct { // concrete command
	device device
}

func (c *Command) execute() {
	c.device.on()
}

func (c *Command) unexecute() {
	c.device.off()
}

type device interface { // receiver
	on()
	off()
}

type tv struct {
	isRunning bool
}

func (t *tv) on() {
	t.isRunning = true
}

func (t *tv) off() {
	t.isRunning = false
}

func commandMain() {
	tv := new(tv)

	command := &Command{
		device: tv,
	}

	onButton := &button{
		command: command,
	}

	onButton.pressOn()
	onButton.pressOff()

}

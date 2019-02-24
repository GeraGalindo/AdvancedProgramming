package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"os/exec"
	"strconv"
	"time"
)

func printError(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func printUsage() {
	fmt.Println("Usage: ./clockWall <n>")
	fmt.Println("--------------------")
	fmt.Println("Where n is:")
	fmt.Println("n = 1 -> New York")
	fmt.Println("n = 2 -> Tokyo")
	fmt.Println("n = 3 -> London")
	fmt.Println("n = 4 -> New York and Tokyo")
	fmt.Println("n = 5 -> New York and London")
	fmt.Println("n = 6 -> Tokyo and London")
	fmt.Println("n = 7 -> New York, Tokyo and Tokyo")
	fmt.Println("anything else -> Local Time")
}

func initClockServers(n int) {
	cmd := exec.Command("clear")
	err := cmd.Start()
	printError(err)

	// Init New York Time Zone Server
	if n == 1 || n == 4 || n == 5 || n == 7 {
		cmd = exec.Command("./clock2", "-port", "8010")
		err = cmd.Start()
		printError(err)
	}

	// Init Tokyo Time Zone Server
	if n == 2 || n == 4 || n == 6 || n == 7 {
		cmd = exec.Command("./clock2", "-port", "8020")
		err = cmd.Start()
		printError(err)
	}

	// Init London Time Zone Server
	if n == 3 || n == 5 || n == 6 || n == 7 {
		cmd = exec.Command("./clock2", "-port", "8030")
		err = cmd.Start()
		printError(err)
	}

	// Init Local Time Zone Server
	if n < 1 || n > 7 {
		cmd = exec.Command("./clock2", "-port", "9090")
		err = cmd.Start()
		printError(err)
	}
}

func shiftMsg(delay time.Duration, msg string) {
	for _, r := range msg {
		fmt.Printf("%c", r)
		time.Sleep(delay)
	}
}

func listenToServers(n int) {
	var message string
	switch n {
	case 1:
		conn, err := net.Dial("tcp", "localhost:8010")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')
	case 2:
		conn, err := net.Dial("tcp", "localhost:8020")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')
	case 3:
		conn, err := net.Dial("tcp", "localhost:8030")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')
	case 4:
		conn, err := net.Dial("tcp", "localhost:8010")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')

		conn2, err := net.Dial("tcp", "localhost:8020")
		printError(err)
		message2, _ := bufio.NewReader(conn2).ReadString('\n')
		message += message2
	case 5:
		conn, err := net.Dial("tcp", "localhost:8010")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')

		conn2, err := net.Dial("tcp", "localhost:8030")
		printError(err)
		message2, _ := bufio.NewReader(conn2).ReadString('\n')
		message += message2
	case 6:
		conn, err := net.Dial("tcp", "localhost:8020")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')

		conn2, err := net.Dial("tcp", "localhost:8030")
		printError(err)
		message2, _ := bufio.NewReader(conn2).ReadString('\n')
		message += message2
	case 7:
		conn, err := net.Dial("tcp", "localhost:8010")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')

		conn2, err := net.Dial("tcp", "localhost:8020")
		printError(err)
		message2, _ := bufio.NewReader(conn2).ReadString('\n')
		message += message2

		conn3, err := net.Dial("tcp", "localhost:8030")
		printError(err)
		message3, _ := bufio.NewReader(conn3).ReadString('\n')
		message += message3
	default:
		conn, err := net.Dial("tcp", "localhost:9090")
		printError(err)
		message, _ = bufio.NewReader(conn).ReadString('\n')
	}

	for {
		shiftMsg(100*time.Millisecond, message)
	}

}

func main() {
	if len(os.Args) != 2 {
		printUsage()
		os.Exit(1)
	}

	n, err := strconv.Atoi(os.Args[1])
	printError(err)

	initClockServers(n)
	time.Sleep(1 * time.Second)
	listenToServers(n)
}

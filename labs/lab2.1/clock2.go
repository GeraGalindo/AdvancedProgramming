// Clock1 is a concurrent TCP server that periodically writes the time.
package main

import (
	"fmt"
	"io"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
)

func handleConn(c net.Conn, port int) {
	defer c.Close()
	for {
		loc, err := time.LoadLocation(os.Getenv("TZ"))
		printError(err)
		now := time.Now().In(loc)
		_, err = io.WriteString(c, fmt.Sprintf("%s: %s", os.Getenv("TZ"),
			now.Format("15:04:05\n")))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func printError(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func main() {
	fmt.Printf("%s Clock Server Running...", os.Getenv("TZ"))

	if len(os.Args) < 3 || strings.Compare(os.Args[1], "-port") != 0 {
		fmt.Println("Usage: ./clock2 -port <port_number>")
		os.Exit(1)
	}

	portNo, err := strconv.Atoi(os.Args[2])
	printError(err)

	listener, err := net.Listen("tcp", fmt.Sprintf("localhost:%s", os.Args[2]))
	printError(err)

	for {
		conn, err := listener.Accept()
		printError(err)
		go handleConn(conn, portNo) // handle one connection at a time
	}
}

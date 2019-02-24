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
		switch port {
		case 8010:
			loc, err := time.LoadLocation("America/New_York")
			printError(err)
			now := time.Now().In(loc)
			_, err = io.WriteString(c, fmt.Sprintf("New York: %s", now.Format("15:04:05\n")))
			if err != nil {
				return // e.g., client disconnected
			}
		case 8020:
			loc2, err := time.LoadLocation("Asia/Tokyo")
			printError(err)
			now2 := time.Now().In(loc2)
			_, err = io.WriteString(c, fmt.Sprintf("Tokyo: %s", now2.Format("15:04:05\n")))
			if err != nil {
				return // e.g., client disconnected
			}
		case 8030:
			loc3, err := time.LoadLocation("Europe/London")
			printError(err)
			now3 := time.Now().In(loc3)
			_, err = io.WriteString(c, fmt.Sprintf("London: %s", now3.Format("15:04:05\n")))
			if err != nil {
				return // e.g., client disconnected
			}
		default:
			_, err := io.WriteString(c, fmt.Sprintf("Local Time:%s", time.Now().Format("15:04:05\n")))
			if err != nil {
				return // e.g., client disconnected
			}
		}
		time.Sleep(1 * time.Second)
	}
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}

func printError(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func main() {
	fmt.Println("Clock Running...")

	if len(os.Args) < 3 || strings.Compare(os.Args[1], "-port") != 0 {
		fmt.Println("Usage: ./clock2 -port <port_number>")
		os.Exit(1)
	}

	portNo, err := strconv.Atoi(os.Args[2])
	checkError(err)

	listener, err := net.Listen("tcp", fmt.Sprintf("localhost:%s", os.Args[2]))
	printError(err)

	for {
		conn, err := listener.Accept()
		printError(err)
		go handleConn(conn, portNo) // handle one connection at a time
	}
}

package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
	"time"
)

func printError(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func printUsage() {
	fmt.Println("Usage: ./clockWall City=host:port")
	fmt.Println("For Example:")
	fmt.Println("Usage: ./clockWall NewYork=localhost:8010 Tokyo=192.168.2.1:8020")
}

func shiftMsg(delay time.Duration, msg string, ch chan int) {
	for _, r := range msg {
		fmt.Printf("%c", r)
		time.Sleep(delay)
	}
	ch <- 1
}

func getHostAndPort(tz string) string {
	return tz[strings.Index(tz, "=")+1 : len(tz)]
}

func main() {

	if len(os.Args) < 2 {
		printUsage()
		os.Exit(1)
	}

	for i := range os.Args {
		if i != 0 {
			hostAndPort := getHostAndPort(os.Args[i])

			ch := make(chan int)
			go func(ch chan int) {
				conn, err := net.Dial("tcp", hostAndPort)
				printError(err)
				defer conn.Close()

				message, _ := bufio.NewReader(conn).ReadString('\n')

				shiftMsg(100*time.Millisecond, message, ch)

			}(ch)
			<-ch
		}

	}

}

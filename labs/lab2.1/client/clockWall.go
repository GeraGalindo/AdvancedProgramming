package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strings"
	"sync"
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

func shiftMsg(delay time.Duration, msg string) {
	for _, r := range msg {
		fmt.Printf("%c", r)
		time.Sleep(delay)
	}
}

func getHostAndPort(tz string) string {
	return tz[strings.Index(tz, "=")+1 : len(tz)]
}

func main() {
	var wg sync.WaitGroup

	if len(os.Args) < 2 {
		printUsage()
		os.Exit(1)
	}

	for i := range os.Args {
		if i != 0 {
			hostAndPort := getHostAndPort(os.Args[i])
			wg.Add(1)
			go func() {
				conn, err := net.Dial("tcp", hostAndPort)
				printError(err)
				//defer conn.Close()
				//for {
				message, _ := bufio.NewReader(conn).ReadString('\n')
				//shiftMsg(100*time.Millisecond, message)
				fmt.Print(message)
				wg.Done()
				//}
			}()
		}

	}
	wg.Wait()

}

package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel
var who string
var users map[string]string
var connections map[string]net.Conn

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
	pMessage = make(chan string) // private message
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)

		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	who = conn.RemoteAddr().String()
	ch <- "irc-server > You are connected"
	messages <- "irc-server > New user has arrived"
	entering <- ch

	input := bufio.NewScanner(conn)
	for input.Scan() {
		if strings.Contains(input.Text(), ":**:"){
			split := strings.Index(input.Text(), ":**:")
			users[conn.RemoteAddr().String()] = input.Text()[:split]
			messages <- users[conn.RemoteAddr().String()] + " > " + input.Text()[split+4:]
			continue
		}
		if strings.Compare(input.Text(), "/users") == 0 {
			var sb strings.Builder
			sb.WriteString("irc-server > ")
			for usr := range users {
				sb.WriteString(users[usr] + ", ")
			}
			ch <- sb.String()[:len(sb.String())-2]
			continue
		}
		if strings.Contains(input.Text(), "/user ") { // username: user2, IP: 192.168.1.22
			found := false
			tmpUsr := input.Text()[6:]
			for usr := range users {
				if strings.Compare(users[usr], tmpUsr) == 0 {
					ch <- "irc-server > username: " + users[usr] + ", IP: " + usr[:len(usr)-6]
					found = true
				}
			}
			if !found {
				ch <- "irc-server > user not found"
			}
			continue
		}
		if strings.Contains(input.Text(), "/msg ") {

		}
		if strings.Compare(input.Text(), "/time") == 0 {
			zone, _ := time.Now().Zone()
			ch <- "irc-server > Local Time: " + time.Now().Format("15:04:05") + " " + zone
			continue
		} else {
			messages <- users[conn.RemoteAddr().String()] + " > " + input.Text()
		}
	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	messages <- users[conn.RemoteAddr().String()] + " has left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {
	if len(os.Args) < 5 || strings.Compare(os.Args[1], "-host") != 0 || strings.Compare(os.Args[3], "-port") != 0 {
		fmt.Println("Usage: go run server.go -host localhost -port 9000")
		os.Exit(1)
	}

	users = make(map[string]string)

	var sb strings.Builder
	sb.WriteString(os.Args[2])
	sb.WriteString(":")
	sb.WriteString(os.Args[4])

	listener, err := net.Listen("tcp", sb.String())
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("irc-server > Simple IRC Server started at %s\n", sb.String())
	fmt.Printf("irc-server > Ready for receiving new clients\n")

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main

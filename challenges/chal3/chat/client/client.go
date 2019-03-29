package main

import (
"fmt"
"io"
"log"
"net"
"os"
"strings"
)

///                0        1    2     3        4
//!+    go run client.go -user user1 -server localhost:9000
func main() {
	if len(os.Args) < 5 || strings.Compare(os.Args[1], "-user") != 0 || strings.Compare(os.Args[3], "-server") != 0 {
		fmt.Println("Usage: go run client.go -user user_name -server localhost:9000")
		os.Exit(1)
	}

	conn, err := net.Dial("tcp", os.Args[4])
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan struct{})

	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		fmt.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	io.WriteString(conn, os.Args[2] + ":**:")
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

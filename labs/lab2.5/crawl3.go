// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"sync"

	"gopl.io/ch5/links"
)

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)
var maxDepth int
var currentDepth int
var seen = make(map[string]bool)
var lock = sync.Mutex{}

func checkError(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func crawl(url string, depth int, wg *sync.WaitGroup) {
	defer wg.Done()
	if depth > maxDepth {
		return
	}

	tokens <- struct{}{} // obtain token
	list, err := links.Extract(url)
	<-tokens // release token
	checkError(err)

	for _, link := range list {
		lock.Lock()
		if seen[link] {
			lock.Unlock()
			continue
		}
		seen[link] = true
		lock.Unlock()
		wg.Add(1)
		go crawl(link, depth+1, wg)
	}

	fmt.Printf("Depth: %d\t", depth)
	fmt.Printf("URL: %s\n", url)

}

//!-sema

//!+
func main() {
	flag.IntVar(&maxDepth, "depth", 0, "Maximum Depth Crawl")
	flag.Parse()
	currentDepth = 0

	waitGroup := &sync.WaitGroup{}

	for _, link := range flag.Args() {
		waitGroup.Add(1)
		go crawl(link, currentDepth, waitGroup)
	}
	waitGroup.Wait()
}

//!-

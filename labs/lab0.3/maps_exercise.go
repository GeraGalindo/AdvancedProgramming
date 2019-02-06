/*
	Copy and Paste this code in Go Playground
	https://tour.golang.org/moretypes/23
*/

package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {
	words := strings.Fields(s)
	mapa := make(map[string]int)
	
	for i := range words {
		elem, ok := mapa[words[i]]
		if (ok){
			mapa[words[i]] = elem + 1
		} else {
			mapa[words[i]] = 1
		}
	}
	
	return mapa
}

func main() {
	wc.Test(WordCount)
}

/*
	Copy and Paste this code in the Go Playground
	https://tour.golang.org/moretypes/18
*/

package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	matrix := make([][]uint8, dy, dy)
	
	for i := range matrix {
		matrix[i] = make([]uint8, dx)
		for j := range matrix[i] {
			matrix[i][j] = uint8(i*j)
		}
	}
	
	return matrix
}

func main() {
	pic.Show(Pic)
}

package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {
	err := filepath.Walk(dir, processPath)
	if err != nil {
		return err
	}

	return nil
}

func logType(s string) {
	file, err := os.OpenFile("log.txt", os.O_RDWR|os.O_APPEND|os.O_CREATE, 0666)
	checkError(err)

	defer file.Close()

	_, err = io.WriteString(file, s)
	checkError(err)
}

func checkError(err error) {
	if err != nil {
		panic(err)
	}
}

func processPath(path string, info os.FileInfo, err error) error {
	if err != nil {
		m := info.Mode()
		if m&(1<<2) != 0 {
			return err
		} else {
			logType("Permission denied\n")
			//fmt.Println("Permission Denied")
		}
	}

	fi, err := os.Lstat(path)
	checkError(err)

	switch mode := fi.Mode(); {
	case mode.IsDir():
		logType("Directory\n")
		//fmt.Println("Directory")
	case mode&os.ModeSymlink != 0:
		logType("Symbolic link\n")
		//fmt.Println("Symbolic Link")
	default:
		logType("Other files\n")
		//fmt.Println("Other Files")
	}

	return nil
}

type Ocurrences struct {
	Dirs  int
	Perms int
	Syms  int
	Oths  int
}

func printStats(dir string) {
	directories := Ocurrences{0, 0, 0, 0}
	file, err := os.OpenFile("log.txt", os.O_RDONLY, 0666)
	checkError(err)
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		//fmt.Println(scanner.Text())
		if strings.Compare(scanner.Text(), "Permission denied") == 0 {
			directories.Perms++
		}
		if strings.Compare(scanner.Text(), "Directory") == 0 {
			directories.Dirs++
		}
		if strings.Compare(scanner.Text(), "Other files") == 0 {
			directories.Oths++
		}
		if strings.Compare(scanner.Text(), "Symbolic link") == 0 {
			directories.Syms++
		}
	}
	fmt.Println("Directory Scanner Tool")
	fmt.Printf("+-------------------------+------+\n")
	fmt.Printf("| Path                    | %v |\n", dir)
	fmt.Printf("+-------------------------+------+\n")
	fmt.Printf("| Directories             | %v   |\n", directories.Dirs)
	fmt.Printf("| Permission denied files | %v   |\n", directories.Perms)
	fmt.Printf("| Symbolic links          | %v   |\n", directories.Syms)
	fmt.Printf("| Other files             | %v   |\n", directories.Oths)
	fmt.Printf("+-------------------------+------+\n")
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	_, err := os.Stat("log.txt")
	if err == nil {
		os.Remove("log.txt")
	}

	err = scanDir(os.Args[1])
	checkError(err)

	printStats(os.Args[1])

	_, err = os.Stat("log.txt")
	if err == nil {
		os.Remove("log.txt")
	}

}

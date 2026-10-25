package main

import (
	//"os"
	//"path/filepath"
	//"strings"

	"github.com/jung-kurt/gofpdf"
)

const (
	WIDTH = 210
	HSPACE = 5
	VSPACE = 5
)

const (
	left = HSPACE
	right = WIDTH-HSPACE
	width = right-left
)

type rgb struct {
	r, g, b int
}

var (
	ink = rgb{34, 34, 34}
)

func write(pdf *gofpdf.Fpdf, x, y float64, text string, size float64, alignment string, bold bool, color rgb) {
	style := ""
	if bold {
		style = "B"
	}

	pdf.SetFont("lmroman", style, size)
	pdf.SetTextColor(color.r, color.g, color.b)
	if (alignment == "R") {
		pdf.SetXY(x-45, y)
		pdf.CellFormat(45, 4, text, "", 0, alignment, false, 0, "")
	} else {
		pdf.SetXY(x, y)
		pdf.CellFormat(0, 4, text, "", 0, alignment, false, 0, "")
	}
}

func line(pdf *gofpdf.Fpdf, x1, y1, x2, y2 float64, color rgb, thickness float64) {
	pdf.SetDrawColor(color.r, color.g, color.b)
	pdf.SetLineWidth(thickness)
	pdf.Line(x1, y1, x2, y2)
}

func write_header(pdf *gofpdf.Fpdf) {
	write(pdf, left, 3, "CHAKOTEAM!", 12, "L", true, ink)
	write(pdf, 0, 3, "ICPC Notebook", 12, "C", true, ink)
	write(pdf, right, 3, "Page 1", 12, "R", true, ink)
	line(pdf, left, 8, right, 8, ink, 0.2)
}

func main() {
	pdf := gofpdf.New("P", "mm", "A4", "")
	pdf.SetMargins(0, 0, 0)
	pdf.SetAutoPageBreak(false, 0)
	pdf.AddUTF8Font(
		"lmroman", "",
		"vendor/lmroman10-regular.ttf",
	)
	pdf.AddUTF8Font(
		"lmroman", "B",
		"vendor/lmroman10-bold.ttf",
	)
	
	pdf.SetFont("lmroman", "", 11)

	//filepath.Walk("src")
	//Title page
	pdf.AddPage()
	write_header(pdf)
	write(pdf, left, 31, "Bookhand CHAKOTEAM!", 20, "C", true, ink)
	pdf.OutputFileAndClose("bookhand.pdf")
}

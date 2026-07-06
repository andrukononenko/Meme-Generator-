// meme_generator.go
package main

import (
	"flag"
	"fmt"
	"image"
	"image/color"
	"image/draw"
	"image/jpeg"
	_ "image/png"
	"io/ioutil"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/fogleman/gg"
)

func parseHexColor(c string) color.RGBA {
	if len(c) == 7 && c[0] == '#' {
		r, _ := strconv.ParseInt(c[1:3], 16, 8)
		g, _ := strconv.ParseInt(c[3:5], 16, 8)
		b, _ := strconv.ParseInt(c[5:7], 16, 8)
		return color.RGBA{uint8(r), uint8(g), uint8(b), 255}
	}
	return color.RGBA{255, 255, 255, 255}
}

func memeGenerator(inputPath, outputPath, topText, bottomText string,
	fontSize float64, fontPath string, colorHex string,
	strokeColorHex string, strokeWidth float64, shadow bool, padding float64) error {
	img, err := gg.LoadImage(inputPath)
	if err != nil {
		return err
	}
	w, h := img.Bounds().Dx(), img.Bounds().Dy()
	dc := gg.NewContext(w, h)
	dc.DrawImage(img, 0, 0)

	// Шрифт
	if fontPath != "" {
		if err := dc.LoadFontFace(fontPath, fontSize); err != nil {
			// fallback
			dc.SetRGB(1, 1, 1)
		}
	} else {
		dc.SetRGB(1, 1, 1)
	}

	textColor := parseHexColor(colorHex)
	strokeColor := parseHexColor(strokeColorHex)

	// Верхний текст
	if topText != "" {
		dc.SetColor(textColor)
		dc.SetStrokeColor(strokeColor)
		dc.SetStrokeWidth(strokeWidth)
		dc.DrawStringAnchored(topText, float64(w)/2, padding, 0.5, 0)
	}

	// Нижний текст
	if bottomText != "" {
		dc.SetColor(textColor)
		dc.SetStrokeColor(strokeColor)
		dc.SetStrokeWidth(strokeWidth)
		dc.DrawStringAnchored(bottomText, float64(w)/2, float64(h)-padding, 0.5, 1)
	}

	// Сохранение
	return dc.SavePNG(outputPath)
}

func batchProcess(inputDir, outputDir, topText, bottomText string,
	fontSize float64, fontPath, colorHex, strokeColorHex string,
	strokeWidth float64, shadow bool, padding float64) error {
	os.MkdirAll(outputDir, 0755)
	exts := map[string]bool{".png": true, ".jpg": true, ".jpeg": true, ".bmp": true, ".tiff": true, ".webp": true}
	files, err := ioutil.ReadDir(inputDir)
	if err != nil {
		return err
	}
	var imageFiles []string
	for _, f := range files {
		if !f.IsDir() && exts[strings.ToLower(filepath.Ext(f.Name()))] {
			imageFiles = append(imageFiles, filepath.Join(inputDir, f.Name()))
		}
	}
	for i, f := range imageFiles {
		outPath := filepath.Join(outputDir, filepath.Base(f))
		fmt.Printf("[%d/%d] %s\n", i+1, len(imageFiles), filepath.Base(f))
		if err := memeGenerator(f, outPath, topText, bottomText, fontSize, fontPath,
			colorHex, strokeColorHex, strokeWidth, shadow, padding); err != nil {
			fmt.Printf("Error: %v\n", err)
		}
	}
	return nil
}

func main() {
	var (
		input      string
		output     string
		top        string
		bottom     string
		fontSize   float64
		fontPath   string
		color      string
		stroke     string
		strokeW    float64
		shadow     bool
		padding    float64
		batchDir   string
	)
	flag.StringVar(&input, "input", "", "Input image or dir (with --batch)")
	flag.StringVar(&output, "o", "meme.png", "Output file")
	flag.StringVar(&top, "t", "", "Top text")
	flag.StringVar(&bottom, "b", "", "Bottom text")
	flag.Float64Var(&fontSize, "s", 48, "Font size")
	flag.StringVar(&fontPath, "f", "", "Font path")
	flag.StringVar(&color, "c", "#FFFFFF", "Text color")
	flag.StringVar(&stroke, "stroke", "#000000", "Stroke color")
	flag.Float64Var(&strokeW, "stroke-width", 2, "Stroke width")
	flag.BoolVar(&shadow, "shadow", false, "Add shadow")
	flag.Float64Var(&padding, "padding", 20, "Padding")
	flag.StringVar(&batchDir, "batch", "", "Batch processing output dir")
	flag.Parse()

	if flag.NArg() > 0 {
		input = flag.Arg(0)
	}
	if input == "" {
		fmt.Println("Укажите входной файл или папку.")
		flag.Usage()
		os.Exit(1)
	}

	if batchDir != "" {
		if err := batchProcess(input, batchDir, top, bottom, fontSize, fontPath,
			color, stroke, strokeW, shadow, padding); err != nil {
			fmt.Println("Ошибка:", err)
			os.Exit(1)
		}
		fmt.Println("✅ Batch processing completed. Results in", batchDir)
	} else {
		if err := memeGenerator(input, output, top, bottom, fontSize, fontPath,
			color, stroke, strokeW, shadow, padding); err != nil {
			fmt.Println("Ошибка:", err)
			os.Exit(1)
		}
		fmt.Println("✅ Meme saved to", output)
	}
}

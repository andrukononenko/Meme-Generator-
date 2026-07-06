// meme_generator.cs
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.IO;
using System.Linq;

class MemeGenerator
{
    static void DrawTextWithOutline(Graphics g, string text, Font font, PointF point,
                                    Color color, Color outlineColor, float outlineWidth)
    {
        using (var pen = new Pen(outlineColor, outlineWidth) { LineJoin = LineJoin.Round })
        using (var path = new GraphicsPath())
        {
            path.AddString(text, font.FontFamily, (int)font.Style, font.Size, point, StringFormat.GenericTypographic);
            g.DrawPath(pen, path);
            g.FillPath(new SolidBrush(color), path);
        }
    }

    static void MemeGenerator(string inputPath, string outputPath, string topText, string bottomText,
                              float fontSize, string fontPath, string colorHex, string strokeHex,
                              float strokeWidth, bool shadow, int padding)
    {
        using (var img = Image.FromFile(inputPath))
        using (var g = Graphics.FromImage(img))
        {
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.TextRenderingHint = TextRenderingHint.AntiAlias;

            var color = ColorTranslator.FromHtml(colorHex);
            var stroke = ColorTranslator.FromHtml(strokeHex);

            Font font;
            if (!string.IsNullOrEmpty(fontPath) && File.Exists(fontPath))
            {
                var privateFont = new PrivateFontCollection();
                privateFont.AddFontFile(fontPath);
                font = new Font(privateFont.Families[0], fontSize, FontStyle.Bold);
            }
            else
            {
                font = new Font("Impact", fontSize, FontStyle.Bold);
            }

            int w = img.Width, h = img.Height;

            // Верхний текст
            if (!string.IsNullOrEmpty(topText))
            {
                var point = new PointF(w / 2f, padding);
                var format = new StringFormat { Alignment = StringAlignment.Center, LineAlignment = StringAlignment.Near };
                DrawTextWithOutline(g, topText, font, point, color, stroke, strokeWidth);
            }

            // Нижний текст
            if (!string.IsNullOrEmpty(bottomText))
            {
                var point = new PointF(w / 2f, h - padding);
                var format = new StringFormat { Alignment = StringAlignment.Center, LineAlignment = StringAlignment.Far };
                DrawTextWithOutline(g, bottomText, font, point, color, stroke, strokeWidth);
            }

            img.Save(outputPath, System.Drawing.Imaging.ImageFormat.Jpeg);
        }
    }

    static void BatchProcess(string inputDir, string outputDir, string topText, string bottomText,
                             float fontSize, string fontPath, string colorHex, string strokeHex,
                             float strokeWidth, bool shadow, int padding)
    {
        Directory.CreateDirectory(outputDir);
        var exts = new HashSet<string> { ".png", ".jpg", ".jpeg", ".bmp", ".tiff", ".webp" };
        var files = Directory.GetFiles(inputDir)
            .Where(f => exts.Contains(Path.GetExtension(f).ToLower()))
            .ToList();
        for (int i = 0; i < files.Count; i++)
        {
            string outPath = Path.Combine(outputDir, Path.GetFileName(files[i]));
            Console.WriteLine($"[{i+1}/{files.Count}] {Path.GetFileName(files[i])}");
            MemeGenerator(files[i], outPath, topText, bottomText, fontSize, fontPath,
                          colorHex, strokeHex, strokeWidth, shadow, padding);
        }
    }

    static void Main(string[] args)
    {
        string input = null, output = "meme.jpg";
        string topText = "", bottomText = "";
        float fontSize = 48;
        string fontPath = null, colorHex = "#FFFFFF", strokeHex = "#000000";
        float strokeWidth = 2;
        bool shadow = false;
        int padding = 20;
        string batchDir = null;

        for (int i = 0; i < args.Length; i++)
        {
            if (args[i] == "-t" && i+1 < args.Length) topText = args[++i];
            else if (args[i] == "-b" && i+1 < args.Length) bottomText = args[++i];
            else if (args[i] == "-o" && i+1 < args.Length) output = args[++i];
            else if (args[i] == "-s" && i+1 < args.Length) fontSize = float.Parse(args[++i]);
            else if (args[i] == "-c" && i+1 < args.Length) colorHex = args[++i];
            else if (args[i] == "-f" && i+1 < args.Length) fontPath = args[++i];
            else if (args[i] == "--stroke" && i+1 < args.Length) strokeHex = args[++i];
            else if (args[i] == "--stroke-width" && i+1 < args.Length) strokeWidth = float.Parse(args[++i]);
            else if (args[i] == "--shadow") shadow = true;
            else if (args[i] == "--padding" && i+1 < args.Length) padding = int.Parse(args[++i]);
            else if (args[i] == "--batch" && i+1 < args.Length) batchDir = args[++i];
            else if (args[i] == "-h" || args[i] == "--help")
            {
                Console.WriteLine("Usage: meme_generator <input> [options]\n  -t <text>    Top text\n  -b <text>    Bottom text\n  -o <file>    Output\n  -s <N>       Font size\n  -c <hex>     Color\n  -f <file>    Font path\n  --stroke <hex>  Stroke color\n  --stroke-width <N>  Stroke width\n  --shadow     Add shadow\n  --padding <N>  Padding\n  --batch <dir>  Batch");
                return;
            }
            else if (input == null) input = args[i];
        }
        if (input == null) { Console.Error.WriteLine("Укажите входной файл или папку."); return; }

        if (batchDir != null)
        {
            BatchProcess(input, batchDir, topText, bottomText, fontSize, fontPath,
                         colorHex, strokeHex, strokeWidth, shadow, padding);
            Console.WriteLine($"✅ Batch processing completed. Results in {batchDir}");
        }
        else
        {
            MemeGenerator(input, output, topText, bottomText, fontSize, fontPath,
                          colorHex, strokeHex, strokeWidth, shadow, padding);
            Console.WriteLine($"✅ Meme saved to {output}");
        }
    }
}

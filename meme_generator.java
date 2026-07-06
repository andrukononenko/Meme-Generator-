// meme_generator.java
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import java.nio.file.*;
import java.util.*;
import javax.imageio.*;

public class meme_generator {
    public static void main(String[] args) throws Exception {
        String input = null, output = "meme.jpg";
        String topText = "", bottomText = "";
        int fontSize = 48;
        String fontPath = null, colorHex = "#FFFFFF", strokeHex = "#000000";
        float strokeWidth = 2;
        boolean shadow = false;
        int padding = 20;
        String batchDir = null;

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-t") && i+1 < args.length) topText = args[++i];
            else if (args[i].equals("-b") && i+1 < args.length) bottomText = args[++i];
            else if (args[i].equals("-o") && i+1 < args.length) output = args[++i];
            else if (args[i].equals("-s") && i+1 < args.length) fontSize = Integer.parseInt(args[++i]);
            else if (args[i].equals("-c") && i+1 < args.length) colorHex = args[++i];
            else if (args[i].equals("-f") && i+1 < args.length) fontPath = args[++i];
            else if (args[i].equals("--stroke") && i+1 < args.length) strokeHex = args[++i];
            else if (args[i].equals("--stroke-width") && i+1 < args.length) strokeWidth = Float.parseFloat(args[++i]);
            else if (args[i].equals("--shadow")) shadow = true;
            else if (args[i].equals("--padding") && i+1 < args.length) padding = Integer.parseInt(args[++i]);
            else if (args[i].equals("--batch") && i+1 < args.length) batchDir = args[++i];
            else if (args[i].equals("-h") || args[i].equals("--help")) {
                System.out.println("Usage: meme_generator <input> [options]\n  -t <text>    Top text\n  -b <text>    Bottom text\n  -o <file>    Output\n  -s <N>       Font size\n  -c <hex>     Color\n  -f <file>    Font path\n  --stroke <hex>  Stroke color\n  --stroke-width <N>  Stroke width\n  --shadow     Add shadow\n  --padding <N>  Padding\n  --batch <dir>  Batch");
                return;
            } else if (input == null) input = args[i];
        }
        if (input == null) { System.err.println("Укажите входной файл или папку."); System.exit(1); }

        if (batchDir != null) {
            batchProcess(input, batchDir, topText, bottomText, fontSize, fontPath,
                         colorHex, strokeHex, strokeWidth, shadow, padding);
            System.out.println("✅ Batch processing completed. Results in " + batchDir);
        } else {
            memeGenerator(input, output, topText, bottomText, fontSize, fontPath,
                          colorHex, strokeHex, strokeWidth, shadow, padding);
            System.out.println("✅ Meme saved to " + output);
        }
    }

    static void memeGenerator(String inputPath, String outputPath, String topText, String bottomText,
                              int fontSize, String fontPath, String colorHex, String strokeHex,
                              float strokeWidth, boolean shadow, int padding) throws Exception {
        BufferedImage img = ImageIO.read(new File(inputPath));
        int w = img.getWidth(), h = img.getHeight();

        Graphics2D g = img.createGraphics();
        g.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
        g.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);

        Color color = Color.decode(colorHex);
        Color stroke = Color.decode(strokeHex);

        Font font;
        if (fontPath != null && new File(fontPath).exists()) {
            font = Font.createFont(Font.TRUETYPE_FONT, new File(fontPath)).deriveFont(Font.BOLD, fontSize);
        } else {
            font = new Font("Impact", Font.BOLD, fontSize);
        }

        // Верхний текст
        if (topText != null && !topText.isEmpty()) {
            g.setFont(font);
            g.setColor(color);
            FontMetrics fm = g.getFontMetrics();
            int x = (w - fm.stringWidth(topText)) / 2;
            int y = padding + fm.getAscent();
            // Обводка
            g.setColor(stroke);
            g.setStroke(new BasicStroke(strokeWidth));
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 || dy != 0)
                        g.drawString(topText, x + dx, y + dy);
                }
            }
            g.setColor(color);
            g.drawString(topText, x, y);
        }

        // Нижний текст
        if (bottomText != null && !bottomText.isEmpty()) {
            g.setFont(font);
            g.setColor(color);
            FontMetrics fm = g.getFontMetrics();
            int x = (w - fm.stringWidth(bottomText)) / 2;
            int y = h - padding - fm.getDescent();
            g.setColor(stroke);
            g.setStroke(new BasicStroke(strokeWidth));
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx != 0 || dy != 0)
                        g.drawString(bottomText, x + dx, y + dy);
                }
            }
            g.setColor(color);
            g.drawString(bottomText, x, y);
        }

        g.dispose();
        ImageIO.write(img, "jpg", new File(outputPath));
    }

    static void batchProcess(String inputDir, String outputDir, String topText, String bottomText,
                             int fontSize, String fontPath, String colorHex, String strokeHex,
                             float strokeWidth, boolean shadow, int padding) throws Exception {
        Files.createDirectories(Paths.get(outputDir));
        String[] exts = {".png", ".jpg", ".jpeg", ".bmp", ".tiff", ".webp"};
        Set<String> extSet = new HashSet<>(Arrays.asList(exts));
        File[] files = new File(inputDir).listFiles();
        if (files == null) return;
        int total = 0;
        java.util.List<File> imageFiles = new ArrayList<>();
        for (File f : files) {
            if (f.isFile() && extSet.contains(f.getName().substring(f.getName().lastIndexOf('.')).toLowerCase())) {
                imageFiles.add(f);
            }
        }
        total = imageFiles.size();
        for (int i = 0; i < total; i++) {
            File f = imageFiles.get(i);
            String outPath = outputDir + File.separator + f.getName();
            System.out.println("[" + (i+1) + "/" + total + "] " + f.getName());
            memeGenerator(f.getPath(), outPath, topText, bottomText, fontSize, fontPath,
                          colorHex, strokeHex, strokeWidth, shadow, padding);
        }
    }
}

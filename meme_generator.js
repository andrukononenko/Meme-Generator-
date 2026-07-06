// meme_generator.js
#!/usr/bin/env node
'use strict';

const { createCanvas, loadImage, registerFont } = require('canvas');
const fs = require('fs');
const path = require('path');

function parseHexColor(hex) {
    if (hex.length === 7 && hex[0] === '#') {
        const r = parseInt(hex.substring(1, 3), 16);
        const g = parseInt(hex.substring(3, 5), 16);
        const b = parseInt(hex.substring(5, 7), 16);
        return `rgb(${r},${g},${b})`;
    }
    return 'rgb(255,255,255)';
}

async function memeGenerator(inputPath, outputPath, topText, bottomText,
                            fontSize, fontPath, color, strokeColor, strokeWidth, shadow, padding) {
    const img = await loadImage(inputPath);
    const w = img.width, h = img.height;
    const canvas = createCanvas(w, h);
    const ctx = canvas.getContext('2d');
    ctx.drawImage(img, 0, 0);

    if (fontPath) {
        registerFont(fontPath, { family: 'Custom' });
        ctx.font = `bold ${fontSize}px Custom`;
    } else {
        ctx.font = `bold ${fontSize}px Impact, sans-serif`;
    }
    ctx.textAlign = 'center';
    ctx.textBaseline = 'top';
    ctx.fillStyle = parseHexColor(color);
    ctx.strokeStyle = parseHexColor(strokeColor);
    ctx.lineWidth = strokeWidth;

    // Верхний текст
    if (topText) {
        const x = w / 2;
        const y = padding;
        ctx.strokeText(topText, x, y);
        ctx.fillText(topText, x, y);
    }

    // Нижний текст
    if (bottomText) {
        ctx.textBaseline = 'bottom';
        const x = w / 2;
        const y = h - padding;
        ctx.strokeText(bottomText, x, y);
        ctx.fillText(bottomText, x, y);
    }

    const out = fs.createWriteStream(outputPath);
    const stream = canvas.createPNGStream();
    stream.pipe(out);
    return new Promise((resolve, reject) => {
        out.on('finish', resolve);
        out.on('error', reject);
    });
}

async function batchProcess(inputDir, outputDir, opts) {
    if (!fs.existsSync(outputDir)) fs.mkdirSync(outputDir, { recursive: true });
    const exts = ['.png', '.jpg', '.jpeg', '.bmp', '.tiff', '.webp'];
    const files = fs.readdirSync(inputDir).filter(f => exts.includes(path.extname(f).toLowerCase()));
    for (let i = 0; i < files.length; i++) {
        const inPath = path.join(inputDir, files[i]);
        const outPath = path.join(outputDir, files[i]);
        console.log(`[${i+1}/${files.length}] ${files[i]}`);
        await memeGenerator(inPath, outPath, opts.top, opts.bottom, opts.fontSize,
                            opts.fontPath, opts.color, opts.stroke, opts.strokeWidth,
                            opts.shadow, opts.padding);
    }
}

async function main() {
    const args = process.argv.slice(2);
    let input, output = 'meme.png', top = '', bottom = '';
    let fontSize = 48, fontPath = null, color = '#FFFFFF', stroke = '#000000';
    let strokeWidth = 2, shadow = false, padding = 20, batchDir = null;

    for (let i = 0; i < args.length; i++) {
        if (args[i] === '-t' && i+1 < args.length) top = args[++i];
        else if (args[i] === '-b' && i+1 < args.length) bottom = args[++i];
        else if (args[i] === '-o' && i+1 < args.length) output = args[++i];
        else if (args[i] === '-s' && i+1 < args.length) fontSize = parseInt(args[++i], 10);
        else if (args[i] === '-c' && i+1 < args.length) color = args[++i];
        else if (args[i] === '-f' && i+1 < args.length) fontPath = args[++i];
        else if (args[i] === '--stroke' && i+1 < args.length) stroke = args[++i];
        else if (args[i] === '--stroke-width' && i+1 < args.length) strokeWidth = parseInt(args[++i], 10);
        else if (args[i] === '--shadow') shadow = true;
        else if (args[i] === '--padding' && i+1 < args.length) padding = parseInt(args[++i], 10);
        else if (args[i] === '--batch' && i+1 < args.length) batchDir = args[++i];
        else if (args[i] === '-h' || args[i] === '--help') {
            console.log(`Usage: node meme_generator.js <input> [options]
  -t <text>    Top text
  -b <text>    Bottom text
  -o <file>    Output
  -s <N>       Font size
  -c <hex>     Color
  -f <file>    Font path
  --stroke <hex>  Stroke color
  --stroke-width <N>  Stroke width
  --shadow     Add shadow
  --padding <N>  Padding
  --batch <dir>  Batch`);
            process.exit(0);
        } else if (!input) input = args[i];
    }
    if (!input) { console.error('Укажите входной файл или папку.'); process.exit(1); }

    const opts = { top, bottom, fontSize, fontPath, color, stroke, strokeWidth, shadow, padding };

    if (batchDir) {
        await batchProcess(input, batchDir, opts);
        console.log(`✅ Batch processing completed. Results in ${batchDir}`);
    } else {
        await memeGenerator(input, output, top, bottom, fontSize, fontPath, color, stroke, strokeWidth, shadow, padding);
        console.log(`✅ Meme saved to ${output}`);
    }
}

main().catch(console.error);

# meme_generator.py
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import argparse
from PIL import Image, ImageDraw, ImageFont, ImageColor

def parse_hex_color(color):
    if color.startswith('#'):
        return ImageColor.getrgb(color)
    return ImageColor.getrgb(color)

def draw_text_with_outline(draw, text, font, xy, fill, stroke_fill, stroke_width=2):
    # Рисуем обводку
    x, y = xy
    for dx in range(-stroke_width, stroke_width+1):
        for dy in range(-stroke_width, stroke_width+1):
            if dx != 0 or dy != 0:
                draw.text((x+dx, y+dy), text, font=font, fill=stroke_fill)
    draw.text((x, y), text, font=font, fill=fill)

def meme_generator(input_path, output_path, top_text='', bottom_text='',
                   font_size=48, font_path=None, color='#FFFFFF',
                   stroke_color='#000000', stroke_width=2,
                   shadow=False, shadow_offset=(2,2), padding=20):
    img = Image.open(input_path).convert('RGB')
    draw = ImageDraw.Draw(img)
    w, h = img.size

    # Шрифт
    try:
        font = ImageFont.truetype(font_path, font_size) if font_path else ImageFont.load_default()
    except:
        font = ImageFont.load_default()

    color_rgb = parse_hex_color(color)
    stroke_rgb = parse_hex_color(stroke_color)

    # Верхний текст
    if top_text:
        # Ограничиваем ширину текста
        max_width = w - 2 * padding
        # Простая обрезка, если текст слишком длинный
        # Для упрощения используем стандартный метод
        draw_text_with_outline(draw, top_text, font, (w//2, padding), color_rgb, stroke_rgb, stroke_width)

    # Нижний текст
    if bottom_text:
        bbox = draw.textbbox((0,0), bottom_text, font=font)
        text_h = bbox[3] - bbox[1]
        y = h - padding - text_h
        draw_text_with_outline(draw, bottom_text, font, (w//2, y), color_rgb, stroke_rgb, stroke_width)

    img.save(output_path, quality=95)

def batch_process(input_dir, output_dir, **kwargs):
    os.makedirs(output_dir, exist_ok=True)
    exts = ('.png', '.jpg', '.jpeg', '.bmp', '.tiff', '.webp')
    files = [f for f in os.listdir(input_dir) if f.lower().endswith(exts)]
    total = len(files)
    for i, f in enumerate(files):
        in_path = os.path.join(input_dir, f)
        out_path = os.path.join(output_dir, f)
        if kwargs.get('verbose'):
            print(f"[{i+1}/{total}] {f}")
        meme_generator(in_path, out_path, **kwargs)

def main():
    parser = argparse.ArgumentParser(description="Meme Generator")
    parser.add_argument('input', help='Входное изображение или папка (с -b)')
    parser.add_argument('-t', '--top', default='', help='Верхний текст')
    parser.add_argument('-b', '--bottom', default='', help='Нижний текст')
    parser.add_argument('-o', '--output', default='meme.jpg', help='Выходной файл')
    parser.add_argument('-s', '--size', type=int, default=48, help='Размер шрифта')
    parser.add_argument('-c', '--color', default='#FFFFFF', help='Цвет текста (HEX)')
    parser.add_argument('-f', '--font', help='Путь к TTF-шрифту')
    parser.add_argument('--stroke', default='#000000', help='Цвет обводки')
    parser.add_argument('--stroke-width', type=int, default=2, help='Ширина обводки')
    parser.add_argument('--shadow', action='store_true', help='Добавить тень')
    parser.add_argument('--shadow-offset', default='2,2', help='Смещение тени (X,Y)')
    parser.add_argument('--padding', type=int, default=20, help='Отступ от краёв')
    parser.add_argument('--batch', help='Пакетная обработка (выходная папка)')
    parser.add_argument('-v', '--verbose', action='store_true', help='Подробный вывод')
    args = parser.parse_args()

    kwargs = {
        'top_text': args.top,
        'bottom_text': args.bottom,
        'font_size': args.size,
        'font_path': args.font,
        'color': args.color,
        'stroke_color': args.stroke,
        'stroke_width': args.stroke_width,
        'shadow': args.shadow,
        'shadow_offset': tuple(map(int, args.shadow_offset.split(','))),
        'padding': args.padding,
        'verbose': args.verbose,
    }

    if args.batch:
        batch_process(args.input, args.batch, **kwargs)
        print(f"✅ Пакетная обработка завершена. Результаты в {args.batch}")
    else:
        output = args.output or 'meme.jpg'
        meme_generator(args.input, output, **kwargs)
        print(f"✅ Мем сохранён в {output}")

if __name__ == '__main__':
    main()

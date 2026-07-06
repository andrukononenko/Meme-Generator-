#!/usr/bin/env ruby
# meme_generator.rb
# encoding: UTF-8

require 'rmagick'
include Magick
require 'optparse'
require 'fileutils'

def parse_hex_color(hex)
  if hex =~ /^#([0-9a-f]{6})$/i
    rgb = hex[1..-1].scan(/../).map { |c| c.to_i(16) }
    return "rgb(#{rgb[0]},#{rgb[1]},#{rgb[2]})"
  end
  'rgb(255,255,255)'
end

def meme_generator(input_path, output_path, top_text, bottom_text,
                   font_size, font_path, color, stroke_color, stroke_width, shadow, padding)
  img = Image.read(input_path).first
  w, h = img.columns, img.rows

  draw = Draw.new
  draw.font = font_path if font_path && File.exist?(font_path)
  draw.pointsize = font_size
  draw.fill = parse_hex_color(color)
  draw.stroke = parse_hex_color(stroke_color)
  draw.stroke_width = stroke_width
  draw.gravity = NorthGravity
  draw.annotate(img, 0, 0, 0, padding, top_text) if top_text && !top_text.empty?

  draw.gravity = SouthGravity
  draw.annotate(img, 0, 0, 0, padding, bottom_text) if bottom_text && !bottom_text.empty?

  img.write(output_path)
end

def batch_process(input_dir, output_dir, opts)
  FileUtils.mkdir_p(output_dir)
  exts = %w[.png .jpg .jpeg .bmp .tiff .webp]
  files = Dir.entries(input_dir)
             .select { |f| exts.include?(File.extname(f).downcase) }
             .map { |f| File.join(input_dir, f) }
  files.each_with_index do |f, i|
    out_path = File.join(output_dir, File.basename(f))
    puts "[#{i+1}/#{files.size}] #{File.basename(f)}"
    meme_generator(f, out_path, opts[:top], opts[:bottom], opts[:font_size],
                   opts[:font_path], opts[:color], opts[:stroke], opts[:stroke_width],
                   opts[:shadow], opts[:padding])
  end
end

def main
  options = { top: '', bottom: '', output: 'meme.jpg', font_size: 48,
              font_path: nil, color: '#FFFFFF', stroke: '#000000',
              stroke_width: 2, shadow: false, padding: 20, batch: nil }
  input = nil

  OptionParser.new do |opts|
    opts.banner = "Usage: meme_generator.rb <input> [options]"
    opts.on('-t TEXT', 'Top text') { |v| options[:top] = v }
    opts.on('-b TEXT', 'Bottom text') { |v| options[:bottom] = v }
    opts.on('-o FILE', 'Output') { |v| options[:output] = v }
    opts.on('-s N', Integer, 'Font size') { |v| options[:font_size] = v }
    opts.on('-c HEX', 'Color') { |v| options[:color] = v }
    opts.on('-f FILE', 'Font path') { |v| options[:font_path] = v }
    opts.on('--stroke HEX', 'Stroke color') { |v| options[:stroke] = v }
    opts.on('--stroke-width N', Integer, 'Stroke width') { |v| options[:stroke_width] = v }
    opts.on('--shadow', 'Add shadow') { options[:shadow] = true }
    opts.on('--padding N', Integer, 'Padding') { |v| options[:padding] = v }
    opts.on('--batch DIR', 'Batch output dir') { |v| options[:batch] = v }
    opts.on('-h', 'Help') { puts opts; exit }
  end.parse!

  input = ARGV[0] if ARGV[0]
  unless input
    puts "Укажите входной файл или папку."
    exit 1
  end

  if options[:batch]
    batch_process(input, options[:batch], options)
    puts "✅ Batch processing completed. Results in #{options[:batch]}"
  else
    meme_generator(input, options[:output], options[:top], options[:bottom],
                   options[:font_size], options[:font_path], options[:color],
                   options[:stroke], options[:stroke_width], options[:shadow],
                   options[:padding])
    puts "✅ Meme saved to #{options[:output]}"
  end
end

main if __FILE__ == $0

# encoding: utf-8

require 'pp'

module ReVIEW
  class MARKDOWNBuilder

    def builder_init_file
      @ul_indent = 0
    end

    def ul_begin
      blank if @ul_indent == 0
      @ul_indent += 1
    end

    def ul_item(lines)
      puts "  " * (@ul_indent - 1) + "* " + "#{lines.join}"
    end

    def ul_item_begin(lines)
      puts "  " * (@ul_indent - 1) + "* " + "#{lines.join}"
    end

    def ul_item_end
    end

    def ul_end
      @ul_indent -= 1
      blank if @ul_indent == 0
    end

    def cmd(lines)
      puts "```"
      lines.each do |line|
        puts detab(line)
      end
      puts "```"
    end

    def table(lines, id = nil, caption = nil)
      rows = []
      sepidx = nil
      lines.each_with_index do |line, idx|
        if /\A[\=\-]{12}/ =~ line
          # just ignore
          #error "too many table separator" if sepidx
          sepidx ||= idx
          next
        end
        rows.push line.strip.split(/\t+/).map {|s| s.sub(/\A\./, '') }
      end
      rows = adjust_n_cols(rows)

      begin
        table_header id, caption unless caption.nil?
      rescue KeyError
        error "no such table: #{id}"
      end
      table_begin rows.first.size
      return if rows.empty?
      if sepidx
        sepidx.times do
          tr rows.shift.map {|s| th(s) }
        end
        table_border rows.first.size
        rows.each do |cols|
          tr cols.map {|s| td(s) }
        end
      else
        rows.each do |cols|
          h, *cs = *cols
          tr [th(h)] + cs.map {|s| td(s) }
        end
      end
      table_end
    end

    def table_header(id, caption)
      if get_chap.nil?
        puts %Q[#{I18n.t("table")}#{I18n.t("format_number_header_without_chapter", [@chapter.table(id).number])}#{I18n.t("caption_prefix")}#{compile_inline(caption)}]
      else
        puts %Q[#{I18n.t("table")}#{I18n.t("format_number_header", [get_chap, @chapter.table(id).number])}#{I18n.t("caption_prefix")}#{compile_inline(caption)}]
      end
      blank
    end

    def table_begin(ncols)
    end

    def tr(rows)
      puts "|#{rows.join("|")}|"
    end

    def table_border(ncols)
      puts (0..ncols).map{"|"}.join(":--")
    end

    def th(str)
      "#{str}"
    end

    def td(str)
      "#{str}"
    end

    def table_end
      blank
    end

    def inline_br(str)
      "\n"
    end
  end
end

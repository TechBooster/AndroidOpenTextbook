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
  end
end

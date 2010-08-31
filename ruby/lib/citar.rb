# Binding for the citar part of speech tagger.
#
# Author::    Daniel de Kok (mailto:me@danieldk.eu)
# Copyright:: Copyright 2010 Daniel de Kok
# License::   Distributed under the GNU Lesser General Public License version 3

require 'ffi'

module CitarLibC
  extend FFI::Library
  ffi_lib FFI::Library::LIBC

  attach_function :free, [:pointer], :void
end

# This module provides bindings for the citar part of speech tagger.

module Citar
  extend FFI::Library
  ffi_lib 'citar'

  attach_function :citar_tagger_new, [:string, :string],
    :pointer
  attach_function :citar_tagger_free, [:pointer], :void
  attach_function :citar_tagger_tag, [:pointer, :pointer, :int], :pointer

  class TaggerException < Exception
  end

  # This class embeds a citar tagger instance.

  class Tagger

    # Construct a tagger, using the lexicon and ngram file names.

    def initialize(lexicon, ngrams)
      @tagger = Citar::citar_tagger_new(lexicon, ngrams)

      if @tagger.null?
        raise TaggerException, "Could construct tagger from model."
      end

      ObjectSpace.define_finalizer(self, self.class.finalize(@tagger))
    end

    # Tag a sentence, represented as a list of words.

    def tag(words)
      # Convert array of words to char**
      wordPtrs = words.map { |e|
        FFI::MemoryPointer.from_string(e)
      }
      wordArr = FFI::MemoryPointer.new(:pointer, words.length).write_array_of_pointer(wordPtrs)

      # Tag it!
      tagArr = Citar::citar_tagger_tag(@tagger, wordArr, words.length)

      # Convert char** to Ruby array of strings
      tagPtrs = tagArr.read_array_of_pointer(words.length)
      tags = tagPtrs.map { |p|
        p.read_string
      }

      # Free memory allocated by citar_tagger_tag
      tagPtrs.each { |p|
        CitarLibC::free(p)
      }
      CitarLibC::free(tagArr)

      tags
    end

    def self.finalize(tagger)
      proc { Citar::citar_tagger_free(tagger) }
    end
  end
end

# mp4dump

This is a little test utility used to parse and dump structure of data files based on [ISO Base Media Format](http://fileformats.archiveteam.org/wiki/ISO_Base_Media_File_Format)  (BMF) e.g. MP4, 3GP etc. It is heavily incomplete and by itself does not make a lot of sense. It is just a little test project.

The original task did sound like this:
1. Download file from given URL
2. Iterate through it, find out all boxes and print them
3. For Media Data MDAT box analyze its content and extract found information (actually images)

A link on a sample MP4 file to play with was given. Quick analysys shown that it is a piece of MP4 with [SMPTE-TT](https://www.speechpad.com/captions/smpte-tt) XML document inside. This document in turn stores several PNG images with Closed Captions. mp4dump utility was written with all that stuff in mind in a sense that it processes boxes mostly found this sample file. Though nobody stops one to add more parsers for another types of boxes. Up to the full BMP/MP4 compliance.

### Sample output

For original reference MP4 file output should look like this:

```
$ ./mp4dump https://github.com/ianzag/mp4dump/raw/main/data/test.mp4
Start to fetch data stream from 'https://github.com/ianzag/mp4dump/raw/main/data/test.mp4'
Stream {
  Box type: 'Movie Fragment' size: 181 {
    Box type: 'Movie Fragment Header' size: 16 sequenceNumber: 1041331
    Box type: 'Track Fragment' size: 157 {
      Box type: 'Track Fragment Header' size: 24 tfFlags: 0x18 trackId: 1 defaultSampleDuration: 20000000 defaultSampleSize: 17900
      Box type: 'Track Fragment Run' size: 20 trFlags: 0x1 sampleCount: 1 dataOffset: 128
      Box type: 'User Extension' size: 44 uuid: 6d1d9b05:42d5:44e6:80e2:141daff757b2 dataSize: 20
      Box type: 'User Extension' size: 61 uuid: d4807ef2:ca39:4695:8e54:26cb9e46a79f dataSize: 37
    }
  }
  Box type: 'Movie Data' size: 17908 images: {type: png size: 3888 file: image1.png}, {type: png size: 4174 file: image2.png}, {type: png size: 4174 file: image3.png}
}
```

### Dependencies

1. [CURL](https://curl.se/libcurl/) is used to fetch remote file over HTTP
2. [Expat](https://libexpat.github.io/) is used to parse XML documents and locate images

### TODO

1. Add more parsers for different types of boxes
2. Handle large files (especially remote)

Current implementation downloads remote file completely and parses it. While this approach makes parser neat and simple, it is impractical for large files. For example a real MP4 media file with some movie inside can be very large (gigabytes). On the other hand, the vast majority of this file is used by media data. Structural elements require relatively little space. For local files parser can be easily rewritten from stream to random access. For remote files accessible over HTTP trick with **HTTP byte-range** reading. In this case we can simulate a random access to remote file and *greatly* speedup file processing. No need to download it completely anymore.

*Have fun!*

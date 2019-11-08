# archiver

Program for data compression and decompression implemented in C++

## Examples of usage

```
make
./huffman -c -f FILE_TO_COMRESS -o COMPRESSED_FILE
./huffman -u -f COMPRESSED_FILE -o DECOMPRESSED_FILE
```

## Output description

### Compessing

```
$ ./huffman -c -f FILE_TO_COMRESS -o COMPRESSED_FILE
> size of FILE_TO_COMPRESS  in bytes
> size of COMPRESSED_FILE   in bytes
```

### Decompressing

```
$ ./huffman -u -f COMPRESSED_FILE -o DECOMPRESSED_FILE
> size of COMPRESSED_FILE   in bytes
> size of DECOMPRESSED_FILE in bytes
```

# Prerequirement 
This program needs imagemagick's `convert` command.

# Usage

```sh
[Usage] ./debug.out [ -n <number of clusters> ] [ -i <input file> ] [ -o <output file> ]
  -h                      : show usage
  -n <number of clusters> : set number of cluster ; default = 8
  -i <input file>         : set input file name   ; default = input.ppm
  -o <output file>        : set output file name  ; default = output.ppm
```

# How to use

```sh
convert input.png input.ppm
./debug.out
convert output.ppm output.png
```

# Sample
## Input image
![入力画像](sample_images/input.jpg '入力画像')

[Original Photo](https://pixabay.com/ja/photos/%E3%83%AA%E3%82%B9%E3%83%9C%E3%83%B3-%E3%83%9D%E3%83%AB%E3%83%88%E3%82%AC%E3%83%AB-%E8%B5%A4-4379656/)

## Output images
### N = 2
![出力画像(N=2)](sample_images/output_2.jpg '出力画像(N=2)')

### N = 4
![出力画像(N=4)](sample_images/output_4.jpg '出力画像(N=4)')

### N = 8
![出力画像(N=8)](sample_images/output_8.jpg '出力画像(N=8)')

### N = 16
![出力画像(N=16)](sample_images/output_16.jpg '出力画像(N=16)')

### N = 24
![出力画像(N=24)](sample_images/output_24.jpg '出力画像(N=24)')

### N = 32
![出力画像(N=32)](sample_images/output_32.jpg '出力画像(N=32)')

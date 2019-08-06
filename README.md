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

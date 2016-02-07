#!/bin/bash
ffmpeg -r 20 -f image2 -i '%04d.png' -c:v libvpx -b:v 1M movie.webm




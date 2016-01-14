#!/bin/bash
avconv -r 5 -f image2 -i '%04d.png' -b:v 4096k movie.avi




#!/usr/bin/env bash

docker build --platform linux/amd64 -t milestone2 .
docker run --platform linux/amd64 --rm -v $(pwd):/milestone2 -w /milestone2 milestone2

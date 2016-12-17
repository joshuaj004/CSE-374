#!/bin/bash

if [ $# -lt 1 ]; then
    echo "This script needs a URL" >&2
    exit 1
fi

tmpfile=$(mktemp)
wget --quiet -O $tmpfile $1 > /dev/null

wc -c < $tmpfile

rm $tmpfile


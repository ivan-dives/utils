#!/bin/bash

find /var/db/paludis/repositories/*/packages/ -type f -name "*.exheres-0" -o -name "*.exlib" | while read name; do
    out=$(./main "$name" 2>/dev/null)
    if ! [[ -z $out ]]; then
    	echo "== $name:"
    	echo "$out"
    fi
done

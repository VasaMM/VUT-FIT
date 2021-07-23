#!/bin/bash

# Zastavim pri chybe
set -e


cd ./data

outDir="out/"
tmpDir="tmp/"
outExtension=".out"
outAdaptiveExtension=".adaptive.out"

# Vytvorim podslozky
mkdir -p $outDir
mkdir -p $tmpDir



# Iteruji pres vsechny RAWy
echo "STATIC:"
for file in *.raw; do
    [ -f "$file" ] || break
    echo -n $file

	../huff_codec -i "$file" -o "$outDir$file$outExtension" -q
	../huff_codec -i "$outDir$file$outExtension" -o "$tmpDir$file" -q -d
	diff "$file" "$tmpDir$file" -q
	echo " OK"
done


echo
echo "STATIC MODEL:"
for file in *.raw; do
    [ -f "$file" ] || break
    echo -n $file

	../huff_codec -i "$file" -o "$outDir$file$outExtension" -q -m
	../huff_codec -i "$outDir$file$outExtension" -o "$tmpDir$file" -q -d
	diff "$file" "$tmpDir$file" -q
	echo " OK"
done


echo
echo "ADAPTIVE:"
for file in *.raw; do
    [ -f "$file" ] || break
    echo -n $file

	../huff_codec -i "$file" -o "$outDir$file$outAdaptiveExtension" -q -h adaptive
	../huff_codec -i "$outDir$file$outAdaptiveExtension" -o "$tmpDir$file" -q -d
	diff "$file" "$tmpDir$file" -q
	echo " OK"
done


echo
echo "ADAPTIVE MODEL:"
for file in *.raw; do
    [ -f "$file" ] || break
    echo -n $file

	../huff_codec -i "$file" -o "$outDir$file$outAdaptiveExtension" -q -h adaptive -m
	../huff_codec -i "$outDir$file$outAdaptiveExtension" -o "$tmpDir$file" -q -d
	diff "$file" "$tmpDir$file" -q
	echo " OK"
done

rm -rf $outDir
rm -rf $tmpDir


cd ..
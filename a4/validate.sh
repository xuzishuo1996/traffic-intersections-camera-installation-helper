# rm previous output folder
echo --- remove my-output/
rm -r my-output/

# create output folder
echo --- create my-output/
mkdir my-output/

# create output folder
echo --- execute ece650-a4 on inputs from sample-input/ and output to my-output/
cd build/
./ece650-a4 < ../sample-input/simple.in > ../my-output/simple.out
./ece650-a4 < ../sample-input/my-graphs.in > ../my-output/my-graphs.out

# compare current output against sample-output
echo --- compare against sample-output
diff ../sample-output/simple.out ../my-output/simple.out
if [ $? -eq 0 ]; then
    echo simple.out match
else
    echo simple.out DO NOT match
fi

diff ../sample-output/my-graphs.out ../my-output/my-graphs.out
if [ $? -eq 0 ]; then
    echo my-graphs.out match
else
    echo my-graphs.out DO NOT match
fi
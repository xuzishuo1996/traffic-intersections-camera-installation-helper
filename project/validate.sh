# rm previous output folder
echo --- remove my-output/
rm -r my-output/

# create output folder
echo --- create my-output/
mkdir my-output/

# create output folder
echo --- execute ece650-prj on inputs from sample-input/ and output to my-output/
build/ece650-prj < ./sample-input/instruction.in > ./my-output/instruction.out
build/ece650-prj < ./sample-input/simple.in > ./my-output/simple.out
build/ece650-prj < ./sample-input/my-graphs.in > ./my-output/my-graphs.out

# compare current output against sample-output
echo --- compare against sample-output

diff ./sample-output/instruction.out ./my-output/instruction.out
if [ $? -eq 0 ]; then
    echo instruction.out match
else
    echo instruction.out DO NOT match
fi

diff ./sample-output/simple.out ./my-output/simple.out
if [ $? -eq 0 ]; then
    echo simple.out match
else
    echo simple.out DO NOT match
fi

diff ./sample-output/my-graphs.out ./my-output/my-graphs.out
if [ $? -eq 0 ]; then
    echo my-graphs.out match
else
    echo my-graphs.out DO NOT match
fi
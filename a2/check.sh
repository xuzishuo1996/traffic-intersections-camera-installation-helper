echo --- Removing build/
rm -r build/

echo --- Build using cmake
mkdir build && cd build && cmake ../ && make

echo --- Test ece650-a2
cd build/
# overwrite rather than append
./ece650-a2 < ../sample_input/input1.txt > ../my_output/my_output1.txt 2> ../my_error/my_error1.txt
cd ../

echo --- Comparing generated output against sample output
diff my_output/my_output1.txt sample_output/sample_output1.txt
if [ $? -eq 0 ]; then
    echo Outputs match
else
    echo Outputs DO NOT match
fi

diff my_error/my_error1.txt sample_error/sample_error1.txt
if [ $? -eq 0 ]; then
    echo Errors match
else
    echo Errors DO NOT match
fi
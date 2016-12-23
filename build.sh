echo -e "Compiling..."
make
echo -e "\nRunning..."
./nes_test
echo -e "\nComparing..."
python log_compare.py
echo -e "===== Compiling ====="
make clean
make
echo -e "\n===== Running ======="
./nes_test
echo -e "\n===== Comparing ====="
python log_compare.py

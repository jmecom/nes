echo -e "===== Compiling ====="
make
echo -e "\n===== Running ======="
./nes_test
echo -e "\n===== Comparing ====="
python log_compare.py

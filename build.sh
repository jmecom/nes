echo -e "===== Compiling ====="
make clean
make
if [[ $? -ne 0 ]]; then 
    exit 
fi

echo -e "\n===== Running ======="
./nes_test
if [[ $? -ne 0 ]]; then 
    exit 
fi

echo -e "\n===== Comparing ====="
python log_compare.py

echo -e "## Compiling ######"
make clean
make
if [[ $? -ne 0 ]]; then 
    exit 
fi

echo -e "\n## Running #######"
./nes_test

echo -e "\n## Comparing #####"
tail out.log
echo
python log_compare.py

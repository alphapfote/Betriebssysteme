#bash script to delete old myfs files and compile new one
#usage bash run.sh
rm *myfs
make -f Makefile

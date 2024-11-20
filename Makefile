HEADERS = decode.h encode.h dict.h common.h stack.h codec.h
OBJECTS = program.o decode.o encode.o common.o stack.o codec.o 

default: program codec_encode codec_decode

program.o: main.c $(HEADERS)
	gcc -c main.c -o program.o

decode.o: decode.c decode.h dict.h common.h
	gcc -c decode.c -o decode.o

encode.o: encode.c encode.h dict.h common.h
	gcc -c encode.c -o encode.o

common.o: common.c common.h
	gcc -c common.c -o common.o

program: $(OBJECTS)
	touch encode
	touch decode
	rm -f decode
	rm -f encode
	gcc $(OBJECTS) -o program
	ln program decode
	ln program encode

codec_encode: codec_encode.o common.o codec.o
	gcc codec_encode.o common.o codec.o -o codec_encode

codec_decode: codec_decode.o common.o codec.o
	gcc codec_decode.o common.o codec.o -o codec_decode

clean:
	rm -f $(OBJECTS)
	rm -f p -f program
	rm -f decode
	rm -f encode
	rm -f codec_encode.o
	rm -f codec_decode.o
	rm -f DBG.encode
	rm -f DBG.decode
	rm -f DBG.pruned_encode


three_spaces: 
	echo "   " | DBG=1 STAGE=1 ./encode | DBG=1 STAGE=1 ./decode


foo_1: 
	echo "Foo Bar Foo Bar Foo Bar foo bar foo bar Foo Bar Foobar" | STAGE=1 ./encode | STAGE=1 ./decode

foo_2: 
	echo "Foo Bar Foo Bar Foo Bar foo bar foo bar Foo Bar Foobar" | STAGE=2 ./encode | STAGE=2 ./decode

foo_3: 
	echo "Foo Bar Foo Bar Foo Bar foo bar foo bar Foo Bar Foobar" | STAGE=3 ./encode | STAGE=3 ./decode


baba:
	echo "BABA" | ./encode | ./decode


baba_1:
	echo "BABA" | STAGE=1 ./encode | STAGE=1 ./decode


baba_2:
	echo "BABA" | STAGE=2 ./encode -m 9 -p | STAGE=2 ./decode


baba_3:
	echo "BABA" | STAGE=3 ./encode -m 9 -p | STAGE=3 ./decode

hello_world:
	./encode < test_cases/hello_world.txt | ./decode | cmp - test_cases/hello_world.txt


numbers: 
	./encode < test_cases/numbers.txt | ./decode | cmp - test_cases/numbers.txt


numbers_3: 
	STAGE=3 ./encode < test_cases/numbers.txt | STAGE=3 ./decode | cmp - test_cases/numbers.txt



blank: 
	./encode < test_cases/blank.txt | ./decode | cmp - test_cases/blank.txt

baba_encode:
	echo "BABA" | STAGE=1 ./encode
	echo "BABA" | STAGE=2 ./encode -m 10 -p
	echo "BABA" | STAGE=2 ./encode -p


boot_info:
	#DBG=1 ./encode < test_cases/Boot-Info_20240527_2222.txt | DBG=1 ./decode | cmp - test_cases/Boot-Info_20240527_2222.txt
	#DBG=1 STAGE=2 ./encode -p < test_cases/Boot-Info_20240527_2222.txt | DBG=1 STAGE=2 ./decode  | cmp - test_cases/Boot-Info_20240527_2222.txt
	DBG=1 STAGE=3 ./encode  -p  < test_cases/Boot-Info_20240527_2222.txt | DBG=1 STAGE=3 ./decode  | cmp - test_cases/Boot-Info_20240527_2222.txt
	#DBG=1 STAGE=3 ./encode -p < test_cases/Boot-Info_20240527_2222.txt > junk

unreadable_2:
	DBG=1 STAGE=2 ./encode -p < test_cases/unreadable.dat | DBG=1 STAGE=2 ./decode  | cmp - test_cases/unreadable.dat

unreadable_3:
	DBG=1 STAGE=3 ./encode -p < test_cases/unreadable.dat | DBG=1 STAGE=3 ./decode  | cmp - test_cases/unreadable.dat

testing:
	STAGE=2 ./encode -p < test_cases/Boot-Info_20240527_2222.txt     > ./boot_data.txt
	cat boot_data.txt | ./codec_encode > ./boot_data.bin
	cat boot_data.bin | ./codec_decode > ./boot_data.verify


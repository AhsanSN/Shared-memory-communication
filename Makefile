
all:
	gcc -o producer producer.c
	gcc -o consumer consumer.c

clean:
	rm consumer producer producer.txt consumer.txt

rebuild: clean all

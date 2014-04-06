#include "cachelab.h"
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

int s = 0, E = 0, b = 0, verbosity = 0;
int S = 0, B = 0;
int hit_count = 0, miss_count = 0, eviction_count = 0;
long long ** cache = NULL;
long long set_index_mask = 0;
long long lru_counter = 0;

void printUsage(char * name);
void initCache();
void replayTrace(char * trace_file);
void freeCache();

int main(int argc, char * argv[])
{
	char *trace_file = NULL;
	char opt = 0;
	while ((opt = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
		switch (opt) {
			case 's':
				s = atoi(optarg);
			break;
			case 'E':
				E = atoi(optarg);
			break;
			case 'b':
				b = atoi(optarg);
			break;
			case 't':
				trace_file = optarg;
			break;
			case 'v':
				verbosity = 1;
			break;
			default:
				printUsage(argv[0]);
				return 1;
		}
	}
	
	if (s == 0 || E == 0 || b == 0 || trace_file == 0){
		printf("%s: Отсутствует необходимый аргумент командной строки\n", argv[0]);
		printUsage(argv[0]);
		return 1;
	}
	
	S = 1 << s;
	B = 1 << b;
	
	initCache();
	replayTrace(trace_file);
	freeCache();
	
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}

void printUsage(char * name){
	printf("Использование: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", name);
	puts("Опции:");
	puts("  -h         Напечатать это вспомогательное сообщение.");
	puts("  -v         Необязательный флаг, включающий подробный вывод.");
	puts("  -s <num>   Число битов для номера набора.");
	puts("  -E <num>   Число линий в наборе.");
	puts("  -b <num>   Число битов для сдвига блока.");
	puts("  -t <file>  Файл журнала.");
	puts("\nПримеры:");
	printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", name);
	printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", name);
}

void initCache(){
	int i;
	cache = malloc(S * sizeof(*cache));
	for (i=0; i<S; i++) cache[i] = calloc(3 * E, sizeof(*cache[i]));

	set_index_mask = S - 1;
}

void accessData(long long address){
	int i;
	long long Eaddress = (address >> b) & set_index_mask; //rbp30
	long long Caddress = (address >> (b+s)); //rbp28
	
	for (i=0; i<E; i++){
		if (cache[Eaddress][3*i+1] == Caddress && cache[Eaddress][3*i] != 0){
			hit_count++;
			if (verbosity) printf("hit ");
			cache[Eaddress][3*i+2] = lru_counter++;
			return;
		}
	}

	miss_count++;
	long long minLru = 0xffffffff;
	int EvicI = 0;
	if (verbosity) printf("miss ");
	for (i=0; i<E; i++)
		if (cache[Eaddress][3*i+2] < minLru){
			EvicI = i;
			minLru = cache[Eaddress][3*i+2];
		}
	
	if (cache[Eaddress][3*EvicI] != 0){
		eviction_count++;
		if (verbosity) printf("eviction ");
	}
	cache[Eaddress][3*EvicI] = 1;
	cache[Eaddress][3*EvicI+1] = Caddress;
	cache[Eaddress][3*EvicI+2] = lru_counter++;
}

void replayTrace(char * trace_file){
	char buf[1000];
	FILE *fp = fopen(trace_file, "r");
	if (fp == NULL){
		printf("%s: No such file or directory!", trace_file);
		return;
	}
	
	while(fgets(buf, 1000, fp)){
		unsigned long long address = 0;
		unsigned length = 0;
		if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M'){
			sscanf(buf+2, "%llx,%u", &address , &length);
			
			if (verbosity) printf("%c %llx,%u ", buf[1], address, length);
			accessData(address);
		}
		if (buf[1] == 'M')
			accessData(address);
		if (verbosity) putchar('\n');
	}
	fclose(fp);
}

void freeCache(){
	int i;
	for (i=0; i<S; i++) free(cache[i]);
	free(cache);
}

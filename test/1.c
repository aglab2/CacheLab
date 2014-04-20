int main(){
	int k = 0;
	
	int blocki = 0;
	int blockj = 0;
	
	for (k=0; k<64; k++){
		blocki = k%2 + 2*((k/4)%2);
		blockj = ((k%4)/2) + 2*(k/8);
		
		printf("%d %d\n", blocki, blockj);
	}
	
	return 0;
}
